/*
 * LCD.c
 *
 *  Created on: 14 Jan 2023
 *      Author: Alaa Wahba
 */

#include "inc/LCD.h"
void delay_ms(uint32 time) {
	for (uint32 i = 0; i < time; i++)
		for (uint32 j = 0; j < 255; j++)
			;

}
GPIO_PinConfig_t GPIO_pinConfig;

void isBusy(LCD_t *LCD_Config) {
	/* 1.Set the data port as input
	 * 2.Set RW = 1 to read && Command mode >> RS =0
	 * 3.Deactivate and Activate Enable
	 */
	// STEP ONE
	uint32 MAX = 4;
	if (LCD_Config->MODE == EIGHT_BIT_MODE)
		MAX = 8;
	for (int i = 0; i < MAX; i++) {
		GPIO_pinConfig.MODE = MODE_INPUT_FLO;
		GPIO_pinConfig.Pin_Number = LCD_Config->PINS[i];
		GPIO_init(LCD_Config->LCD_PORT, &GPIO_pinConfig);

	}
	// STEP TWO
	GPIO_WritePin(LCD_Config->LCD_CONTROL_PORT, LCD_Config->REGISTER_SELECT,
	PIN_LOW);
	// STEP THREE
	lcd_kick(LCD_Config);
}

