MCU = atmega16
# AVRLIB = /home/ushakov/devel/embedded/avrlib

ASFLAGS = -Wa, -gstabs
CPFLAGS	= -dr -g -Os -Wall -Wa,-ahlms=$(<:.c=.lst) -std=c99 -ffunction-sections -fdata-sections # -finline-functions-called-once 

CC	= avr-gcc
AS	= avr-gcc -x assembler-with-cpp	
RM	= rm -f
RN	= mv
CP	= cp
OBJCOPY	= avr-objcopy
SIZE	= avr-size
INCDIR	= .
FORMAT = ihex	

CPFLAGS += -mmcu=$(MCU)
ASFLAGS += -mmcu=$(MCU)

%.o : %.c 
	$(CC) -c $(CPFLAGS) -I$(INCDIR) $< -o $@

%.s : %.c
	$(CC) -S $(CPFLAGS) -I$(INCDIR) $< -o $@

%.o : %.s
	$(AS) -c $(ASFLAGS) -I$(INCDIR) $< -o $@

%.elf:
	$(CC) $^ $(LIB) -Wl,-Map=$*.map,--cref -mmcu=$(MCU) -o $@

%.bin: %.elf
	$(OBJCOPY) -O binary -R .eeprom $< $@

%.hex: %.elf
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O $(FORMAT) $< $@

#################################################################
# Description of source files starts here

TARGETS = test

default: test

all: $(TARGETS)

test.elf: test.c

$(TARGETS): %: %.elf %.bin %.hex %.eep
	$(SIZE) $*.elf
	git-commit -e -a -m "Autocommit when making $@"
	@echo $< done

clean:
	$(RM) $(ALL_SRC:.c=.o)
	$(RM) $(ALL_SRC:.c=.s)
	$(RM) $(ALL_SRC:.c=.lst)
	$(RM) $(TARGETS:=.map)
	$(RM) $(TARGETS:=.elf)
	$(RM) $(TARGETS:=.cof)
	$(RM) $(TARGETS:=.obj)
	$(RM) $(TARGETS:=.a90)
	$(RM) $(TARGETS:=.sym)
	$(RM) $(TARGETS:=.eep)
	$(RM) $(TARGETS:=.hex)
	$(RM) $(TARGETS:=.bin)
	$(RM) *~
