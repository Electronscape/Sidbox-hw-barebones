################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/kernel/filesys/bsp_driver_sd.c \
../Core/kernel/filesys/fatfs_platform.c \
../Core/kernel/filesys/filesys.c \
../Core/kernel/filesys/sd_diskio.c \
../Core/kernel/filesys/sdmmc.c 

OBJS += \
./Core/kernel/filesys/bsp_driver_sd.o \
./Core/kernel/filesys/fatfs_platform.o \
./Core/kernel/filesys/filesys.o \
./Core/kernel/filesys/sd_diskio.o \
./Core/kernel/filesys/sdmmc.o 

C_DEPS += \
./Core/kernel/filesys/bsp_driver_sd.d \
./Core/kernel/filesys/fatfs_platform.d \
./Core/kernel/filesys/filesys.d \
./Core/kernel/filesys/sd_diskio.d \
./Core/kernel/filesys/sdmmc.d 


# Each subdirectory must supply rules for building sources it contributes
Core/kernel/filesys/bsp_driver_sd.o: ../Core/kernel/filesys/bsp_driver_sd.c Core/kernel/filesys/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/kernel -I../Core/kernel/Drivers/ -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc -I../Core/kernel/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Core/kernel/Drivers/USB -I../Core/kernel/Drivers/STM32_USB_Device_Library/Core/Inc -I../Core/kernel/Drivers/STM32_USB_Device_Library/Class/MSC/Inc -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Core/kernel/Drivers/CMSIS/Include -I../Core/kernel/filesys -I../Core/kernel/display -I../Core/kernel/uart -I../Core/resources -I../Core/kernel/filesys/FatFs/ -I../Core/libs -I../Core -I../Core/os -Ofast -ffunction-sections -mslow-flash-data -Wall -Wextra -Wfatal-errors -Wswitch-enum -fstack-usage -MMD -MP -MF"Core/kernel/filesys/bsp_driver_sd.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/kernel/filesys/fatfs_platform.o: ../Core/kernel/filesys/fatfs_platform.c Core/kernel/filesys/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/kernel -I../Core/kernel/Drivers/ -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc -I../Core/kernel/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Core/kernel/Drivers/USB -I../Core/kernel/Drivers/STM32_USB_Device_Library/Core/Inc -I../Core/kernel/Drivers/STM32_USB_Device_Library/Class/MSC/Inc -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Core/kernel/Drivers/CMSIS/Include -I../Core/kernel/filesys -I../Core/kernel/display -I../Core/kernel/uart -I../Core/resources -I../Core/kernel/filesys/FatFs/ -I../Core/libs -I../Core -I../Core/os -Ofast -ffunction-sections -mslow-flash-data -Wall -Wextra -Wfatal-errors -Wswitch-enum -fstack-usage -MMD -MP -MF"Core/kernel/filesys/fatfs_platform.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/kernel/filesys/filesys.o: ../Core/kernel/filesys/filesys.c Core/kernel/filesys/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/kernel -I../Core/kernel/Drivers/ -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc -I../Core/kernel/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Core/kernel/Drivers/USB -I../Core/kernel/Drivers/STM32_USB_Device_Library/Core/Inc -I../Core/kernel/Drivers/STM32_USB_Device_Library/Class/MSC/Inc -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Core/kernel/Drivers/CMSIS/Include -I../Core/kernel/filesys -I../Core/kernel/display -I../Core/kernel/uart -I../Core/resources -I../Core/kernel/filesys/FatFs/ -I../Core/libs -I../Core -I../Core/os -Ofast -ffunction-sections -mslow-flash-data -Wall -Wextra -Wfatal-errors -Wswitch-enum -fstack-usage -MMD -MP -MF"Core/kernel/filesys/filesys.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/kernel/filesys/sd_diskio.o: ../Core/kernel/filesys/sd_diskio.c Core/kernel/filesys/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/kernel -I../Core/kernel/Drivers/ -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc -I../Core/kernel/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Core/kernel/Drivers/USB -I../Core/kernel/Drivers/STM32_USB_Device_Library/Core/Inc -I../Core/kernel/Drivers/STM32_USB_Device_Library/Class/MSC/Inc -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Core/kernel/Drivers/CMSIS/Include -I../Core/kernel/filesys -I../Core/kernel/display -I../Core/kernel/uart -I../Core/resources -I../Core/kernel/filesys/FatFs/ -I../Core/libs -I../Core -I../Core/os -Ofast -ffunction-sections -mslow-flash-data -Wall -Wextra -Wfatal-errors -Wswitch-enum -fstack-usage -MMD -MP -MF"Core/kernel/filesys/sd_diskio.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/kernel/filesys/sdmmc.o: ../Core/kernel/filesys/sdmmc.c Core/kernel/filesys/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/kernel -I../Core/kernel/Drivers/ -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc -I../Core/kernel/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Core/kernel/Drivers/USB -I../Core/kernel/Drivers/STM32_USB_Device_Library/Core/Inc -I../Core/kernel/Drivers/STM32_USB_Device_Library/Class/MSC/Inc -I../Core/kernel/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Core/kernel/Drivers/CMSIS/Include -I../Core/kernel/filesys -I../Core/kernel/display -I../Core/kernel/uart -I../Core/resources -I../Core/kernel/filesys/FatFs/ -I../Core/libs -I../Core -I../Core/os -Ofast -ffunction-sections -mslow-flash-data -Wall -Wextra -Wfatal-errors -Wswitch-enum -fstack-usage -MMD -MP -MF"Core/kernel/filesys/sdmmc.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

