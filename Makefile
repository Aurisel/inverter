#TOOLCHAIN_PATH := C:/Users/jack/arm-none-eabi-gcc/bin/
TARGET := main
LINK_SCRIPT := driver/stm32g431cbux.ld
TOOLCHAIN_PREFIX := arm-none-eabi-
CC := g++
AS := as
AR := ar
SIZE := size
OBJCOPY := objcopy
CFLAG := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -O0 -Idriver/st/ll/inc -Idriver/st/dev/inc -Idriver/cmsis/core -Idriver/cmsis/dsp/inc -Iuser/inc -Ilib/oled/inc -g -save-temps=obj -Wl,--gc-sections -DUSE_FULL_LL_DRIVER -ffunction-sections -fdata-sections -specs=nano.specs -Ldriver/cmsis/dsp/lib -larm_cortexM4lf_math
BUILD_DIR := build/
BINARY_DIR := bin/
#LINK := stlink-v2
LINK := cmsis-dap

OBJ :=

LL_SRC := $(wildcard driver/st/ll/src/*.c)
OBJ += $(patsubst driver/st/ll/src/%.c,build/%.o,$(LL_SRC))

DEV_SRC := $(wildcard driver/st/dev/src/*.c)
OBJ += $(patsubst driver/st/dev/src/%.c,build/%.o,$(DEV_SRC))

START_UP := $(wildcard driver/*.s)
OBJ += $(patsubst driver/%.s,build/%.o,$(START_UP))

POSIX_SRC := $(wildcard driver/*.cpp)
OBJ += $(patsubst driver/%.cpp,build/%.o,$(POSIX_SRC))

USER_SRC := $(wildcard user/src/*.cpp)
OBJ += $(patsubst user/src/%.cpp,build/%.o,$(USER_SRC))

USER_SRC := $(wildcard user/src/*.c)
OBJ += $(patsubst user/src/%.c,build/%.o,$(USER_SRC))

LIB_SRC := $(wildcard lib/oled/src/*.cpp)
OBJ += $(patsubst lib/oled/src/%.cpp,build/%.o,$(LIB_SRC))



all: download

download: bin
	@openocd -f interface/$(LINK).cfg -f target/stm32g4x.cfg -c "adapter speed 5000; program bin/main.elf verify reset exit"
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(SIZE) bin/$(TARGET).elf

bin: $(OBJ)
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(CC) $(CFLAG) -o bin/$(TARGET).elf -T $(LINK_SCRIPT) -Wl,-Map=bin/$(TARGET).map $(OBJ)
	@echo "LD     OBJ"
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(SIZE) bin/$(TARGET).elf
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(OBJCOPY) -O binary bin/$(TARGET).elf bin/$(TARGET).bin
	
build/%.o: driver/st/ll/src/%.c
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(CC) -c $(CFLAG) -o $@ $<
	@echo "CC      $<"

build/%.o: driver/st/dev/src/%.c
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(CC) -c $(CFLAG) -o $@ $<
	@echo "CC      $<"

build/%.o: driver/%.s
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(CC) -c $(CFLAG) -o $@ $<
	@echo "AS      $<"

build/%.o: driver/%.cpp
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(CC) -c $(CFLAG) -o $@ $<
	@echo "CXX     $<"

build/%.o: lib/oled/src/%.cpp
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(CC) -c $(CFLAG) -o $@ $<
	@echo "CXX     $<"

build/%.o: lib/oled/src/%.c
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(CC) -c $(CFLAG) -o $@ $<
	@echo "CC      $<"

build/%.o: user/src/%.cpp
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(CC) -c $(CFLAG) -o $@ $<
	@echo "CXX     $<"

build/%.o: user/src/%.c
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(CC) -c $(CFLAG) -o $@ $<
	@echo "CC      $<"

clean:
	@rm -rf build/* bin/*
