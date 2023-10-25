/*
 * GPIO.c
 *
 *  Created on: Jan 15, 2023
 *      Author: Alaa Wahba
 */

#include "inc/GPIO.h"

uint8 Get_CRLH_Position(uint16 pinNumber) {
	uint8 shift = 0;
	switch (pinNumber) {
	case PIN_0:
		shift = 0;
		break;
	case PIN_1:
		shift = 4;
		break;
	case PIN_2:
		shift = 8;
		break;
	case PIN_3:
		shift = 12;
		break;
	case PIN_4:
		shift = 16;
		break;
	case PIN_5:
		shift = 20;
		break;
	case PIN_6:
		shift = 24;
		break;
	case PIN_7:
		shift = 28;
		break;

	case PIN_8:
		shift = 0;
		break;
	case PIN_9:
		shift = 4;
		break;
	case PIN_10:
		shift = 8;
		break;
	case PIN_11:
		shift = 12;
		break;
	case PIN_12:
		shift = 16;
		break;
	case PIN_13:
		shift = 20;
		break;
	case PIN_14:
		shift = 24;
		break;
	case PIN_15:
		shift = 28;
		break;

	}
	return shift;
}

/**================================================================
 * @Fn				- GPIO_init
 * @brief 			- Initializes the GPIOx PINy according to the specified configuration
 * @param [in]	    - GPIOx : Select the port where the x can be (A..E depending on device used)
 * @param [in]	    - pinConfig : pointer to a structure that contains the pin configuration information.
 * @retval 			- none
 * Note				- Stm32F103C6 MCU has GPIO A,B,C,D,E Modules
 * 				     But LQFP48 Package has only GPIO A,B,PART of C/D exported as external PINS from the MCU
 */

void GPIO_init(GPIO_Registers_t *GPIOx, GPIO_PinConfig_t *pinConfig) {
	// CRL configures the pins from pin 0 to 7
	// CRH configures the pins from pin 8 to 15
	volatile uint32 *config_Register = NULL; //this pointer determines which register to use
	config_Register = (pinConfig->Pin_Number < PIN_8) ? &GPIOx->CRL : &GPIOx->CRH;

	// This returns the starting bit in register CRL/CRH for the specified pin.
	uint8 shift_bit = Get_CRLH_Position(pinConfig->Pin_Number);

	// First Clear the CNF[1:0] MODE[1:0]
	(*config_Register) &= ~(0xF << shift_bit);

	uint8 register_bits = 0;
	// if pin is OUTPUT
	if ((pinConfig->MODE == MODE_OUTPUT_AF_OD) || (pinConfig->MODE == MODE_OUTPUT_AF_PP) || (pinConfig->MODE == MODE_OUTPUT_PP)	|| (pinConfig->MODE == MODE_OUTPUT_OD)) {
		register_bits = ((((pinConfig->MODE - 4) << 2)	| (pinConfig->Output_Speed)) & 0x0f);

	}
	else
	{
		if ((pinConfig->MODE == MODE_INPUT_FLO) || (pinConfig->MODE == MODE_Analog))
		{
			register_bits = (((pinConfig->MODE << 2) | 0x00) & 0x0f);
		}
		else if (pinConfig->MODE == MODE_INPUT_AF)
		{
			// consider alternative input as input floating
			register_bits = (((MODE_INPUT_FLO << 2) | 0x00) & 0x0f);
		}
		else
		{
			register_bits = (((MODE_INPUT_PU << 2) | 0x00) & 0x0f);
			if (pinConfig->MODE == MODE_INPUT_PU)
			{
				// set ODR register 1 for input pull up
				GPIOx->ODR |= pinConfig->Pin_Number;
			}
			else
			{
				// set ODR register 0 for input pull down
				GPIOx->ODR &= ~(pinConfig->Pin_Number);
			}
		}

	}
	// write to CRL /CRH
	(*config_Register) |= (register_bits << shift_bit);
}
/**================================================================
 * @Fn				- GPIO_DeInit
 * @brief 			- reset all the GPIO registers
 * @param [in]	    - GPIOx : Select the port where the x can be (A..E depending on device used)
 * @retval 			- none
 */
void GPIO_DeInit(GPIO_Registers_t *GPIOx) {
	//Reset values can be determined from TRM
	// Fortunately STM32 has a reset Controller register from RCC
	if (GPIOx == GPIOA) {  //Set >>1  and then cleared >>0  by software
		RCC->APB2RSTR |= (1 << 2);
		RCC->APB2RSTR &= ~(1 << 2);
	} else if (GPIOx == GPIOB) {
		RCC->APB2RSTR |= (1 << 3);
		RCC->APB2RSTR &= ~(1 << 3);
	} else if (GPIOx == GPIOC) {
		RCC->APB2RSTR |= (1 << 4);
		RCC->APB2RSTR &= ~(1 << 4);
	} else if (GPIOx == GPIOD) {
		RCC->APB2RSTR |= (1 << 5);
		RCC->APB2RSTR &= ~(1 << 5);
	} else if (GPIOx == GPIOE) {
		RCC->APB2RSTR |= (1 << 6);
		RCC->APB2RSTR &= ~(1 << 6);
	}

}
/**================================================================
 * @Fn				- GPIO_ReadPin
 * @brief 			- Reads the value of PINy in PORTx
 * @param [in]	    - GPIOx : Select the port where the x can be (A..E depending on device used)
 * @param [out]	    - pinNumber : Must be a value from @ref GPIO_PINS_define
 * @retval 			- the input value based on @ref GPIO_PIN_VALUE
 */
