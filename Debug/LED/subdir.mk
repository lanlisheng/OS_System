################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LED/led.c 

OBJS += \
./LED/led.o 

C_DEPS += \
./LED/led.d 


# Each subdirectory must supply rules for building sources it contributes
LED/%.o LED/%.su: ../LED/%.c LED/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/OptimusPrime/Desktop/OS_System/OS" -I"C:/Users/OptimusPrime/Desktop/OS_System/LED" -I"C:/Users/OptimusPrime/Desktop/OS_System/CPU" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-LED

clean-LED:
	-$(RM) ./LED/led.d ./LED/led.o ./LED/led.su

.PHONY: clean-LED