void lcd_init(LCD_t *LCD_Config) {

	/*
	 * 1.delay
	 * 2. Configure port DATA as OUTPUT
	 * 3. Configure port CONTROL as OUTPUT
	 * 4. Reset all control pins.
	 * 5. delay
	 * 6. Clear Screen
	 * 7. Send command 8BIT or 4BIT
	 * 8. Go to Entry Mode
	 */

	// you must wait for the hardware to initialize
	delay_ms(20);
	// set port as ouput to write commands
	if (LCD_Config->MODE == EIGHT_BIT_MODE) {
		for (int i = 0; i < 8; i++) {
			GPIO_pinConfig.MODE = MODE_OUTPUT_PP;
			GPIO_pinConfig.Output_Speed = SPEED_10M;
			GPIO_pinConfig.Pin_Number = LCD_Config->PINS[i];
			GPIO_init(LCD_Config->LCD_PORT, &GPIO_pinConfig);
		}
	} else {
		for (int i = 0; i < 4; i++) {
			GPIO_pinConfig.Pin_Number = LCD_Config->PINS[i];
			GPIO_pinConfig.MODE = MODE_OUTPUT_PP;
			GPIO_pinConfig.Output_Speed = SPEED_10M;
			GPIO_init(LCD_Config->LCD_PORT, &GPIO_pinConfig);

		}
	}
	// set three control pins as output and write 0
	GPIO_pinConfig.MODE = MODE_OUTPUT_PP;
	GPIO_pinConfig.Pin_Number = LCD_Config->ENABLE_SWITCH;
	GPIO_pinConfig.Output_Speed = SPEED_10M;
	GPIO_init(LCD_Config->LCD_CONTROL_PORT, &GPIO_pinConfig);
	GPIO_WritePin(LCD_Config->LCD_CONTROL_PORT, LCD_Config->ENABLE_SWITCH,
	PIN_LOW);

	GPIO_pinConfig.MODE = MODE_OUTPUT_PP;
	GPIO_pinConfig.Pin_Number = LCD_Config->REGISTER_SELECT;
	GPIO_pinConfig.Output_Speed = SPEED_10M;
	GPIO_init(LCD_Config->LCD_CONTROL_PORT, &GPIO_pinConfig);
	GPIO_WritePin(LCD_Config->LCD_CONTROL_PORT, LCD_Config->REGISTER_SELECT,
	PIN_LOW);

	delay_ms(15);
	lcd_Clear_Screen(LCD_Config);
	/* Define the lcd Mode by its instruction*/
	if (LCD_Config->MODE == EIGHT_BIT_MODE)
		lcd_Send_Command(FUNCTION_8BIT_2LINES, LCD_Config);

	if (LCD_Config->MODE == FOUR_BIT_MODE) {
		// Command that initializes LCD as four bit mode
		lcd_Send_Command(0x02, LCD_Config);
		lcd_Send_Command(FUNCTION_4BIT_2LINES, LCD_Config);
	}
	lcd_Send_Command(ENTRY_MODE, LCD_Config);
	lcd_Send_Command(CURSOR_FIRST_LINE, LCD_Config);
	lcd_Send_Command(DISPLAY_ON_CURSOR_BLINK, LCD_Config);

}
void lcd_Send_Command(unsigned char command, LCD_t *LCD_Config) {
	/*
	 * 1.Write command to port
	 * 2.Reset control pins
	 * 3.Deactivate and Activate Enable
	 */
	if (LCD_Config->MODE == EIGHT_BIT_MODE) {
		GPIO_WritePort(LCD_Config->LCD_PORT, command);
		GPIO_WritePin(LCD_Config->LCD_CONTROL_PORT, LCD_Config->REGISTER_SELECT,
		PIN_LOW);
		delay_ms(1);
		lcd_kick(LCD_Config);
	}

	if (LCD_Config->MODE == FOUR_BIT_MODE) {
		GPIO_WritePin(LCD_Config->LCD_CONTROL_PORT, LCD_Config->REGISTER_SELECT,
		PIN_LOW);

		GPIO_WritePin(LCD_Config->LCD_PORT, LCD_Config->PINS[0],
				GET(command, 4));
		GPIO_WritePin(LCD_Config->LCD_PORT, LCD_Config->PINS[1],
				GET(command, 5));
		GPIO_WritePin(LCD_Config->LCD_PORT, LCD_Config->PINS[2],
				GET(command, 6));
		GPIO_WritePin(LCD_Config->LCD_PORT, LCD_Config->PINS[3],
				GET(command, 7));

		delay_ms(1);
		lcd_kick(LCD_Config);

		GPIO_WritePin(LCD_Config->LCD_PORT, LCD_Config->PINS[0],
				GET(command, 0));
		GPIO_WritePin(LCD_Config->LCD_PORT, LCD_Config->PINS[1],
				GET(command, 1));
		GPIO_WritePin(LCD_Config->LCD_PORT, LCD_Config->PINS[2],
				GET(command, 2));
		GPIO_WritePin(LCD_Config->LCD_PORT, LCD_Config->PINS[3],
				GET(command, 3));

		delay_ms(1);
		lcd_kick(LCD_Config);
	}
}
void lcd_Send_Char(unsigned char character, LCD_t *LCD_Config) {

	if (LCD_Config->MODE == EIGHT_BIT_MODE) {
		/*
		 *1. set the RS to 1 to send data
		 *2. Write data
		 *3. Reactivate Enable
		 */
		GPIO_WritePort(LCD_Config->LCD_PORT, character);
		GPIO_WritePin(LCD_Config->LCD_CONTROL_PORT, LCD_Config->REGISTER_SELECT,
		PIN_HIGH);

		delay_ms(1);
		lcd_kick(LCD_Config);
	}
	if (LCD_Config->MODE == FOUR_BIT_MODE) {
		GPIO_WritePin(LCD_Config->LCD_CONTROL_PORT, LCD_Config->REGISTER_SELECT,
		PIN_HIGH);

		GPIO_WritePin(LCD_Config->LCD_PORT, LCD_Config->PINS[0],
				GET(character, 4));
		GPIO_WritePin(LCD_Config->LCD_PORT, LCD_Config->PINS[1],
				GET(character, 5));
		GPIO_WritePin(LCD_Config->LCD_PORT, LCD_Config->PINS[2],
				GET(character, 6));
		GPIO_WritePin(LCD_Config->LCD_PORT, LCD_Config->PINS[3],
				GET(character, 7));
		delay_ms(1);
		lcd_kick(LCD_Config);
		GPIO_WritePin(LCD_Config->LCD_PORT, LCD_Config->PINS[0],
				GET(character, 0));
		GPIO_WritePin(LCD_Config->LCD_PORT, LCD_Config->PINS[1],
				GET(character, 1));
		GPIO_WritePin(LCD_Config->LCD_PORT, LCD_Config->PINS[2],
				GET(character, 2));
		GPIO_WritePin(LCD_Config->LCD_PORT, LCD_Config->PINS[3],
				GET(character, 3));
		delay_ms(1);
		lcd_kick(LCD_Config);
	}

}
void lcd_send_String(char *string, LCD_t *LCD_Config) {
	// keeps track of chars count
	int count = 0;
	while (*string > 0) {
		count++;
		lcd_Send_Char(*string++, LCD_Config);
		// if first line is full go to second
		if (count == MAX_COLS) {
			lcd_GOTO_XY(1, 0, LCD_Config);
		}	// if both lines are full clear and start over.
		else if (count == (MAX_COLS * MAX_ROWS)) {
			lcd_Clear_Screen(LCD_Config);
			lcd_GOTO_XY(0, 0, LCD_Config);
			count = 0;
		}
		if (LCD_Config->LCD_Size == LCD_4x20) {
			if (count == MAX_COLS * 2) {
				lcd_GOTO_XY(2, 0, LCD_Config);
			} else if (count == MAX_COLS * 3) {
				lcd_GOTO_XY(3, 0, LCD_Config);
			}
		}

	}
}
void lcd_GOTO_XY(unsigned char row, unsigned char col, LCD_t *LCD_Config) {

	if (row == 0) {
		if ((col < MAX_COLS) && (col >= 0))
			lcd_Send_Command(CURSOR_FIRST_LINE + col, LCD_Config);
	} else if (row == 1) {
		if ((col < MAX_COLS) && (col >= 0))
			lcd_Send_Command(CURSOR_SECOND_LINE + col, LCD_Config);
	}
	if (LCD_Config->LCD_Size == LCD_4x20) {
		if (row == 2) {
			if ((col < MAX_COLS) && (col >= 0))
				lcd_Send_Command(CURSOR_THIRD_LINE + col, LCD_Config);
		} else if (row == 3) {
			if ((col < MAX_COLS) && (col >= 0))
				lcd_Send_Command(CURSOR_FOURTH_LINE + col, LCD_Config);
		}
	}
}
void lcd_Clear_Screen(LCD_t *LCD_Config) {
	lcd_Send_Command(CLEAR_SCREEN,LCD_Config);
}
void lcd_display_number(int Number, LCD_t *LCD_Config) {
	char str[7];
// Converts Int to String
	sprintf(str, "%d", Number);
	lcd_send_String(str,LCD_Config);
}
void lcd_display_Real_number(double Number, LCD_t *LCD_Config) {
	char str[16];
	char *tmpSign = (Number > 0) ? "" : "-";
	float tmpNum = (Number > 0) ? Number : -Number;

	int tmpVal = tmpNum;
	float tmpFrac = tmpNum - tmpVal;

	int Frac = tmpFrac * 10000;

	sprintf(str, "%s%d.%04d", tmpSign, tmpVal, Frac);
	lcd_send_String(str,LCD_Config);

}
void lcd_kick(LCD_t *LCD_Config) {
// Enable =0 >> LCD Busy
	GPIO_WritePin(LCD_Config->LCD_CONTROL_PORT, LCD_Config->ENABLE_SWITCH, PIN_HIGH);
	delay_ms(50);
	GPIO_WritePin(LCD_Config->LCD_CONTROL_PORT, LCD_Config->ENABLE_SWITCH, PIN_LOW);
}

void LCD_createCustomCharacter(uint8 *pattern, uint8 location,
		LCD_t *LCD_Config) {
	uint8 i = 0;

	lcd_Send_Command(0x40 + (location * 8),LCD_Config); /* Send the Address of CGRAM */

	for (i = 0; i < 8; i++) {
		lcd_Send_Char(pattern[i],LCD_Config); /* Pass the Bytes of pattern on LCD */
	}
}

