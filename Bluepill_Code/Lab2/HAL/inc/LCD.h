/*
 * LCD.h
 *
 *  Created on: 14 Jan 2023
 *      Author: Alaa Wahba
 */

#ifndef LCD_H_
#define LCD_H_
#include "../../STM32_F103C6_Drivers/inc/GPIO.h"
#include "../../Library/BIT_MATH.h"

typedef struct {
	uint8 MODE;			// @ref LCD_MODE_t
	uint8 LCD_Size;		// @ref LCD_SIZE
    GPIO_Registers_t* LCD_PORT;
	GPIO_Registers_t* LCD_CONTROL_PORT;
	uint16 ENABLE_SWITCH;
	uint16 REGISTER_SELECT;
	uint16 PINS[8];			/*
							 * Four bit mode pins are initalized starting from PINS[0]
	 	 	 	 	 	 	 */

}LCD_t;

typedef enum{
	EIGHT_BIT_MODE,
	FOUR_BIT_MODE
}LCD_MODE_t;

// @ref LCD_SIZE
#define LCD_4x20				1
#define LCD_2x16				0


#define MAX_COLS				16
#define MAX_ROWS				2

/* Some instructions from data sheet */
#define CURSOR_FIRST_LINE 		(0x80)
#define CURSOR_SECOND_LINE 		(0xC0)
#ifdef LCD_4x20
#define CURSOR_THIRD_LINE 		(0x94)
#define CURSOR_FOURTH_LINE 		(0xD4)
#endif

#define FUNCTION_8BIT_2LINES 	(0x38)
#define FUNCTION_4BIT_2LINES 	(0x28)

#define SHIFT_DISPLAY_LEFT 		(0x18)
#define SHIFT_DISPLAY_RIGHT 	(0x1C)

#define MOVE_CURSOR_LEFT 		(0x10)
#define MOVE_CURSOR_RIGHT 		(0x14)
#define DISPLAY_OFF				(0x08)
#define DISPLAY_ON_CURSOR_OFF	(0x0C)
#define DISPLAY_ON_CURSOR_ON	(0x0E)
#define DISPLAY_ON_CURSOR_BLINK	(0x0F)

#define ENTRY_MODE 				(0x06)
#define ENTRY_DEC 				(0x04)
#define ENTRY_DEC_SHIFT			(0x05)
#define ENTRY_INC 				(0x06)
#define ENTRY_INC_SHIFT			(0x07)

#define CLEAR_SCREEN 			(0x01)

void isBusy(LCD_t *LCD_Config);
void lcd_kick(LCD_t *LCD_Config);
void lcd_Send_Command(unsigned char command,LCD_t *LCD_Config);
void lcd_Send_Char(unsigned char data,LCD_t *LCD_Config);
void lcd_send_String(char *string,LCD_t *LCD_Config);
void lcd_init(LCD_t *LCD_Config);
void lcd_Clear_Screen(LCD_t *LCD_Config);
void lcd_GOTO_XY(unsigned char row, unsigned char col,LCD_t *LCD_Config);
void lcd_display_number(int Number,LCD_t *LCD_Config);
void lcd_display_Real_number(double Number,LCD_t *LCD_Config);

#endif /* LCD_H_ */
