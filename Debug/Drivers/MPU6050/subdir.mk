################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/MPU6050/MPU6050.c 

OBJS += \
./Drivers/MPU6050/MPU6050.o 

C_DEPS += \
./Drivers/MPU6050/MPU6050.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/MPU6050/%.o Drivers/MPU6050/%.su Drivers/MPU6050/%.cyclo: ../Drivers/MPU6050/%.c Drivers/MPU6050/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32H533xx -c -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32H5xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I"/Users/Rosso/Documents/e-design/code/Drivers/ssd1306" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-MPU6050

clean-Drivers-2f-MPU6050:
	-$(RM) ./Drivers/MPU6050/MPU6050.cyclo ./Drivers/MPU6050/MPU6050.d ./Drivers/MPU6050/MPU6050.o ./Drivers/MPU6050/MPU6050.su

.PHONY: clean-Drivers-2f-MPU6050

