/*
 * IS31FL3730.h
 *
 * Created: 16/10/2022 17:26:39
 *  Author: youss
 */ 


#ifndef IS31FL3730_H_
#define IS31FL3730_H_

#include "TWI.h"
#include "IO_Macros.h"
#include "TWI.h"
#include "FONT_5x7.h"

// ADDRESSES.................
#define ADDRESS0 0xC0
#define ADDRESS1 0xC6
#define ADDRESS2 0xC2
#define ADDRESS3 0xC4

// REGISTERS.................
#define CONFIG 0x00
#define MATRIX1 0x01
#define MATRIX2 0x0E
#define UPDATE 0x0C
#define EFFECT 0x0D
#define PWMR 0x19
#define RESETR 0xFF

// BITS......................

// CONFIG ....
#define SSD 7
#define DM0 3
#define DM1 4
#define A_EN 2
#define ADM0 0
#define ADM1 1

// EFFECT ....
#define AGS0 4
#define AGS1 5
#define AGS2 6
#define CS0 0
#define CS1 1
#define CS2 2
#define CS3 3

// INDICES....................
// SHIFT ....
#define shift_col 7
#define shift_row 7

// DEC ....
#define DEC_col 7
#define DEC_row 7

// HEX ....
#define HEX_col 8
#define HEX_row 7

// BIN ....
#define BIN_col 9
#define BIN_row 7

// FUNCTIONS.................
void IS31_init(uint8_t adrs);
void IS31_begin();
void IS31_update(uint8_t adrs);
void IS31_test_sequence(uint8_t adrs);
void IS31_reset(uint8_t adrs);
void IS31_shift(uint8_t state);
void IS31_DEC(uint8_t state);
void IS31_HEX(uint8_t state);
void IS31_BIN(uint8_t state);
void IS31_setPWM(uint8_t adrs, uint8_t setting);
void IS31_setCurrent(uint8_t adrs, uint8_t setting);
void IS31_setBrightness(uint8_t setting);
void IS31_setMaxCurrent(uint8_t setting);
void IS31_writeBuffer();
void IS31_Update(uint8_t view);
void IS31_clear();
void IS31_setBase(uint8_t base);
void IS31_scrollLeft();
void IS31_scrollRight();
void IS31_scrollUp();
void IS31_scrollDown();
void IS31_drawChar(uint8_t pos, uint8_t chr);

uint8_t buffer[170];
uint8_t temp_buf[40];
uint8_t shift_state;
uint8_t base_state;


#endif /* IS31FL3730_H_ */