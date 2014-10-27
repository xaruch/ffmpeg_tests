################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
ff_test_mem.hex \

SECONDARY_LIST += \
ff_test_mem.lst \

SECONDARY_SIZE += \
ff_test_mem.siz \


# All Target
all: ff_test_mem.elf secondary-outputs

# Tool invocations
ff_test_mem.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Sourcery Linux GCC C++ Linker'
	arm-none-eabi-g++ -L"/home/romar/workspace/arlogging/Debug" -L/home/romar/workspace/libs/ffmpeg-2.4-arm/lib -L/home/romar/ti-ezsdk_dm816x-evm_5_05_01_04/linux-devkit/arm-none-linux-gnueabi/usr/lib -Wl,-Map,ff_test_mem.map -mcpu=arm926ej-s -mthumb -mfloat-abi=softfp -mfpu=neon -g3 -gdwarf-2 -o "ff_test_mem.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ff_test_mem.hex: ff_test_mem.elf
	@echo 'Invoking: ARM Sourcery Linux GNU Create Flash Image'
	arm-none-eabi-objcopy -O binary ff_test_mem.elf  "ff_test_mem.hex"
	@echo 'Finished building: $@'
	@echo ' '

ff_test_mem.lst: ff_test_mem.elf
	@echo 'Invoking: ARM Sourcery Linux GNU Create Listing'
	arm-none-eabi-objdump -h -S ff_test_mem.elf > "ff_test_mem.lst"
	@echo 'Finished building: $@'
	@echo ' '

ff_test_mem.siz: ff_test_mem.elf
	@echo 'Invoking: ARM Sourcery Linux GNU Print Size'
	arm-none-eabi-size  --format=berkeley ff_test_mem.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(SECONDARY_FLASH)$(CXX_DEPS)$(S_UPPER_DEPS)$(SECONDARY_LIST)$(C++_DEPS)$(SECONDARY_SIZE)$(ASM_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS) ff_test_mem.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
