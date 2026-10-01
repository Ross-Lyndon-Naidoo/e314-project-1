################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/NEO_6M/gps.c 

OBJS += \
./Drivers/NEO_6M/gps.o 

C_DEPS += \
./Drivers/NEO_6M/gps.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/NEO_6M/%.o Drivers/NEO_6M/%.su Drivers/NEO_6M/%.cyclo: ../Drivers/NEO_6M/%.c Drivers/NEO_6M/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32H533xx -c -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32H5xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I"/Users/Rosso/Documents/e-design/code/Drivers/ssd1306" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-NEO_6M

clean-Drivers-2f-NEO_6M:
	-$(RM) ./Drivers/NEO_6M/gps.cyclo ./Drivers/NEO_6M/gps.d ./Drivers/NEO_6M/gps.o ./Drivers/NEO_6M/gps.su

.PHONY: clean-Drivers-2f-NEO_6M

