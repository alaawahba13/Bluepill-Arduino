/*
 * GPIO.h
 *
 *  Created on: Jan 15, 2023
 *      Author: Alaa Wahba
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

//=======================
//Includes
//=======================
#include "STM32_F103x6.h"
#include "RCC.h"
//====================================
//User type definitions (structures)
//====================================
typedef struct{
	uint16 Pin_Number    ;   // Specifies the GPIO pin Number to be configured
							//	This parameter must be a value of @ref GPIO_PINS_define
	uint8 MODE 			;	// Specifies the selected pin operating MODE
							//	This parameter must be a value of @ref GPIO_MODE_define
	uint8 Output_Speed  ;   // Specifies the pin speed
							//	This parameter must be a value of @ref GPIO_SPEED_define

}GPIO_PinConfig_t;


//===============================================
//Macros Configuration References
//===============================================
// @ref GPIO_PINS_define
#define PIN_0						((uint16)0x0001)
#define PIN_1						((uint16)0x0002)
#define PIN_2						((uint16)0x0004)
#define PIN_3						((uint16)0x0008)
#define PIN_4						((uint16)0x0010)
#define PIN_5						((uint16)0x0020)
#define PIN_6						((uint16)0x0040)
#define PIN_7						((uint16)0x0080)
#define PIN_8						((uint16)0x0100)
#define PIN_9						((uint16)0x0200)
#define PIN_10						((uint16)0x0400)
#define PIN_11						((uint16)0x0800)
#define PIN_12						((uint16)0x1000)
#define PIN_13						((uint16)0x2000)
#define PIN_14						((uint16)0x4000)
#define PIN_15						((uint16)0x8000)
#define ALL_PINS					((uint16)0xFFFF)

/*
 * @ref GPIO_MODE_define
 * 0: Analog mode
 * 1: Floating input (reset state)
 * 2: Input with pull-up
 * 3: pull-down
 * 4: General purpose output push-pull
 * 5: General purpose output Open-drain
 * 6: Alternate function output Push-pull
 * 7: Alternate function output Open-drain
 * 8: Alternate function input
*/
#define MODE_Analog								0x00
#define MODE_INPUT_FLO							0x01
#define MODE_INPUT_PU							0x02
#define MODE_INPUT_PD							0x03
#define MODE_OUTPUT_PP							0x04
#define MODE_OUTPUT_OD							0x05
#define MODE_OUTPUT_AF_PP						0x06
#define MODE_OUTPUT_AF_OD						0x07
#define MODE_INPUT_AF							0x10



/*
 * @ref GPIO_SPEED_define
 *	0: Output mode, max speed 10 MHz.
 *	1: Output mode, max speed 2 MHz.
 *	2: Output mode, max speed 50 MHz.
*/
#define SPEED_10M								0x01
#define SPEED_2M								0x02
#define SPEED_50M								0x03

/*
 * @ref GPIO_PIN_VALUE
 */
#define PIN_HIGH  							1
#define PIN_LOW								0

/*
 * @ref GPIO_RET_STATE
 */
#define RET_OK 								1
#define RET_ERROR							0

/*
* ===============================================
* APIs Supported by "MCAL GPIO DRIVER"
* ===============================================
*/

void GPIO_init(GPIO_Registers_t* GPIOx , GPIO_PinConfig_t* pinConfig);
void GPIO_DeInit(GPIO_Registers_t* GPIOx);

uint8 GPIO_ReadPin(GPIO_Registers_t* GPIOx , uint16 pinNumber);
uint16 GPIO_ReadPort(GPIO_Registers_t* GPIOx );

void GPIO_WritePin(GPIO_Registers_t* GPIOx , uint16 pinNumber , uint8 value );
void GPIO_WritePort(GPIO_Registers_t* GPIOx , uint16 value );

void GPIO_TogglePin(GPIO_Registers_t* GPIOx , uint16 pinNumber );

uint8 GPIO_LockPin(GPIO_Registers_t* GPIOx , uint16 pinNumber );

#endif /* INC_GPIO_H_ */
