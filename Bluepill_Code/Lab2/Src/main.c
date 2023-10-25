/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Team Project
 * @brief          : Communicating with Arduino .
 ******************************************************************************
 ******************************************************************************
 */

#include "main.h"
uint8 Buffer[8];  // store data sent from arduino
uint8 authorized_ID[] = "53f30211";

void clock_init() {
	RCC_CLK_EN(APB2_ID, GPIOA_ID);
	RCC_CLK_EN(APB2_ID, GPIOB_ID);
	RCC_CLK_EN(APB2_ID, AFIO_ID);
}

int main(void) {
	//  Enable Clock for peripherals
	clock_init();
	// USART configuration
	USART_pinConfig_t USART_pinConfig;
	USART_pinConfig.BaudRate = USART_BaudRate_9600;
	USART_pinConfig.Parity = USART_Parity_None;
	USART_pinConfig.DataLength = USART_DataLength8;
	USART_pinConfig.StopBits = USART_StopBits_1;
	USART_pinConfig.FlowControl = USART_FlowControl_NONE;
	USART_pinConfig.USART_Mode = USART_TXRXEN;
	USART_pinConfig.P_CallBack_Fun = NULL;
	USART_pinConfig.IRQ_Enable = USART_NONE;
	USART_init(&USART_pinConfig, USART1);
	USART_SetPins(USART1);
	//LCD init
	LCD_t LCD_pinConfig = { FOUR_BIT_MODE, LCD_2x16, GPIOA, GPIOB, PIN_0, PIN_1,
	PIN_4, PIN_5, PIN_6, PIN_7 };
	lcd_init(&LCD_pinConfig);
	//GPIO init
	GPIO_PinConfig_t GPIO_pinConfig;
	GPIO_pinConfig.MODE = MODE_OUTPUT_PP;
	GPIO_pinConfig.Output_Speed = SPEED_10M;
	GPIO_pinConfig.Pin_Number = Green_LED;
	GPIO_init(LED_GPIO, &GPIO_pinConfig);
	GPIO_pinConfig.Pin_Number = Red_LED;
	GPIO_init(LED_GPIO, &GPIO_pinConfig);
	// Display message
	lcd_Clear_Screen(&LCD_pinConfig);
	lcd_GOTO_XY(0, 6, &LCD_pinConfig);
	lcd_send_String("LAB2", &LCD_pinConfig);
	lcd_GOTO_XY(1, 4, &LCD_pinConfig);
	lcd_send_String("4TH CSE", &LCD_pinConfig);

	while (1) {


		for (int i = 0; i<8; i++) {
			// Uart recieve with polling enabled
			USART_Recieve(USART1, &Buffer[i], Enable);
			Buffer[i + 1] = '\0';
		}
		lcd_Clear_Screen(&LCD_pinConfig);
		lcd_send_String("Tag ID: ", &LCD_pinConfig);
		for (int i = 0; i<8; i++) {
			lcd_Send_Char(Buffer[i], &LCD_pinConfig);
		}
		// Compare the recieved data with the stored one.
		if (!strcmp(Buffer,authorized_ID)) {
			GPIO_WritePin(LED_GPIO, Red_LED, PIN_LOW);
			GPIO_WritePin(LED_GPIO, Green_LED, PIN_HIGH);
		} else {
			GPIO_WritePin(LED_GPIO, Green_LED, PIN_LOW);
			GPIO_WritePin(LED_GPIO, Red_LED, PIN_HIGH);
		}
	}

}

