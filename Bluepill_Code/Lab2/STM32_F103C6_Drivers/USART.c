/*
 * USART.c
 *
 *  Created on: 16 Feb 2023
 *      Author: Alaa Wahba
 */

//=======================
//       Includes
//=======================
#include "inc/USART.h"

/*
 * =======================================================================================
 * 							Generic Variables
 * =======================================================================================
 */
USART_pinConfig_t Global_pinConfig[3] = {NULL,NULL,NULL};

#define USART1_Index 			0
#define USART2_Index 			1
#define USART3_Index 			2

/*
 * =======================================================================================
 * 							Generic Functions
 * =======================================================================================
 */

/**================================================================
 * @Fn				-USART_init
 * @brief 			- Initializes UART (Supported feature ASYNCH. Only)
 * @param [in] 		- USARTx: where x can be (1..3 depending on device used)
 * @param [in] 		- pinConfig: All UART Configuration USART_pinConfig_t
 * @retval 			-none
 * Note				-Support for Now Asynch mode & Clock 8 MHZ S
 */
void USART_init(USART_pinConfig_t *pinConfig, USART_Registers_t *USARTx) {
	uint32 BRR, pclk;

	/*            Enable USART clocks      */
	if (USARTx == USART1) {
		Global_pinConfig[USART1_Index] = *pinConfig;
		RCC_CLK_EN(APB2_ID,USART1_ID);
	}
	if (USARTx == USART2) {
		Global_pinConfig[USART2_Index] = *pinConfig;
		RCC_CLK_EN(APB1_ID,USART2_ID);
	} else if (USARTx == USART3) {
		Global_pinConfig[USART3_Index] = *pinConfig;
		RCC_CLK_EN(APB1_ID,USART3_ID);
	}

	/*            Enable USART Module    	  */
	//Bit 13 UE: USART enable
	USARTx->CR1 |= (1 << 13);
	/*            Enable TX/RX  		      */
	USARTx->CR1 |= pinConfig->USART_Mode;
	/*            Define The StopBits         */
	USARTx->CR2 |= pinConfig->StopBits;
	/*            Define The DataBits         */
	USARTx->CR1 |= pinConfig->DataLength;
	/*            Configure The Parity        */
	USARTx->CR1 |= pinConfig->Parity;
	/*            Hardware Flow Control       */
	USARTx->CR3 |= pinConfig->FlowControl;
	/*            Define The BaudRate	      */
//FCLK - Input clock to the peripheral (PCLK1 for USART2, 3 or PCLK2 for USART1)
	if (USARTx == USART1) {
		pclk = RCC_getPCKL2_Freq();
	} else {
		pclk = RCC_getPCKL1_Freq();
	}

	BRR = USART_BRR(pclk, pinConfig->BaudRate);
	USARTx->BRR = BRR;

	/*            Enabling The Interrupt      */
	if (pinConfig->IRQ_Enable != USART_NONE) {
		USARTx->CR1 |= (pinConfig->IRQ_Enable);
		//		Enable NVIC For USARTx IRQ
		if (USARTx == USART1) {
			NVIC_Enable(USART1_LineNumber);
		}else if (USARTx == USART2) {
			NVIC_Enable(USART2_LineNumber);
		} else if (USARTx == USART3) {
			NVIC_Enable(USART3_LineNumber);
		}
	}
}

/**================================================================
 * @Fn				-USART_DeInit
 * @brief 			- Resets UART
 * @param [in] 		- USARTx: where x can be (1..3 depending on device used)
 * @retval 			-none
 */
void USART_DeInit(USART_Registers_t *USARTx) {

	if (USARTx == USART1) {
		RCC_CLK_RST(APB2_ID,USART1_ID);
		NVIC_Disable(USART1_LineNumber);
	}
	if (USARTx == USART2) {
		RCC_CLK_RST(APB1_ID,USART2_ID);
		NVIC_Disable(USART2_LineNumber);
	} else if (USARTx == USART3) {
		RCC_CLK_RST(APB1_ID,USART3_ID);
		NVIC_Disable(USART3_LineNumber);
	}

}

/**================================================================
 * @Fn				-USART_SetPins
 * @brief 			-initialize GPIO Pins
 * @param [in] 		-USARTx: where x can be (1..3 depending on device used)
 * @retval 			-none
 * @Note			-Should enable the corresponding ALT  & GPIO  in RCC clock Also called after USART_Init()
 */
