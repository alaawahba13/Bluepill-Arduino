################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32_F103C6_Drivers/GPIO.c \
../STM32_F103C6_Drivers/NVIC.c \
../STM32_F103C6_Drivers/RCC.c \
../STM32_F103C6_Drivers/SPI.c \
../STM32_F103C6_Drivers/USART.c 

OBJS += \
./STM32_F103C6_Drivers/GPIO.o \
./STM32_F103C6_Drivers/NVIC.o \
./STM32_F103C6_Drivers/RCC.o \
./STM32_F103C6_Drivers/SPI.o \
./STM32_F103C6_Drivers/USART.o 

C_DEPS += \
./STM32_F103C6_Drivers/GPIO.d \
./STM32_F103C6_Drivers/NVIC.d \
./STM32_F103C6_Drivers/RCC.d \
./STM32_F103C6_Drivers/SPI.d \
./STM32_F103C6_Drivers/USART.d 


# Each subdirectory must supply rules for building sources it contributes
STM32_F103C6_Drivers/GPIO.o: ../STM32_F103C6_Drivers/GPIO.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32_F103C6_Drivers/GPIO.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
STM32_F103C6_Drivers/NVIC.o: ../STM32_F103C6_Drivers/NVIC.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32_F103C6_Drivers/NVIC.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
STM32_F103C6_Drivers/RCC.o: ../STM32_F103C6_Drivers/RCC.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32_F103C6_Drivers/RCC.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
STM32_F103C6_Drivers/SPI.o: ../STM32_F103C6_Drivers/SPI.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32_F103C6_Drivers/SPI.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
STM32_F103C6_Drivers/USART.o: ../STM32_F103C6_Drivers/USART.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32_F103C6_Drivers/USART.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

