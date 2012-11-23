PREFIX=arm-none-eabi

SUPPORT = ~/Dropbox/EDA/repo/lib/STM32F4
LDSCRIPT = $(SUPPORT)/stm32_flash.ld

-include ~/Dropbox/EDA/Makefile.common.inc

CFLAGS += -I$(SUPPORT) -I$(SUPPORT)/inc -I$(SUPPORT)/inc/core -I$(SUPPORT)/inc/peripherals
LDFLAGS += -L$(SUPPORT) -lstm32f4

SRC += $(SUPPORT)/system_stm32f4xx.c
ASRC +=$(SUPPORT)/startup_stm32f4xx.s

upload: flash
flash: $(TARGET).elf
	openocd ~/Dropbox/EDA/repo/openocd/STM32F4.cfg
	$(GDB) #connect "prog $(TARGET).elf"

clean:
	rm -fr $(OBJ) $(TARGET).elf $(LST)

.PHONY: all upload flash clean
.PRECIOUS: .elf

