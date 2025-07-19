#TOOLCHAIN_PATH := C:/Users/jack/arm-none-eabi-gcc/bin/
TARGET			:=	main
TOOLCHAIN_PREFIX:=	arm-none-eabi-
CC				:=	g++
C_STANDARD		:=	c++11
AS				:=	as
AR				:=	ar
SIZE			:=	size
OBJCOPY			:=	objcopy
HEADER_PATHS	:=	driver/st/ll/inc \
					driver/st/dev/inc \
					driver/cmsis/core \
					driver/cmsis/dsp/inc \
					user/inc \
					lib/oled/inc \
					lib/pll

CFALG_HEADER	:=	$(patsubst %,-I%,$(HEADER_PATHS))

LIB_PATHS		:=	lib driver/cmsis/dsp/lib
LIB				:=	ll arm_cortexM4lf_math
LINK_PATH		:=	$(patsubst %,-L%,$(LIB_PATHS))
LINK_LIB		:=	$(patsubst %,-l%,$(LIB))

CFLAG			:=	-mcpu=cortex-m4 \
					-mfpu=fpv4-sp-d16 \
					-mthumb \
					-mfloat-abi=hard \
					-std=$(C_STANDARD) \
					-ffunction-sections \
					-fdata-sections \
					-g \
					-gstrict-dwarf \
					-Wall \
					-O3 \
					$(CFALG_HEADER) \
					-Wl,--gc-sections \
					-nostartfiles \
					-specs=nano.specs \
					-fno-exceptions \
					-fno-rtti \
					-DUSE_FULL_LL_DRIVER
LINK_FLAG		:=	$(LINK_PATH) $(LINK_LIB)


SOURCE_PATHS	:=	driver/st/dev/src \
					driver \
					user/src \
					lib/oled/src \
					lib/pll

COMPILE_MODE ?= DEBUG

BUILD_DIR := build
BINARY_DIR := bin


LINK_SCRIPT := driver/stm32g431cbux.ld
STARTUP_FILE := driver/startup_stm32g431xx.s

OPENOCD_INTERFACE := cmsis-dap
#OPENOCD_INTERFACE := stlink-v2
OPENOCD_TARGET := stm32g4x


OBJ :=
OBJ += $(foreach SOURCE_PATH,$(SOURCE_PATHS),$(patsubst $(SOURCE_PATH)/%.c,$(BUILD_DIR)/%.o,$(wildcard $(SOURCE_PATH)/*.c)))
OBJ += $(foreach SOURCE_PATH,$(SOURCE_PATHS),$(patsubst $(SOURCE_PATH)/%.cpp,$(BUILD_DIR)/%.o,$(wildcard $(SOURCE_PATH)/*.cpp)))
OBJ += $(BUILD_DIR)/$(notdir $(basename $(STARTUP_FILE))).o
VPATH := $(SOURCE_PATHS) $(dir $(STARTUP_FILE))



$(shell mkdir -p $(BINARY_DIR) $(BUILD_DIR))

all: download

download: bin
	@openocd -f interface/$(OPENOCD_INTERFACE).cfg -f target/$(OPENOCD_TARGET).cfg -c "adapter speed 5000; program $(BINARY_DIR)/$(TARGET).elf verify reset exit"
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(SIZE) $(BINARY_DIR)/$(TARGET).elf

bin: $(OBJ)
	@echo "LD      $(LINK_SCRIPT)"
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(CC) $(CFLAG) -o $(BINARY_DIR)/$(TARGET).elf -T $(LINK_SCRIPT) -Wl,-Map=$(BINARY_DIR)/$(TARGET).map $(OBJ) $(LINK_FLAG)
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(SIZE) $(BINARY_DIR)/$(TARGET).elf
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(OBJCOPY) -O binary $(BINARY_DIR)/$(TARGET).elf $(BINARY_DIR)/$(TARGET).bin
	
$(BUILD_DIR)/%.o: %.c
	@echo "CC      $<"
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(CC) -c $(CFLAG) -o $@ $<

$(BUILD_DIR)/%.o: %.cpp
	@echo "CXX     $<"
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(CC) -c $(CFLAG) -o $@ $<

$(BUILD_DIR)/%.o: %.s
	@echo "AS      $<"
	@$(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)$(CC) -c $(CFLAG) -o $@ $<

clean:
	@rm -rf $(BUILD_DIR)/* $(BINARY_DIR)/*
