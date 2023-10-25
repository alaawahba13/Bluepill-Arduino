/*
 * SOI.c
 *
 *  Created on: 28 Feb 2023
 *      Author: Alaa Wahba
 */

//=======================
//       Includes
//=======================
#include "inc/SPI.h"
#define   RXNE_PIN            ((uint8_t)0x01)         //Bit 0 RXNE: Receive buffer not empty
#define   TXE_PIN             ((uint8_t)0x02)         //Bit 1 TXE: Transmit buffer empty
/*
 * =======================================================================================
 * 							Generic Variables
 * =======================================================================================
 */
// Each array element is for Each SPI
SPI_PinConfig_t Global_SPI_pinConfig[2] = { NULL, NULL };

#define SPI1_Index 			0
#define SPI2_Index 			1
/**================================================================
 * @Fn				- SPI_init
 * @brief 			- Initializes SPI
 * @param [in]	    - SPI_pinConfig :  pointer to a structure that contains the pin configuration information
 * @param [in]	    - SPIx : Pointer to the SPI number where x could be 1 or 2 depending on the device
 */
void SPI_init(SPI_PinConfig_t *SPI_pinConfig, SPI_Registers_t *SPIx) {
	// for Safety of registers
	uint16 tmpreg_CR1 = 0;
	uint16 tmpreg_CR2 = 0;

	/*            		Enable SPI clocks  					    */
	if (SPIx == SPI1) {
		Global_SPI_pinConfig[SPI1_Index] = *SPI_pinConfig;
		RCC_CLK_EN(APB2_ID,SPI1_ID);
	} else if (SPIx == SPI2) {
		Global_SPI_pinConfig[SPI2_Index] = *SPI_pinConfig;
		RCC_CLK_EN(APB1_ID,SPI2_ID);
	}

	/*			Enable SPI Module       */
	tmpreg_CR1  |= (1 << 6);		 //SPE: SPI enable
	/*			Configure SPI Mode	  						*/
	tmpreg_CR1 |= SPI_pinConfig->SPI_Mode;
	/*			Configure SPI Commuincation Mode		*/
	tmpreg_CR1 |= SPI_pinConfig->Commuincation_Mode;
	/*			Configure Data Size 		*/
	tmpreg_CR1 |= SPI_pinConfig->Data_Size;
	/*			Configure Frame Format		*/
	tmpreg_CR1 |= SPI_pinConfig->Frame_Format;
	/*			Configure BaudRate		*/
	tmpreg_CR1 |= SPI_pinConfig->BaudRate;

	/*			Configure NSS		*/
	if (SPI_pinConfig->NSS == SPI_NSS_Hard_Master_output_enable)
		tmpreg_CR2 |= SPI_pinConfig->NSS;
	else if (SPI_pinConfig->NSS == SPI_NSS_Hard_Master_output_disable || SPI_pinConfig->NSS == SPI_NSS_Hard_Slave)
		tmpreg_CR2 &= SPI_pinConfig->NSS;
	else
		tmpreg_CR1 |= SPI_pinConfig->NSS;

	/*			Configure Clock Polarity		*/
	tmpreg_CR1 |= SPI_pinConfig->CLK_Polarity;
	/*			Configure Clock Phase		*/
	tmpreg_CR1 |= SPI_pinConfig->CLK_Phase;

	/*			Enable interrupts		*/
	if (SPI_pinConfig->IRQ_Enable != SPI_IRQ_EN_None) {
		tmpreg_CR2 |= SPI_pinConfig->IRQ_Enable;
		if (SPIx == SPI1)
			NVIC_Enable(SPI1_IRQ_LineNumber);
		else if (SPIx == SPI2)
			NVIC_Enable(SPI2_IRQ_LineNumber);
	} else {
		if (SPIx == SPI1)
			NVIC_Disable(SPI1_IRQ_LineNumber);
		else if (SPIx == SPI2)
			NVIC_Disable(SPI2_IRQ_LineNumber);
	}

	SPIx->CR1 = tmpreg_CR1;
	SPIx->CR2 = tmpreg_CR2;

	SPIx->I2SCFGR &= ~(1<<11);
	SPIx->CRCPR = 0x0;

}
/**================================================================
 * @Fn				- SPI_DeInit
 * @brief 			- Deinitializes SPI
 * @param [in]	    - SPIx : Pointer to the SPI number where x could be 1 or 2 depending on the device
 */
