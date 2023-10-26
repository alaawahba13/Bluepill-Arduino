/*
 * NVIC.c
 *
 *  Created on: 20 Aug 2023
 *      Author: Qebaa
 */

#ifndef NVIC_C_
#define NVIC_C_

//=======================
//       Includes
//=======================
#include "inc/NVIC.h"

/**================================================================
 * @Fn				- NVIC_Enable
 * @brief 			- Enables the NVIC interrupt pin
 * @param [in]	    - IRQ_Number :  The specified Interrupt Number to be enabled
 * @retval 			- none
 */

void NVIC_Enable(uint8 IRQ_Number) {
	if ((IRQ_Number >= 0) && (IRQ_Number <= 31)) {
	 NVIC_ISER->ISER0 |= (1 << IRQ_Number);
	 } else if ((IRQ_Number >= 32) && (IRQ_Number <= 63)) {
	 NVIC_ISER->ISER1 |= (1 << (IRQ_Number-32));
	 } else if ((IRQ_Number >= 64) && (IRQ_Number <= 67)) {
	 NVIC_ISER->ISER2 |= (1 << (IRQ_Number-32));
	 }
}
/**================================================================
 * @Fn				- NVIC_Disable
 * @brief 			- Disables the NVIC interrupt pin
 * @param [in]	    - IRQ_Number :  The specified Interrupt Number to be disabled
 * @retval 			- none
 */

void NVIC_Disable(uint8 IRQ_Number) {

	if ((IRQ_Number >= 0) && (IRQ_Number <= 31)) {
	 NVIC_ICER->ICER0 |= (1 << IRQ_Number);
	 } else if ((IRQ_Number >= 32) && (IRQ_Number <= 63)) {
	 NVIC_ICER->ICER1 |= (1 << (IRQ_Number-32));
	 } else if ((IRQ_Number >= 64) && (IRQ_Number <= 67)) {
	 NVIC_ICER->ICER2 |= (1 << (IRQ_Number-32));
	 }

}


#endif /* NVIC_C_ */
