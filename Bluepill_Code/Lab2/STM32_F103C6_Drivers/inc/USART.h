/*
 * USART.h
 *
 *  Created on: 16 Feb 2023
 *      Author: Alaa Wahba
 */

#ifndef INC_USART_H_
#define INC_USART_H_
//=======================
//       Includes
//=======================

#include "GPIO.h"
#include "RCC.h"
#include "NVIC.h"

//================================================
//   User type definitions (structures)
//================================================
typedef struct{
	// This parameter must be set based on @ref USART_Mode_define
	uint8 USART_Mode;			  			   	 // Specifies the TX/RX Mode.
	// This parameter must be set based on @ref USART_BaudRate_define
	uint32 BaudRate;							  // Specifies the UART communication BaudRate
	//  This parameter must be set based on @ref USART_StopBits_define
	uint8 StopBits;								 // Specifies The number of stop bits in the communication frame.
	//  This parameter must be set based on @ref USART_DataLength_define
	uint8 DataLength;							 // Specifies The number of data bits in the communication frame.
	// This parameter must be set based on @ref USART_Parity_define
	uint8 Parity; 								// Determines the Parity type
	// This parameter must be set based on @ref USART_FlowControl_define
	uint8 FlowControl;  						 //Specifies whether the hardware flow control mode is enabled or disabled
	// This parameter must be set based on @ref USART_IRQ_Enable_define
	uint8 IRQ_Enable;  							 // Specifies Interrupt 	Enable / Mask pin

	void(*P_CallBack_Fun)(void);  // The C function() that is called when IRQ is enabled.


}USART_pinConfig_t;

enum Polling_Mechanism{
	Enable,
	Disable
};
//===============================================
//      Macros Configuration References
//===============================================

/*
 * @ref USART_Mode_define
 */
#define USART_TXEN 					(1<<3)			//TE :This bit enables the transmitter.
#define USART_RXEN					(1<<2)			//RE  = 1
#define USART_TXRXEN				(1<<2 | 1<<3)

/*
 * @ref USART_BaudRate_define
 */
#define USART_BaudRate_2400						2400
#define USART_BaudRate_9600						9600
#define USART_BaudRate_19200					19200
#define USART_BaudRate_57600					57600
#define USART_BaudRate_115200					115200
#define USART_BaudRate_230400					230400
#define USART_BaudRate_460800					460800
#define USART_BaudRate_921600					921600
#define USART_BaudRate_2250000					2250000
#define USART_BaudRate_4500000					4500000


/*
 *  @ref USART_StopBits_define
 */
/*
 * These bits are used for programming the stop bits.
	00: 1 Stop bit
	01: 0.5 Stop bit
	10: 2 Stop bits
	11: 1.5 Stop bit
 */
#define USART_StopBits_1						(uint32)(0)
#define USART_StopBits_half						(uint32)(1<<12)
#define USART_StopBits_2						(uint32)(2<<12)
#define USART_StopBits_1_half					(uint32)(3<<12)

/*
 * @ref USART_DataLength_define
 */
#define USART_DataLength8						(uint32)(0)
#define USART_DataLength9						(uint32)(1<<12)

/*
 * @ref USART_Parity_define
 */
#define USART_Parity_None 						(uint32)(0)
#define USART_Parity_Even 						(uint32)(1<<10)
#define USART_Parity_Odd 						(uint32)(1<<10 | 1<<9)

/*
 * @ref USART_FlowControl_define
 */
#define USART_FlowControl_NONE					(uint32)(0)
#define USART_FlowControl_CTS					(uint32)(1<<9)
#define USART_FlowControl_RTS					(uint32)(1<<8)
#define USART_FlowControl_BOTH					(uint32)(1<<8 | 1<<9)

/*
 * @ref USART_IRQ_Enable_define
 */
#define USART_NONE								(uint32)(0)
#define USART_RXIEN								(uint32)(1<<5)
#define USART_TXIEN								(uint32)(1<<7)
#define USART_TXIEN_RXIEN						(uint32)(1<<5 | 1<<7)
#define USART_TCIE								(uint32)(1<<6)
#define USART_PE								(uint32)(1<<8)

/*
 * @ref USART_IRQ_LineNumber
 */
#define USART1_LineNumber						37
#define USART2_LineNumber						38
#define USART3_LineNumber						39

/*
 * Baud Rate Calculation
 */
#define USARTDIV(FCLK, BaudRate)				(FCLK / (16*BaudRate))
#define DIV_Mantissa(FCLK, BaudRate)				(USARTDIV(FCLK,BaudRate)) //Integer Part

#define USARTDIV_Mul100(FCLK, BaudRate)				((FCLK*25) / (4*BaudRate))
#define Mantissa_Mul100(FCLK, BaudRate)				( DIV_Mantissa(FCLK, BaudRate) *100)
#define DIV_Fraction(FCLK, BaudRate)				(((USARTDIV_Mul100(FCLK, BaudRate) - Mantissa_Mul100(FCLK, BaudRate)) *16 ) /100)

#define USART_BRR(FCLK,BaudRate)					( (DIV_Mantissa(FCLK, BaudRate) << 4) | (DIV_Fraction(FCLK, BaudRate) &0xF))
/*
* ===============================================
*      APIs Supported by "MCAL USART DRIVER"
* ===============================================
*/

void USART_init(USART_pinConfig_t *pinConfig , USART_Registers_t* USARTx);
void USART_DeInit(USART_Registers_t *USARTx);

void USART_SetPins(USART_Registers_t *USARTx);

void USART_Send(USART_Registers_t *USARTx , uint16 *pTxBuffer , enum Polling_Mechanism PollingEn );
void USART_Recieve(USART_Registers_t *USARTx , uint16 *pTxBuffer , enum Polling_Mechanism PollingEn );

void USART_Wait_TC(USART_Registers_t *USARTx);
#endif /* INC_USART_H_ */
