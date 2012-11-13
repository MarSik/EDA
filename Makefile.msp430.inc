CC=msp430-gcc


DEBUG=

# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
COPT = s
CSTANDARD = -std=gnu99
CDEBUG = -g$(DEBUG)
CWARN = -Wno-main -Wall -Wstrict-prototypes
CTUNING = -Wall -O$(COPT) -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields \
-fno-exceptions -ffunction-sections -fdata-sections

CDEFS = -mmcu=$(MCU)
CFLAGS = -Wa,-adhlns=$(<:.c=.lst) $(CDEBUG) $(CDEFS) $(CINCS) $(CTUNING) $(CWARN) $(CSTANDARD) $(CEXTRA)
LDFLAGS = -Wl,--gc-sections,-Map=$(TARGET).map $(LDEXTRA)
ASFLAGS = $(CDEFS) -x assembler-with-cpp -Wa,-adhlns=$(<:.S=.lst),-g$(DEBUG)

# Define all object files.
OBJ = $(SRC:.c=.o) $(ASRC:.S=.o)

# Define all listing files.
LST = $(SRC:.c=.lst) $(ASRC:.S=.lst)

all: $(TARGET).elf

$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@ $<


# Compile: create object files from C source files.
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

# Assemble: create object files from assembler source files.
.S.o:
	$(CC) -c $(ASFLAGS) $< -o $@


upload: flash
flash: $(TARGET).elf
	mspdebug rf2500 "prog $(TARGET).elf"

clean:
	rm -fr $(OBJ) $(TARGET).elf $(LST)

.PHONY: all upload flash clean
.PRECIOUS: .elf

