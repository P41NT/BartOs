# toolchain
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# target
TARGET = firmware

# cpu
CPU = -mcpu=cortex-m4 -mthumb

# compiler flags
CFLAGS = $(CPU) -O2 -g -Wall -ffreestanding

# linker flags
LDFLAGS = $(CPU) -T linker/stm32f401.ld -nostdlib

# source files
SRC = \
src/main.c \
src/uart.c \
src/runtime.c \
startup/startup_stm32f401xe.s

# object files
OBJ = $(SRC:.c=.o)
OBJ := $(OBJ:.s=.o)

# default target
all: $(TARGET).bin

# build elf
$(TARGET).elf: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@
	$(SIZE) $@

# convert to bin
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# compile c files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# compile asm files
%.o: %.s
	$(CC) $(CPU) -c $< -o $@

# flash firmware
flash: $(TARGET).bin
	st-flash write $(TARGET).bin 0x08000000

# erase chip
erase:
	st-flash erase

# clean build
clean:
	rm -f $(OBJ) $(TARGET).elf $(TARGET).bin
