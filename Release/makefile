################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Core/resources/lang/subdir.mk
-include Core/resources/images/subdir.mk
-include Core/os/subdir.mk
-include Core/libs/graphics/subdir.mk
-include Core/kernel/uart/subdir.mk
-include Core/kernel/timers/subdir.mk
-include Core/kernel/ram/subdir.mk
-include Core/kernel/joyport/subdir.mk
-include Core/kernel/filesys/FatFs/option/subdir.mk
-include Core/kernel/filesys/FatFs/subdir.mk
-include Core/kernel/filesys/subdir.mk
-include Core/kernel/display/subdir.mk
-include Core/kernel/audio/subdir.mk
-include Core/kernel/Drivers/USB/subdir.mk
-include Core/kernel/Drivers/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Core/kernel/Drivers/STM32_USB_Device_Library/Class/MSC/Src/subdir.mk
-include Core/kernel/Drivers/STM32H7xx_HAL_Driver/Src/subdir.mk
-include Core/kernel/Drivers/subdir.mk
-include Core/kernel/subdir.mk
-include Core/Startup/subdir.mk
-include Core/subdir.mk
-include .metadata/.plugins/org.eclipse.cdt.make.core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := sidbox5
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX := 
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME).$(BUILD_ARTIFACT_EXTENSION)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
sidbox5.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
sidbox5.list \

OBJCOPY_BIN += \
sidbox5.bin \


# All Target
all: main-build

# Main-build Target
main-build: sidbox5.elf secondary-outputs

# Tool invocations
sidbox5.elf: $(OBJS) $(USER_OBJS) /mnt/LinuxDatas/work/electronics/SidBox5.6/STM32H743IITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "sidbox5.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"/mnt/LinuxDatas/work/electronics/SidBox5.6/STM32H743IITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="sidbox5.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

sidbox5.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "sidbox5.list"
	@echo 'Finished building: $@'
	@echo ' '

sidbox5.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "sidbox5.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified
.SECONDARY:

-include ../makefile.targets
