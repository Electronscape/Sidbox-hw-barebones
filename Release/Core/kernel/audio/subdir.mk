################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/kernel/audio/adc.c \
../Core/kernel/audio/audio.c \
../Core/kernel/audio/dac.c 

OBJS += \
./Core/kernel/audio/adc.o \
./Core/kernel/audio/audio.o \
./Core/kernel/audio/dac.o 

C_DEPS += \
./Core/kernel/audio/adc.d \
./Core/kernel/audio/audio.d \
./Core/kernel/audio/dac.d 


# Each subdirectory must supply rules for building sources it contributes
Core/kernel/audio/adc.o: ../Core/kernel/audio/adc.c Core/kernel/audio/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/kernel -I../Core/kernel/Drivers/ -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc -I../Core/kernel/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Core/kernel/Drivers/USB -I../Core/kernel/Drivers/STM32_USB_Device_Library/Core/Inc -I../Core/kernel/Drivers/STM32_USB_Device_Library/Class/MSC/Inc -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Core/kernel/Drivers/CMSIS/Include -I../Core/kernel/filesys -I../Core/kernel/display -I../Core/kernel/uart -I../Core/resources -I../Core/kernel/filesys/FatFs/ -I../Core/libs -I../Core -I../Core/os -Ofast -ffunction-sections -mslow-flash-data -Wall -Wextra -Wfatal-errors -Wswitch-enum -fstack-usage -MMD -MP -MF"Core/kernel/audio/adc.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/kernel/audio/audio.o: ../Core/kernel/audio/audio.c Core/kernel/audio/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/kernel -I../Core/kernel/Drivers/ -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc -I../Core/kernel/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Core/kernel/Drivers/USB -I../Core/kernel/Drivers/STM32_USB_Device_Library/Core/Inc -I../Core/kernel/Drivers/STM32_USB_Device_Library/Class/MSC/Inc -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Core/kernel/Drivers/CMSIS/Include -I../Core/kernel/filesys -I../Core/kernel/display -I../Core/kernel/uart -I../Core/resources -I../Core/kernel/filesys/FatFs/ -I../Core/libs -I../Core -I../Core/os -Ofast -ffunction-sections -mslow-flash-data -Wall -Wextra -Wfatal-errors -Wswitch-enum -fstack-usage -MMD -MP -MF"Core/kernel/audio/audio.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/kernel/audio/dac.o: ../Core/kernel/audio/dac.c Core/kernel/audio/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/kernel -I../Core/kernel/Drivers/ -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc -I../Core/kernel/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Core/kernel/Drivers/USB -I../Core/kernel/Drivers/STM32_USB_Device_Library/Core/Inc -I../Core/kernel/Drivers/STM32_USB_Device_Library/Class/MSC/Inc -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Core/kernel/Drivers/CMSIS/Include -I../Core/kernel/filesys -I../Core/kernel/display -I../Core/kernel/uart -I../Core/resources -I../Core/kernel/filesys/FatFs/ -I../Core/libs -I../Core -I../Core/os -Ofast -ffunction-sections -mslow-flash-data -Wall -Wextra -Wfatal-errors -Wswitch-enum -fstack-usage -MMD -MP -MF"Core/kernel/audio/dac.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