void USART_SetPins(USART_Registers_t *USARTx) {
	GPIO_PinConfig_t GPIO_pinConfig;
	if (USARTx == USART1) {
		// configure  TX PORTA PIN9 As Alternate function Push pull
		GPIO_pinConfig.MODE = MODE_OUTPUT_AF_PP;
		GPIO_pinConfig.Output_Speed = SPEED_10M;
		GPIO_pinConfig.Pin_Number = PIN_9;
		GPIO_init(GPIOA, &GPIO_pinConfig);
		// configure  RX PORTA PIN10 As  Alternate function INPUT
		GPIO_pinConfig.MODE = MODE_INPUT_AF;
		GPIO_pinConfig.Pin_Number = PIN_10;
		GPIO_init(GPIOA, &GPIO_pinConfig);
		// Configure CTS /RTS pins
		if (Global_pinConfig[USART1_Index].FlowControl == USART_FlowControl_CTS
				|| Global_pinConfig[USART1_Index].FlowControl == USART_FlowControl_BOTH) {
			// CTS pin PORTA pin 11 As   Alternate function INPUT
			GPIO_pinConfig.MODE = MODE_INPUT_FLO;
			GPIO_pinConfig.Pin_Number = PIN_11;
			GPIO_init(GPIOA, &GPIO_pinConfig);
		} else if (Global_pinConfig[USART1_Index].FlowControl == USART_FlowControl_RTS
				|| Global_pinConfig[USART1_Index].FlowControl == USART_FlowControl_BOTH) {
			// RTS pin PORTA pin 12 As   Alternate function  Push pull
			GPIO_pinConfig.MODE = MODE_OUTPUT_AF_PP;
			GPIO_pinConfig.Output_Speed = SPEED_10M;
			GPIO_pinConfig.Pin_Number = PIN_12;
			GPIO_init(GPIOA, &GPIO_pinConfig);
		}

	} else if (USARTx == USART2) {
		// configure  TX PORTA PIN2 As Alternate function Push pull
		GPIO_pinConfig.MODE = MODE_OUTPUT_AF_PP;
		GPIO_pinConfig.Output_Speed = SPEED_10M;
		GPIO_pinConfig.Pin_Number = PIN_2;
		GPIO_init(GPIOA, &GPIO_pinConfig);
		// configure  RX PORTA PIN3 As  Alternate function INPUT
		GPIO_pinConfig.MODE = MODE_INPUT_AF;
		GPIO_pinConfig.Pin_Number = PIN_3;
		GPIO_init(GPIOA, &GPIO_pinConfig);
		// Configure CTS /RTS pins
		if (Global_pinConfig[USART2_Index].FlowControl == USART_FlowControl_CTS|| Global_pinConfig[USART2_Index].FlowControl == USART_FlowControl_BOTH) {
			// CTS pin PORTA pin 0 As   Alternate function INPUT
			GPIO_pinConfig.MODE = MODE_INPUT_FLO;
			GPIO_pinConfig.Pin_Number = PIN_0;
			GPIO_init(GPIOA, &GPIO_pinConfig);
		} else if (Global_pinConfig[USART2_Index].FlowControl == USART_FlowControl_RTS|| Global_pinConfig[USART2_Index].FlowControl == USART_FlowControl_BOTH) {
			// RTS pin PORTA pin 1 As   Alternate function  Push pull
			GPIO_pinConfig.MODE = MODE_OUTPUT_AF_PP;
			GPIO_pinConfig.Output_Speed = SPEED_10M;
			GPIO_pinConfig.Pin_Number = PIN_1;
			GPIO_init(GPIOA, &GPIO_pinConfig);
		}

	} else if (USARTx == USART3) {
		// configure  TX PORTB PIN10 As Alternate function Push pull
		GPIO_pinConfig.MODE = MODE_OUTPUT_AF_PP;
		GPIO_pinConfig.Output_Speed = SPEED_10M;
		GPIO_pinConfig.Pin_Number = PIN_10;
		GPIO_init(GPIOB, &GPIO_pinConfig);
		// configure  RX PORTB PIN11 As  Alternate function INPUT
		GPIO_pinConfig.MODE = MODE_INPUT_AF;
		GPIO_pinConfig.Pin_Number = PIN_11;
		GPIO_init(GPIOB, &GPIO_pinConfig);
		// Configure CTS /RTS pins
		if (Global_pinConfig[USART3_Index].FlowControl == USART_FlowControl_CTS|| Global_pinConfig[USART3_Index].FlowControl == USART_FlowControl_BOTH) {
			// CTS pin PORTB pin 13 As   Alternate function INPUT
			GPIO_pinConfig.MODE = MODE_INPUT_AF;
			GPIO_pinConfig.Pin_Number = PIN_13;
			GPIO_init(GPIOB, &GPIO_pinConfig);
		} else if (Global_pinConfig[USART3_Index].FlowControl == USART_FlowControl_RTS || Global_pinConfig[USART3_Index].FlowControl == USART_FlowControl_BOTH) {
			// RTS pin PORTB pin 14 As   Alternate function  Push pull
			GPIO_pinConfig.MODE = MODE_OUTPUT_AF_PP;
			GPIO_pinConfig.Output_Speed = SPEED_10M;
			GPIO_pinConfig.Pin_Number = PIN_14;
			GPIO_init(GPIOB, &GPIO_pinConfig);
		}

	}

}

