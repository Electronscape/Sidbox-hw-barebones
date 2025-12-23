################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/kernel/filesys/FatFs/diskio.c \
../Core/kernel/filesys/FatFs/ff.c \
../Core/kernel/filesys/FatFs/ff_gen_drv.c 

OBJS += \
./Core/kernel/filesys/FatFs/diskio.o \
./Core/kernel/filesys/FatFs/ff.o \
./Core/kernel/filesys/FatFs/ff_gen_drv.o 

C_DEPS += \
./Core/kernel/filesys/FatFs/diskio.d \
./Core/kernel/filesys/FatFs/ff.d \
./Core/kernel/filesys/FatFs/ff_gen_drv.d 


# Each subdirectory must supply rules for building sources it contributes
Core/kernel/filesys/FatFs/diskio.o: ../Core/kernel/filesys/FatFs/diskio.c Core/kernel/filesys/FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/kernel -I../Core/kernel/Drivers/ -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc -I../Core/kernel/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Core/kernel/Drivers/USB -I../Core/kernel/Drivers/STM32_USB_Device_Library/Core/Inc -I../Core/kernel/Drivers/STM32_USB_Device_Library/Class/MSC/Inc -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Core/kernel/Drivers/CMSIS/Include -I../Core/kernel/filesys -I../Core/kernel/display -I../Core/kernel/uart -I../Core/resources -I../Core/kernel/filesys/FatFs/ -I../Core/libs -I../Core -I../Core/os -Ofast -ffunction-sections -mslow-flash-data -Wall -Wextra -Wfatal-errors -Wswitch-enum -fstack-usage -MMD -MP -MF"Core/kernel/filesys/FatFs/diskio.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/kernel/filesys/FatFs/ff.o: ../Core/kernel/filesys/FatFs/ff.c Core/kernel/filesys/FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/kernel -I../Core/kernel/Drivers/ -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc -I../Core/kernel/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Core/kernel/Drivers/USB -I../Core/kernel/Drivers/STM32_USB_Device_Library/Core/Inc -I../Core/kernel/Drivers/STM32_USB_Device_Library/Class/MSC/Inc -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Core/kernel/Drivers/CMSIS/Include -I../Core/kernel/filesys -I../Core/kernel/display -I../Core/kernel/uart -I../Core/resources -I../Core/kernel/filesys/FatFs/ -I../Core/libs -I../Core -I../Core/os -Ofast -ffunction-sections -mslow-flash-data -Wall -Wextra -Wfatal-errors -Wswitch-enum -fstack-usage -MMD -MP -MF"Core/kernel/filesys/FatFs/ff.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/kernel/filesys/FatFs/ff_gen_drv.o: ../Core/kernel/filesys/FatFs/ff_gen_drv.c Core/kernel/filesys/FatFs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/kernel -I../Core/kernel/Drivers/ -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc -I../Core/kernel/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Core/kernel/Drivers/USB -I../Core/kernel/Drivers/STM32_USB_Device_Library/Core/Inc -I../Core/kernel/Drivers/STM32_USB_Device_Library/Class/MSC/Inc -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Core/kernel/Drivers/CMSIS/Include -I../Core/kernel/filesys -I../Core/kernel/display -I../Core/kernel/uart -I../Core/resources -I../Core/kernel/filesys/FatFs/ -I../Core/libs -I../Core -I../Core/os -Ofast -ffunction-sections -mslow-flash-data -Wall -Wextra -Wfatal-errors -Wswitch-enum -fstack-usage -MMD -MP -MF"Core/kernel/filesys/FatFs/ff_gen_drv.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

