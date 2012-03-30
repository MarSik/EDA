#!/usr/bin/env python
# encoding: utf-8
"""
Created by Sean Nelson on 2009-10-14.
Copyright 2009 Sean Nelson <audiohacked@gmail.com>

This file is part of pyBusPirate.

pyBusPirate is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

pyBusPirate is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with pyBusPirate.  If not, see <http://www.gnu.org/licenses/>.
"""
import time, sys, optparse
from pyBusPirateLite.SPI import *

def read_list_data(size):
	return [0]*(size+1)

def parse_prog_args():
	parser = optparse.OptionParser(usage="%prog [options] filename",
									version="%prog 1.0")

	parser.set_defaults(command="read")

	parser.add_option("-v", "--verbose",
						action="store_true", dest="verbose", default=True,
						help="make lots of noise [default]")
	parser.add_option("-q", "--quiet",
						action="store_false", dest="verbose",
						help="be mute")
	parser.add_option("-r", "--read",
						action="store_const", dest="command", const="read",
						help="read from SPI to file [default]")
	parser.add_option("-w", "--write",
						action="store_const", dest="command", const="write",
						help="write from file to SPI")
	parser.add_option("-e", "--erase",
						action="store_const", dest="command", const="erase",
						help="erase SPI")
	parser.add_option("-i", "--id",
						action="store_const", dest="command", const="id",
						help="print Chip ID")
	parser.add_option("-s", "--size",
	 					dest="flash_size", default=0,
						help="Size of Flashchip in bytes", type="int")

	parser.add_option("-P", "--port",
	 					dest="buspirate", default="/dev/cu.buspirate",
						help="Buspirate port")


	(options, args) = parser.parse_args()

	if options.command == "id":
		return (options, args)
	elif len(args) != 1:
		parser.print_help()
		print options
		sys.exit(1)
	else:
		return (options, args)

def wait(spi):
    spi.CS_Low()
    spi.bulk_trans(1, [0x5]) # status register
    while True:
        data = spi.bulk_trans(1, [0x00])[0]
        if (ord(data) & 0x1):
            continue
        else:
            break
    spi.CS_High()

	
""" enter binary mode """
if __name__ == '__main__':
	data = ""
	(opt, args)  = parse_prog_args()

	if opt.command == "read":
		f=open(args[0], 'wb')
	elif opt.command == "write":
		f=open(args[0], 'rb')

	spi = SPI(opt.buspirate, 115200)

	print "Entering binmode: ",
	if spi.BBmode():
		print "OK."
	else:
		print "failed."
		sys.exit()

	print "Entering raw SPI mode: ",
	if spi.enter_SPI():
		print "OK."
	else:
		print "failed."
		sys.exit()
		
	print "Configuring SPI."
	if not spi.cfg_pins(PinCfg.POWER | PinCfg.CS):
		print "Failed to set SPI peripherals."
		sys.exit()
                
	if not spi.set_speed(SPISpeed._8MHZ):
		print "Failed to set SPI Speed."
		sys.exit()
                
	if not spi.cfg_spi(SPICfg.CLK_EDGE | SPICfg.OUT_TYPE):
		print "Failed to set SPI configuration.";
		sys.exit()
                
	spi.timeout(0.2)

	if opt.command == "read":
		print "Reading EEPROM."
		spi.CS_Low()
		spi.bulk_trans(5, [0xB, 0, 0, 0, 0])
		for i in range((int(opt.flash_size)/16)):
			data = spi.bulk_trans(16, read_list_data(16))
			f.write(data)
		spi.CS_High()

	elif opt.command == "write":
		print "Writing EEPROM."
                f.seek(0, 2)
                data_total = data_len = opt.flash_size or f.tell()
                f.seek(0, 0)
                page = 0
		while data_len>0:
                    # write enable
                    spi.CS_Low()
                    spi.bulk_trans(1, [0x6])
                    spi.CS_High()

                    sys.stdout.write("\nPP page %02x%02x%02x " % (page / 256, page % 256, 0))
                    
                    spi.CS_Low()
                    data = spi.bulk_trans(2, [0x5, 0x0])[1:]
                    spi.CS_High()
                    if ord(data) & 0x2:
                        sys.stdout.write("WEL ")
                    if ord(data) & 0x1:
                        sys.stdout.write("WIP ")

                    # write page
                    spi.CS_Low()
                    spi.bulk_trans(4, [0x02, page / 256, page % 256, 0])
                    for i in range(16): # max 256 (= 16*16) bytes
                        sys.stdout.write("\n0x%06x / 0x%06x " % (data_total - data_len, data_total))
                        if data_len>=16:
                            data = [ord(d) for d in f.read(16)]
                            spi.bulk_trans(len(data), data)
                            data_len -= 16
                        else:
                            data = [ord(d) for d in f.read(data_len)]
                            spi.bulk_trans(len(data), data)
                            data_len = 0
                            break
                        
                    spi.CS_High()

                    # wait until page is written
                    wait(spi)
                    page += 1

                print "\n0x%06x / 0x%06x" % (data_total - data_len, data_total)                    
                print ".done"

                        

	elif opt.command == "id":
		print "Reading Chip ID: ",
		spi.CS_Low()
		d = spi.bulk_trans(4, [0x9F, 0, 0, 0, 0])
		spi.CS_High()
		for each in d[1:]:
			print "%02X " % ord(each),
		print

	elif opt.command == "erase":
            # write enable
            spi.CS_Low()
            spi.bulk_trans(1, [0x6])
            spi.CS_High()

            # bulk erase
            spi.CS_Low()
            d = spi.bulk_trans(1, [0xC7])
            spi.CS_High()
            time.sleep(0.2)
            
            # wait until write ends
            wait(spi)
            print "EEPROM was erased"
                

	print "Reset Bus Pirate to user terminal: ",
	if spi.resetBP():
		print "OK."
	else:
		print "failed."
		sys.exit()
		
