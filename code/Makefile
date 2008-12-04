MCU = attiny44
# AVRLIB = /home/ushakov/devel/embedded/avrlib

ASFLAGS = -Wa, -gstabs
CPFLAGS	= -g -Os -Wall -Wa,-ahlms=$(<:.c=.lst) -std=c99 -ffunction-sections -fdata-sections # -dr -finline-functions-called-once 

CC	= avr-gcc
AS	= avr-gcc -x assembler-with-cpp	
RM	= rm -f
RN	= mv
CP	= cp
OBJCOPY	= avr-objcopy
SIZE	= avr-size
INCDIR	= .
FORMAT = ihex	

CPFLAGS += -mmcu=$(MCU) -DF_CPU=8000000UL
ASFLAGS += -mmcu=$(MCU) -DF_CPU=8000000UL

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

TARGETS = sensor receiver
ALL_SRC = sensor.c uart.c delay.c manchester.c receiver.c

default: all

all: $(TARGETS)

sensor: CPFLAGS += -DSENSOR

sensor.elf: sensor.o delay.o manchester.o

receiver.elf: receiver.o delay.o manchester.o

$(TARGETS): %: %.elf %.bin %.hex %.eep
	$(SIZE) $*.elf
	-git commit -e -a
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
