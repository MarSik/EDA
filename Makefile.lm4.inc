PREFIX=arm-none-eabi

SUPPORT = ~/Dropbox/EDA/repo/lib/LM4F
LDSCRIPT = $(SUPPORT)/LM4F.ld

-include ~/Dropbox/EDA/Makefile.common.inc

CPUFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16
SRC += $(SUPPORT)/LM4F_startup.c

LDEXTRA += --entry=rst_handler

CFLAGS += -Dgcc                    #Flag used in driverlib for compiler specific flags
CFLAGS += -DPART_LM4F120H5QR       #Flag used in driverlib for specifying the silicon version.
CFLAGS += -DTARGET_IS_BLIZZARD_RA1 #Used in driverlib to determine what is loaded in rom.

CXXFLAGS += -Dgcc                    #Flag used in driverlib for compiler specific flags
CXXFLAGS += -DPART_LM4F120H5QR       #Flag used in driverlib for specifying the silicon version.
CXXFLAGS += -DTARGET_IS_BLIZZARD_RA1 #Used in driverlib to determine what is loaded in rom.

upload: flash
flash: $(TARGET).elf
	openocd ~/Dropbox/EDA/repo/openocd/LM4F.cfg
	$(GDB) #connect "prog $(TARGET).elf"

clean:
	rm -fr $(OBJ) $(TARGET).elf $(LST)

.PHONY: all upload flash clean
.PRECIOUS: .elf

