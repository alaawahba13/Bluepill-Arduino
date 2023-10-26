/*
 * RCC.c
 *
 *  Created on: 16 Feb 2023
 *      Author: Alaa Wahba
 */



//=======================
//       Includes
//=======================
#include "inc/RCC.h"
/*
 *  APB Low-speed prescaler (APB1)
 	Set and cleared by software to control the division factor of the APB Low speed clock (PCLK1).
	0xx: HCLK not divided
	100: HCLK divided by 2
	101: HCLK divided by 4
	110: HCLK divided by 8
	111: HCLK divided by 16
 */
uint8 APB_PrescTable[8U] = {0,0,0,0,1,2,3,4};

/*
 *  AHB prescaler
	Set and cleared by software to control AHB clock division factor.
	0xxx: SYSCLK not divided
	1000: SYSCLK divided by 2
	1001: SYSCLK divided by 4
	1010: SYSCLK divided by 8
	1011: SYSCLK divided by 16
	1100: SYSCLK divided by 64
	1101: SYSCLK divided by 128
	1110: SYSCLK divided by 256
	1111: SYSCLK divided by 512
 */
uint8 AHB_PrescTable[16U] = {0,0,0,0,0,0,0,0,1,2,3,4,6,7,8,9};
uint32 RCC_getSYSCLK_Freq(void){
/*
 * 	00: HSI selected as system clock
	01: HSE selected as system clock
	10: PLL selected as system clock
 */
	uint8 SW = (RCC->CFGR >> 2) & 0b11;
	switch(SW){
	case 0 :
		return HSI;
		break;
	case 1 :
		//todo need to calculate  it //HSE User Should Specify it
		return HSE;
			break;
	case 2 :
		//todo need to calculate  it PLLCLK and PLLMUL & PLL Source MUX
		return HSE;
			break;
	}
	return 0;
}

uint32 RCC_getPCKL1_Freq(void){
	/* Get HCLK source and Compute PCLK1 frequency ---------------------------*/
	return (RCC_getHCKL_Freq() >> APB_PrescTable[((RCC->CFGR >> 8) &0b111)]); // First right shift is multiplication
}
uint32 RCC_getPCKL2_Freq(void){
	/* Get HCLK source and Compute PCLK2 frequency ---------------------------*/
	return (RCC_getHCKL_Freq() >> APB_PrescTable[((RCC->CFGR >> 11) &0b111)]);
}
uint32 RCC_getHCKL_Freq(void){
	/* Get HCLK source and Compute PCLK1 frequency ---------------------------*/
	return (RCC_getSYSCLK_Freq() >> AHB_PrescTable[((RCC->CFGR >> 4) &0b1111)]);
}


void RCC_CLK_EN(uint8 busID , uint8 periphralID){
	switch(busID){
	case APB1_ID:
		RCC->ABP1ENR |= (1<<periphralID);
		break;
	case APB2_ID:
		RCC->ABP2ENR |= (1<<periphralID);
		break;
	case AHB_ID:
		RCC->AHBENR |= (1<<periphralID);
		break;
	}
}

void RCC_CLK_RST(uint8 busID , uint8 periphralID){
	switch(busID){
		case APB1_ID:
			RCC->ABP1ENR |= (1<<periphralID);
			break;
		case APB2_ID:
			RCC->ABP1ENR |= (1<<periphralID);
			break;
		case AHB_ID:
			RCC->AHBENR &= ~(1<<periphralID);
			break;
		}
}

