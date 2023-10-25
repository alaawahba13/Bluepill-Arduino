/*
 * RCC.h
 *
 *  Created on: 16 Feb 2023
 *      Author: Alaa Wahba
 */

#ifndef INC_RCC_H_
#define INC_RCC_H_
//=======================
//       Includes
//=======================
#include "STM32_F103x6.h"

//===============================================
//Macros Configuration References
//===============================================

#define HSI 		(uint32)8000000
#define HSE 		(uint32)16000000

// @ref BUS_ID_define
#define APB1_ID 		0
#define APB2_ID 		1
#define AHB_ID 			2

// @ref Periphral_ID_define
#define GPIOA_ID		2
#define GPIOB_ID		3
#define GPIOC_ID		4
#define GPIOD_ID		5
#define GPIOE_ID		6

#define AFIO_ID			0

#define USART1_ID		14
#define USART2_ID		17
#define USART3_ID		18

#define SPI1_ID			12
#define SPI2_ID			14

#define I2C1_ID			21
#define I2C2_ID			22



/*
* ===============================================
*      APIs Supported by "MCAL RCC DRIVER"
* ===============================================
*/
uint32 RCC_getSYSCLK_Freq(void);

uint32 RCC_getPCKL1_Freq(void);
uint32 RCC_getPCKL2_Freq(void);
uint32 RCC_getHCKL_Freq(void);

void RCC_CLK_RST(uint8 busID , uint8 periphralID);
void RCC_CLK_EN(uint8 busID , uint8 periphralID);

#endif /* INC_RCC_H_ */
