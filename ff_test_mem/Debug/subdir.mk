################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ff_writer_engine.cpp \
../ffmpeg_pipeline.cpp \
../logs.cpp \
../main.cpp \
../vars.cpp 

OBJS += \
./ff_writer_engine.o \
./ffmpeg_pipeline.o \
./logs.o \
./main.o \
./vars.o 

CPP_DEPS += \
./ff_writer_engine.d \
./ffmpeg_pipeline.d \
./logs.d \
./main.d \
./vars.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Linux GCC C++ Compiler'
	arm-none-eabi-g++ -D__STDC_CONSTANT_MACROS -I/home/romar/workspace/libs/ffmpeg-2.4-arm/include -I/home/romar/workspace/ff_test_mem -I/home/romar/ti-ezsdk_dm816x-evm_5_05_01_04/linux-devkit/arm-none-linux-gnueabi/usr/include/glib-2.0 -I/home/romar/ti-ezsdk_dm816x-evm_5_05_01_04/linux-devkit/arm-none-linux-gnueabi/usr/lib/glib-2.0/include -I/home/romar/ti-ezsdk_dm816x-evm_5_05_01_04/linux-devkit/arm-none-linux-gnueabi/usr/include/glib-2.0/glib -I/home/romar/ti-ezsdk_dm816x-evm_5_05_01_04/linux-devkit/arm-none-linux-gnueabi/usr/include/glib-2.0 -O0 -Wall -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm926ej-s -mthumb -mfloat-abi=softfp -mfpu=neon -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


