PREFIX=arm-none-eabi

SUPPORT = ~/Dropbox/EDA/repo/lib/LM4F
LDSCRIPT = $(SUPPORT)/LM4F.ld

-include ~/Dropbox/EDA/Makefile.common.inc

SRC += $(SUPPORT)/LM4F_startup.c

upload: flash
flash: $(TARGET).elf
	openocd ~/Dropbox/EDA/repo/openocd/LM4F.cfg
	$(GDB) #connect "prog $(TARGET).elf"

clean:
	rm -fr $(OBJ) $(TARGET).elf $(LST)

.PHONY: all upload flash clean
.PRECIOUS: .elf

