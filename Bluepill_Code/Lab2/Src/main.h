/*
 * main.h
 *
 *  Created on: Jan 26, 2023
 *      Author: Alaa Wahba
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "../STM32_F103C6_Drivers/inc/GPIO.h"
#include "../STM32_F103C6_Drivers/inc/STM32_F103x6.h"
#include "../HAL/inc/LCD.h"

#include "../STM32_F103C6_Drivers/inc/USART.h"
#include "../STM32_F103C6_Drivers/inc/RCC.h"
#include "../STM32_F103C6_Drivers/inc/SPI.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define Green_LED		PIN_11
#define Red_LED			PIN_12

#define LED_GPIO		GPIOA


#endif /* INC_MAIN_H_ */
