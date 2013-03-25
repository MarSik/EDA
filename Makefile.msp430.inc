PREFIX=msp430

-include ~/Dropbox/EDA/Makefile.common.inc

CPUFLAGS += -mmcu=$(MCU)

upload: flash
flash: $(TARGET).elf
	mspdebug rf2500 "prog $(TARGET).elf"



