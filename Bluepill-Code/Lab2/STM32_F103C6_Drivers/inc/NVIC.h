/*
 * NVIC.h
 *
 *  Created on: 20 Aug 2023
 *      Author: Qebaa
 */

#ifndef INC_NVIC_H_
#define INC_NVIC_H_

//=======================
//       Includes
//=======================
#include "STM32_F103x6.h"

/*
* ===============================================
*      APIs Supported by "MCAL NVIC DRIVER"
* ===============================================
*/
void NVIC_Enable(uint8 IRQ_Number);
void NVIC_Disable(uint8 IRQ_Number);

#endif /* INC_NVIC_H_ */