uint8 GPIO_ReadPin(GPIO_Registers_t *GPIOx, uint16 pinNumber) {
	uint8 pin_status;
	if ((GPIOx->IDR & pinNumber) != (uint32) PIN_LOW) {
		pin_status = PIN_HIGH;
	} else {
		pin_status = PIN_LOW;
	}
	return pin_status;
}
/**================================================================
 * @Fn				- GPIO_ReadPort
 * @brief 			- Reads the value of PORTx
 * @param [in]	    - GPIOx : Select the port where the x can be (A..E depending on device used)
 * @retval 			- the input port value
 */
uint16 GPIO_ReadPort(GPIO_Registers_t *GPIOx) {
	uint16 portValue;
	portValue = (uint16) (GPIOx->IDR);
	return portValue;
}
/**================================================================
 * @Fn				- GPIO_WritePin
 * @brief 			- Writes to PINy in PORTx
 * @param [in]	    - GPIOx : Select the port where the x can be (A..E depending on device used)
 * @param [in]	    - pinNumber : Must be a value from @ref GPIO_PINS_define
 * @param [in]	    - value : either HIGH or LOW from @ref GPIO_PIN_VALUE
 * @retval 			- none
 */
void GPIO_WritePin(GPIO_Registers_t *GPIOx, uint16 pinNumber, uint8 value) {
	if (value != PIN_LOW) {
		GPIOx->ODR |= (uint32)(pinNumber);
	} else {
		GPIOx->ODR &= ~((uint32)(pinNumber));
	}
}
/**================================================================
 * @Fn				- GPIO_WritePort
 * @brief 			- Writes to PORTx
 * @param [in]	    - GPIOx : Select the port where the x can be (A..E depending on device used)
 * @param [in]	    - value : value to write on port.
 * @retval 			- none
 */
void GPIO_WritePort(GPIO_Registers_t *GPIOx, uint16 value) {

	GPIOx->ODR = (uint32) value;
}
/**================================================================
 * @Fn				- GPIO_TogglePin
 * @brief 			- Toggles the value of PINy in PORTx
 * @param [in]	    - GPIOx : Select the port where the x can be (A..E depending on device used)
 * @param [in]	    - pinNumber : Must be a value from @ref GPIO_PINS_define
 * @retval 			- none
 */
void GPIO_TogglePin(GPIO_Registers_t *GPIOx, uint16 pinNumber) {
	GPIOx->ODR ^= (pinNumber);
}
/**================================================================
 * @Fn				- GPIO_LockPin
 * @brief 			- The locking mechanism allows the IO configuration to be frozen
 * @param [in]	    - GPIOx : Select the port where the x can be (A..E depending on device used)
 * @param [in]	    - pinNumber : Must be a value from @ref GPIO_PINS_define
 * @retval 			- Return the state of locking mechanism @ref GPIO_RET_STATE
 */
uint8 GPIO_LockPin(GPIO_Registers_t *GPIOx, uint16 pinNumber) {
	/*
	 * Bit 16 LCKK[16]: Lock key
	 This bit can be read anytime. It can only be modified using the Lock Key Writing Sequence.
	 0: Port configuration lock key not active
	 1: Port configuration lock key active. GPIOx_LCKR register is locked until the next reset.
	 LOCK key writing sequence:
	 Write 1
	 Write 0
	 Write 1
	 Read 0
	 Read 1 (this read is optional but confirms that the lock is active)
	 Note: During the LOCK Key Writing sequence, the value of LCK[15:0] must not change.
	 Any error in the lock sequence will abort the lock.
	 Bits 15:0 LCKy: Port x Lock bit y (y= 0 .. 15)
	 These bits are read write but can only be written when the LCKK bit is 0.
	 0: Port configuration not locked
	 1: Port configuration locked
	 */

	volatile uint32_t tmp = 1<<16 ;
	//Set the LCKy
	tmp |= pinNumber ;

	//	Write 1
	GPIOx->LCKR = tmp ;
	//	Write 0
	GPIOx->LCKR = pinNumber ;
	//	Write 1
	GPIOx->LCKR = tmp ;

	//	Read 0
	tmp = GPIOx->LCKR  ;
	//	Read 1 (this read is optional but confirms that the lock is active)
	if ( (uint32_t) (GPIOx->LCKR  & 1<<16 ))
	{
		return RET_OK ;
	}else
	{
		return RET_ERROR ;

	}


return 0;
}
