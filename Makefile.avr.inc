# Debugging format
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
DEBUG = stabs

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
COPT = s
CSTANDARD = -std=gnu99
CDEBUG = -g$(DEBUG)
CWARN = -Wall -Wstrict-prototypes 
CXXWARN = -Wall
CTUNING = -Wall -O$(COPT) -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields \
-fno-exceptions -ffunction-sections -fdata-sections

# Place -D or -U options here
CDEFS = -DF_CPU=$(F_CPU)
CXXDEFS = -DF_CPU=$(F_CPU)

CFLAGS = -Wa,-adhlns=$(<:.c=.lst) $(CDEBUG) $(CDEFS) $(CINCS) $(CTUNING) $(CWARN) $(CSTANDARD) $(CEXTRA)
CXXFLAGS = -Wa,-adhlns=$(<:.cpp=.lst) $(CDEBUG) $(CXXDEFS) $(CINCS) $(CTUNING) $(CXXWARN) $(CXXEXTRA)
ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-g$(DEBUG) 
LDFLAGS = -Wl,--gc-sections $(LDEXTRA)

# Programming support using avrdude. Settings and variables.
AVRDUDE_PROGRAMMER = $(PROTOCOL)
AVRDUDE_PORT = $(PORT)
AVRDUDE_FLAGS = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) \
  -b $(UPLOAD_RATE)
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep
AVRDUDE_WRITE_LFUSE = -U lfuse:w:$(LFUSE):m
AVRDUDE_WRITE_HFUSE = -U hfuse:w:$(HFUSE):m
AVRDUDE_WRITE_EFUSE = -U efuse:w:$(EFUSE):m

# Program settings
CC = $(AVRDUDE_DIR)/avr-gcc
CXX = $(AVRDUDE_DIR)/avr-g++
OBJCOPY = $(AVRDUDE_DIR)/avr-objcopy
OBJDUMP = $(AVRDUDE_DIR)/avr-objdump
AR  = $(AVRDUDE_DIR)/avr-ar
SIZE = $(AVRDUDE_DIR)/avr-size
NM = $(AVRDUDE_DIR)/avr-nm
AVRDUDE = $(AVRDUDE_DIR)/avrdude
REMOVE = rm -f
MV = mv -f

# Define all object files.
OBJ = $(SRC:.c=.o) $(CXXSRC:.cpp=.o) $(ASRC:.S=.o)

# Define all listing files.
LST = $(ASRC:.S=.lst) $(CXXSRC:.cpp=.lst) $(SRC:.c=.lst)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS)
ALL_CXXFLAGS = -mmcu=$(MCU) -I. $(CXXFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)


# Default target.
all: build

build: elf eep size hex

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss 
sym: $(TARGET).sym

# Program the device.
upload: $(TARGET).hex $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)
  
flash: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

eeprom: $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_EEPROM)

fuse:
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_LFUSE) $(AVRDUDE_WRITE_HFUSE) $(AVRDUDE_WRITE_EFUSE)

# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
--change-section-address .data-0x800000 \
--change-section-address .bss-0x800000 \
--change-section-address .noinit-0x800000 \
--change-section-address .eeprom-0x810000 


coff: $(TARGET).elf
	$(COFFCONVERT) -O coff-avr $(TARGET).elf $(TARGET).cof


extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr $(TARGET).elf $(TARGET).cof


size: $(TARGET).elf
	$(SIZE) -A --common $<

.SUFFIXES: .elf .hex .eep .lss .sym

.elf.hex:
	$(OBJCOPY) -O $(FORMAT) -R .fuse -R .eeprom $< $@

.elf.eep:
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
.elf.lss:
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
.elf.sym:
	$(NM) -n $< > $@

# Link: create ELF output file from library.
$(TARGET).elf: $(OBJ)
	$(CC) $(ALL_CFLAGS) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

# Compile: create object files from C++ source files.
.cpp.o:
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@ 

# Compile: create object files from C source files.
.c.o:
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 


# Compile: create assembler files from C source files.
.c.s:
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
.S.o:
	$(CC) -c $(ALL_ASFLAGS) $< -o $@



# Target: clean project.
clean:
	$(REMOVE) $(TARGET).hex $(TARGET).eep $(TARGET).cof $(TARGET).elf \
	$(TARGET).map $(TARGET).sym $(TARGET).lss core.a \
	$(OBJ) $(LST) $(SRC:.c=.s) $(SRC:.c=.d) $(CXXSRC:.cpp=.s) $(CXXSRC:.cpp=.d)

.PHONY:	all upload size fuses eeprom build elf hex eep lss sym program coff extcoff clean
.PRECIOUS: .elf .hex .eep
