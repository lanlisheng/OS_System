################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../KEY/key.c 

OBJS += \
./KEY/key.o 

C_DEPS += \
./KEY/key.d 


# Each subdirectory must supply rules for building sources it contributes
KEY/%.o KEY/%.su: ../KEY/%.c KEY/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/OptimusPrime/Desktop/OS_System/TIME" -I"C:/Users/OptimusPrime/Desktop/OS_System/OS" -I"C:/Users/OptimusPrime/Desktop/OS_System/LED" -I"C:/Users/OptimusPrime/Desktop/OS_System/KEY" -I"C:/Users/OptimusPrime/Desktop/OS_System/CPU" -I"C:/Users/OptimusPrime/Desktop/OS_System/USER" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-KEY

clean-KEY:
	-$(RM) ./KEY/key.d ./KEY/key.o ./KEY/key.su

.PHONY: clean-KEY

