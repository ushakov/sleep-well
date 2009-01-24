# fuses: atmega8 high=d9, low=ff

ASFLAGS = -Wa, -gstabs
CPFLAGS	= -Os -Wall -Wa,-ahlms=$(<:.c=.lst) -std=c99 

CC	= avr-gcc
AS	= avr-gcc -x assembler-with-cpp	
RM	= rm -f
RN	= mv
CP	= cp
OBJCOPY	= avr-objcopy
OBJDUMP	= avr-objdump
SIZE	= avr-size
INCDIR	= .
FORMAT = ihex	

SENSOR_MCU=attiny44
SENSOR_CPFLAGS += -mmcu=$(SENSOR_MCU) -DF_CPU=8000000UL -DSENSOR
SENSOR_ASFLAGS += -mmcu=$(SENSOR_MCU) -DF_CPU=8000000UL

RECEIVER_MCU=atmega16
RECEIVER_CPFLAGS += -mmcu=$(RECEIVER_MCU) -DF_CPU=8000000UL
RECEIVER_ASFLAGS += -mmcu=$(RECEIVER_MCU) -DF_CPU=8000000UL

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

%.dmp: %.elf
	$(OBJDUMP) -h -S $< > $@

#################################################################
# Description of source files starts here

TARGETS = sensor receiver accmain sdmain logger
ALL_SRC = sensor.c uart.c put.c delay.c manchester.c receiver.c \
          accmain.c accel.c eeprom.c sdmain.c mmc.c \
          time.c logger.c

default:
	echo "no default here"

sensor: MCU = $(SENSOR_MCU)
sensor: CPFLAGS += $(SENSOR_CPFLAGS)
sensor: ASFLAGS += $(SENSOR_ASFLAGS)
sensor.elf: sensor.o delay.o manchester.o

receiver: MCU = $(RECEIVER_MCU)
receiver: CPFLAGS += $(RECEIVER_CPFLAGS)
receiver: ASFLAGS += $(RECEIVER_ASFLAGS)
receiver.elf: receiver.o delay.o manchester.o uart.o

accmain.elf: MCU = atmega8
accmain.elf: CPFLAGS += -mmcu=$(MCU) -DF_CPU=8000000UL
accmain.elf: ASFLAGS += -mmcu=$(MCU) -DF_CPU=8000000UL
accmain.elf: accmain.o delay.o accel.o uart.o put.o mmc.o

sdmain.elf: MCU = atmega8
sdmain.elf: CPFLAGS += -mmcu=$(MCU) -DF_CPU=8000000UL
sdmain.elf: ASFLAGS += -mmcu=$(MCU) -DF_CPU=8000000UL
sdmain.elf: sdmain.o delay.o uart.o put.o mmc.o

logger.elf: MCU = atmega8
logger.elf: CPFLAGS += -mmcu=$(MCU) -DF_CPU=8000000UL
logger.elf: ASFLAGS += -mmcu=$(MCU) -DF_CPU=8000000UL
logger.elf: logger.o delay.o mmc.o accel.o time.o eeprom.o #uart.o put.o 

$(TARGETS:%=%-build): %-build: clean %.elf %.bin %.hex %.eep %.dmp
	$(SIZE) $*.elf
#	-git commit -e -a -uno
	@echo $< done

$(TARGETS): %: %-build
	./upload.sh $*


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
	$(RM) $(TARGETS:=.dmp)
	$(RM) *~