/**================================================================
 * @Fn				- USART_Send
 * @brief 			- Sends the data
 * @param [in] 		- USARTx: where x can be (1..3 depending on device used)
 * @param [in] 		- pTxBuffer: Data to be sent
 * @param [in] 		- PollingEn: Enables or disables the polling mechanism
 * @retval 			- none
 * @Note              - Should initialize UART First
 * 						When transmitting with the parity enabled (PCE bit set to 1 in the USART_CR1 register),
 the value written in the MSB (bit 7 or bit 8 depending on the data length) has no effect
 because it is replaced by the parity.
 When receiving with the parity enabled, the value read in the MSB bit is the received parity
 bit
 */

void USART_Send(USART_Registers_t *USARTx, uint16 *pTxBuffer,
		enum Polling_Mechanism PollingEn) {
	uint8 index = 0;
	index = (USARTx == USART1) ? USART1_Index:((USARTx ==USART2) ? USART2_Index : USART3_Index);
//Write the data to send in the USART_DR register (this clears the TXE bit).
	if (PollingEn == Enable)
		while (!(USARTx->SR & (1 << 7)))
			;	//TXE bit is 1 means Data is completely sent.

	if (Global_pinConfig[index].DataLength == USART_DataLength9) {
		USARTx->DR = (*pTxBuffer & (uint16)0x01FF);
	} else {
		USARTx->DR = (*pTxBuffer & (uint8)0xFF);
	}
}

/**================================================================
 * @Fn				- USART_Recieve
 * @brief 			- Recieves the data
 * @param [in] 		- USARTx: where x can be (1..3 depending on device used)
 * @param [in] 		- pTxBuffer:
 * @param [in] 		- PollingEn: Enables or disables the polling mechanism
 * @retval 			- none
 */
void USART_Recieve(USART_Registers_t *USARTx, uint16 *pTxBuffer,
		enum Polling_Mechanism PollingEn) {
	uint8 index = 0;
		index = (USARTx == USART1) ? USART1_Index:((USARTx ==USART2) ? USART2_Index : USART3_Index);
	/*
	 * Bit 5 RXNE: Read data register not empty
	 * This bit is set by hardware when the content of the RDR shift register has been transferred to
	 * the USART_DR register.
	 */
	if (PollingEn == Enable)
		while (!(USARTx->SR & (1 << 5)))
			;
	//Check The Data length

	if (Global_pinConfig[index].DataLength == USART_DataLength9) {
		if (Global_pinConfig[index].Parity == USART_Parity_None) {
			// case No parity
				*((uint16 *)pTxBuffer)= USARTx->DR;

		} else {
			// If parity is enabled  then ignore the MSB
			*((uint16 *)pTxBuffer) = (USARTx->DR & (uint8)0xFF);
		}

	} else {
		if (Global_pinConfig[index].Parity == USART_Parity_None) {
			// case No parity
			*((uint16 *)pTxBuffer) = (USARTx->DR & (uint8)0xFF);

		} else {

			// If parity is enabled  then ignore the MSB
			*((uint16 *)pTxBuffer) = (USARTx->DR & (uint8)0x7F);
		}
	}
}

/**================================================================
 * @Fn				-USART_Wait_TC
 * @brief 			- Wait until The TC flag is Set
 * @param [in] 		- USARTx: where x can be (1..3 depending on device used)
 * @retval 			-none
 */
void USART_Wait_TC(USART_Registers_t *USARTx) {
	/*
	 * This bit is set by hardware if the transmission of a frame containing data is complete and if
	 * TXE is set.
	 * 0: Transmission is not complete
	 * 1: Transmission is complete
	 */
	while (!(USARTx->SR & (1 << 6)));
}

// ISR
void USART1_IRQHandler(){
	Global_pinConfig[USART1_Index].P_CallBack_Fun();
}
void USART2_IRQHandler(){
	Global_pinConfig[USART2_Index].P_CallBack_Fun();
}
void USART3_IRQHandler(){
	Global_pinConfig[USART3_Index].P_CallBack_Fun();
}
