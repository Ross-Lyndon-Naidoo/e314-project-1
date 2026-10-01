################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BUTTON/button.c 

OBJS += \
./Drivers/BUTTON/button.o 

C_DEPS += \
./Drivers/BUTTON/button.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BUTTON/%.o Drivers/BUTTON/%.su Drivers/BUTTON/%.cyclo: ../Drivers/BUTTON/%.c Drivers/BUTTON/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32H533xx -c -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32H5xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I"/Users/Rosso/Documents/e-design/code/Drivers/ssd1306" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BUTTON

clean-Drivers-2f-BUTTON:
	-$(RM) ./Drivers/BUTTON/button.cyclo ./Drivers/BUTTON/button.d ./Drivers/BUTTON/button.o ./Drivers/BUTTON/button.su

.PHONY: clean-Drivers-2f-BUTTON