void SPI_DeInit(SPI_Registers_t *SPIx) {
	if (SPIx == SPI1) {
		RCC_CLK_RST(APB2_ID,SPI1_ID);
		NVIC_Disable(SPI1_IRQ_LineNumber);
	} else if (SPIx == SPI2) {
		RCC_CLK_RST(APB1_ID,SPI2_ID);
		NVIC_Disable(SPI2_IRQ_LineNumber);
	}

}
/**================================================================
 * @Fn				- SPI_SendData
 * @brief 			- Send the data in pData to Slave
 * @param [in]	    - SPIx : Pointer to the SPI number where x could be 1 or 2 depending on the device
 * @param [in]	    - pData: Data to be sent
 * @param [in]		- PollingEn: Enables or disables the polling mechanism
 */
void SPI_SendData(SPI_Registers_t *SPIx, uint16 *pData,
		enum PollingMechanism PollingEn) {
	if (PollingEn == Pollingenable) {
		while (!((SPIx->SR) & TXE_PIN))
			;
	}
	SPIx->DR = *pData;
}
/**================================================================
 * @Fn				- SPI_RecieveData
 * @brief 			- Receives the data in pData from Slave
 * @param [in]	    - SPIx : Pointer to the SPI number where x could be 1 or 2 depending on the device
 * @param [in]	    - pData: Data to be received
 * @param [in]		- PollingEn: Enables or disables the polling mechanism
 */
void SPI_RecieveData(SPI_Registers_t *SPIx, uint16 *pData,
		enum PollingMechanism PollingEn) {
	if (PollingEn == Pollingenable) {
		while (!((SPIx->SR) & RXNE_PIN))
			;
	}
	*pData = SPIx->DR;
}
/**================================================================
 * @Fn				-SPI_GPIO_SetPins
 * @brief 			-initialize GPIO Pins
 * @param [in] 		-SPIx: where x can be (1..2 depending on device used)
 * @retval 			-none
 * @Note			-Should enable the corresponding ALT  & GPIO  in RCC clock Also called after SPI_init()
 */
void SPI_GPIO_SetPins(SPI_Registers_t *SPIx) {
	GPIO_PinConfig_t GPIO_pinConfig;
	if (SPIx == SPI1) {
		// MASTER
		if (Global_SPI_pinConfig[SPI1_Index].SPI_Mode == SPI_Mode_Master) {
			// NSS
			switch(Global_SPI_pinConfig[SPI1_Index].NSS)
			{
				case SPI_NSS_Hard_Master_output_disable :
				GPIO_pinConfig.MODE = MODE_INPUT_FLO;
				GPIO_pinConfig.Pin_Number = PIN_4;
				GPIO_init(GPIOA, &GPIO_pinConfig);
				break;
				case SPI_NSS_Hard_Master_output_enable :
				GPIO_pinConfig.MODE = MODE_OUTPUT_AF_PP;
				GPIO_pinConfig.Output_Speed = SPEED_10M;
				GPIO_pinConfig.Pin_Number = PIN_4;
				GPIO_init(GPIOA, &GPIO_pinConfig);
				break;
			}
			// SCLK
			GPIO_pinConfig.MODE = MODE_OUTPUT_AF_PP;
			GPIO_pinConfig.Output_Speed = SPEED_10M;
			GPIO_pinConfig.Pin_Number = PIN_5;
			GPIO_init(GPIOA, &GPIO_pinConfig);
			// MOSI
			GPIO_pinConfig.MODE = MODE_OUTPUT_AF_PP;
			GPIO_pinConfig.Output_Speed = SPEED_10M;
			GPIO_pinConfig.Pin_Number = PIN_7;
			GPIO_init(GPIOA, &GPIO_pinConfig);
			//MISO
			GPIO_pinConfig.MODE = MODE_INPUT_FLO;
			GPIO_pinConfig.Pin_Number = PIN_6;
			GPIO_init(GPIOA, &GPIO_pinConfig);
		}
		// SLAVE
		else {
			//NSS
			if (Global_SPI_pinConfig[SPI1_Index].SPI_Mode == SPI_NSS_Hard_Slave) {
				GPIO_pinConfig.MODE = MODE_INPUT_FLO;
				GPIO_pinConfig.Pin_Number = PIN_4;
				GPIO_init(GPIOA, &GPIO_pinConfig);
			}
			// SCLK
			GPIO_pinConfig.MODE = MODE_INPUT_FLO;
			GPIO_pinConfig.Pin_Number = PIN_5;
			GPIO_init(GPIOA, &GPIO_pinConfig);
			// MOSI
			GPIO_pinConfig.MODE = MODE_INPUT_FLO;
			GPIO_pinConfig.Pin_Number = PIN_7;
			GPIO_init(GPIOA, &GPIO_pinConfig);
			//MISO
			GPIO_pinConfig.MODE = MODE_OUTPUT_AF_PP;
			GPIO_pinConfig.Output_Speed = SPEED_10M;
			GPIO_pinConfig.Pin_Number = PIN_6;
			GPIO_init(GPIOA, &GPIO_pinConfig);
		}

	} else if (SPIx == SPI2) {
		// MASTER
		if (Global_SPI_pinConfig[SPI2_Index].SPI_Mode == SPI_Mode_Master) {
			// NSS

			switch(Global_SPI_pinConfig[SPI2_Index].NSS)
			{
				case SPI_NSS_Hard_Master_output_disable :
				GPIO_pinConfig.MODE = MODE_INPUT_FLO;
				GPIO_pinConfig.Pin_Number = PIN_12;
				GPIO_init(GPIOB, &GPIO_pinConfig);
				break;
				case SPI_NSS_Hard_Master_output_enable :
				GPIO_pinConfig.MODE = MODE_OUTPUT_AF_PP;
				GPIO_pinConfig.Output_Speed = SPEED_10M;
				GPIO_pinConfig.Pin_Number = PIN_12;
				GPIO_init(GPIOB, &GPIO_pinConfig);
				break;
			}
			// SCLK
			GPIO_pinConfig.MODE = MODE_OUTPUT_AF_PP;
			GPIO_pinConfig.Output_Speed = SPEED_10M;
			GPIO_pinConfig.Pin_Number = PIN_13;
			GPIO_init(GPIOB, &GPIO_pinConfig);
			// MOSI
			GPIO_pinConfig.MODE = MODE_OUTPUT_AF_PP;
			GPIO_pinConfig.Output_Speed = SPEED_10M;
			GPIO_pinConfig.Pin_Number = PIN_15;
			GPIO_init(GPIOB, &GPIO_pinConfig);
			//MISO
			GPIO_pinConfig.MODE = MODE_INPUT_FLO;
			GPIO_pinConfig.Pin_Number = PIN_14;
			GPIO_init(GPIOB, &GPIO_pinConfig);
		}
		// SLAVE
		else {
			//NSS
			if (Global_SPI_pinConfig[SPI2_Index].SPI_Mode == SPI_NSS_Hard_Slave) {
				GPIO_pinConfig.MODE = MODE_INPUT_FLO;
				GPIO_pinConfig.Pin_Number = PIN_12;
				GPIO_init(GPIOB, &GPIO_pinConfig);
			}			// SCLK
			GPIO_pinConfig.MODE = MODE_INPUT_FLO;
			GPIO_pinConfig.Pin_Number = PIN_13;
			GPIO_init(GPIOB, &GPIO_pinConfig);
			// MOSI
			GPIO_pinConfig.MODE = MODE_INPUT_FLO;
			GPIO_pinConfig.Pin_Number = PIN_15;
			GPIO_init(GPIOB, &GPIO_pinConfig);
			//MISO
			GPIO_pinConfig.MODE = MODE_OUTPUT_AF_PP;
			GPIO_pinConfig.Output_Speed = SPEED_10M;
			GPIO_pinConfig.Pin_Number = PIN_14;
			GPIO_init(GPIOB, &GPIO_pinConfig);
		}

	}

}
/**================================================================
 * @Fn				- SPI_RecieveData
 * @brief 			- Send and Receives the data in pData from Slave
 * @param [in]	    - SPIx : Pointer to the SPI number where x could be 1 or 2 depending on the device
 * @param [in]	    - pData: Data to be sent and received
 * @param [in]		- PollingEn: Enables or disables the polling mechanism
 */
void SPI_RXTX(SPI_Registers_t *SPIx, uint16 *pData,
		enum PollingMechanism PollingEn) {
	if (PollingEn ==Pollingenable ) {
		while (!((SPIx->SR) & TXE_PIN));
	}
	SPIx->DR = *pData;
	if (PollingEn == Pollingenable) {
		while (!((SPIx->SR) & RXNE_PIN));
	}
	*pData = SPIx->DR;
}

//ISR
void SPI1_IRQHandler(void) {
	struct IRQ_source_t IRQ;
	IRQ.TXE = ((SPI1->SR & (1 << 1)) >> 1);
	IRQ.RXNE = ((SPI1->SR & (1 << 0)) >> 0);
	IRQ.ERRI = ((SPI1->SR & (1 << 4)) >> 4);
	Global_SPI_pinConfig[SPI1_Index].P_CallBackFun(IRQ);
}

void SPI2_IRQHandler(void) {
	struct IRQ_source_t IRQ;
	IRQ.TXE = ((SPI2->SR & (1 << 1)) >> 1);
	IRQ.RXNE = ((SPI2->SR & (1 << 0)) >> 0);
	IRQ.ERRI = ((SPI2->SR & (1 << 4)) >> 4);
	Global_SPI_pinConfig[SPI2_Index].P_CallBackFun(IRQ);
}
