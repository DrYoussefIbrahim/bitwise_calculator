/*
Created: 12/10/2022 16:50:48
Author : Youssef Ibrahim

Bitwise Claculator (c) by Youssef Ibrahim

Bitwise Claculator is licensed under a
Creative Commons Attribution-ShareAlike 4.0 International License.

You should have received a copy of the license along with this
work.  If not, see <http://creativecommons.org/licenses/by-sa/4.0/>.
 */ 

#include "IS31FL3730.h"

void IS31_init(uint8_t adrs){
	
	IS31_reset(adrs);
	
	uint8_t cfg = 0;
	cfg |= (1 << DM0) | (1 << DM1) | (1 << ADM0);
	TWI_BeginTransmission();
	switch(adrs){
		case 0:
		TWI_Transmit(ADDRESS0);
		break;
		
		case 1:
		TWI_Transmit(ADDRESS1);
		break;
		
		case 2:
		TWI_Transmit(ADDRESS2);
		break;
		
		case 3:
		TWI_Transmit(ADDRESS3);
		break;
		
		default:
		break;
	}
	TWI_Transmit(CONFIG);
	TWI_Transmit(cfg);
	TWI_EndTransmission();
}

void IS31_setPWM(uint8_t adrs, uint8_t setting){
	TWI_BeginTransmission();
	switch(adrs){
		case 0:
		TWI_Transmit(ADDRESS0);
		break;
		
		case 1:
		TWI_Transmit(ADDRESS1);
		break;
		
		case 2:
		TWI_Transmit(ADDRESS2);
		break;
		
		case 3:
		TWI_Transmit(ADDRESS3);
		break;
		
		default:
		break;
	}
	TWI_Transmit(PWMR);
	TWI_Transmit(setting);
	TWI_EndTransmission();
}

void IS31_setCurrent(uint8_t adrs, uint8_t setting){
	TWI_BeginTransmission();
	switch(adrs){
		case 0:
		TWI_Transmit(ADDRESS0);
		break;
		
		case 1:
		TWI_Transmit(ADDRESS1);
		break;
		
		case 2:
		TWI_Transmit(ADDRESS2);
		break;
		
		case 3:
		TWI_Transmit(ADDRESS3);
		break;
		
		default:
		break;
	}
	TWI_Transmit(EFFECT);
	TWI_Transmit(setting);
	TWI_EndTransmission();
}

void IS31_setBrightness(uint8_t setting){
	for(uint8_t i =0 ; i < 4; i ++){
		IS31_setPWM(i, setting);
	}
}

void IS31_setMaxCurrent(uint8_t setting){
	for(uint8_t i =0 ; i < 4; i ++){
		IS31_setCurrent(i, setting);
	}
}

void IS31_begin(){
	for(uint8_t i = 0; i < 4; i++){
		IS31_init(i);
	}
	
	for(uint8_t i =0; i < 80; i++){
		buffer[i] = 0x00;
	}
	
	shift_state = 0;
}

void IS31_reset(uint8_t adrs){
	TWI_BeginTransmission();
	switch(adrs){
		case 0:
		TWI_Transmit(ADDRESS0);
		break;
		
		case 1:
		TWI_Transmit(ADDRESS1);
		break;
		
		case 2:
		TWI_Transmit(ADDRESS2);
		break;
		
		case 3:
		TWI_Transmit(ADDRESS3);
		break;
		
		default:
		break;
	}
	TWI_Transmit(RESETR);
	TWI_Transmit(0x00);
	TWI_EndTransmission();
}

void IS31_update(uint8_t adrs){
	TWI_BeginTransmission();
	switch(adrs){
		case 0:
		TWI_Transmit(ADDRESS0);
		break;
		
		case 1:
		TWI_Transmit(ADDRESS1);
		break;
		
		case 2:
		TWI_Transmit(ADDRESS2);
		break;
		
		case 3:
		TWI_Transmit(ADDRESS3);
		break;
		
		default:
		break;
	}
	TWI_Transmit(UPDATE);
	TWI_Transmit(0x00);
	TWI_EndTransmission();
}

void IS31_test_sequence(uint8_t adrs){
	TWI_BeginTransmission();
	switch(adrs){
		case 0:
		TWI_Transmit(ADDRESS0);
		break;
		
		case 1:
		TWI_Transmit(ADDRESS1);
		break;
		
		case 2:
		TWI_Transmit(ADDRESS2);
		break;
		
		case 3:
		TWI_Transmit(ADDRESS3);
		break;
		
		default:
		break;
	}
	TWI_Transmit(MATRIX1);
	TWI_Transmit(0b00100);
	TWI_Transmit(0b00110);
	TWI_Transmit(0b00100);
	TWI_Transmit(0b00100);
	TWI_Transmit(0b00100);
	TWI_Transmit(0b00100);
	TWI_Transmit(0b01110);
	TWI_EndTransmission();
	
	TWI_BeginTransmission();
	switch(adrs){
		case 0:
		TWI_Transmit(ADDRESS0);
		break;
		
		case 1:
		TWI_Transmit(ADDRESS1);
		break;
		
		case 2:
		TWI_Transmit(ADDRESS2);
		break;
		
		case 3:
		TWI_Transmit(ADDRESS3);
		break;
		
		default:
		break;
	}
	TWI_Transmit(MATRIX2);
	TWI_Transmit(0b1100010);
	TWI_Transmit(0b1010001);
	TWI_Transmit(0b1001001);
	TWI_Transmit(0b1001001);
	TWI_Transmit(0b1000110);
	//TWI_Transmit(0b1000000);
	TWI_EndTransmission();
	
	IS31_update(adrs);
}

void IS31_shift(uint8_t state){
	uint8_t current_buf = 0;
	for(uint8_t col = 5; col > 0; col--){
		if((1 << 6) & temp_buf[15 + col - 1]){
			current_buf |= (1 << (5 - col));
		}
	}
	TWI_BeginTransmission();
	TWI_Transmit(ADDRESS1);
	TWI_Transmit(MATRIX1 + (shift_col - 1));
	if(state){
		TWI_Transmit(current_buf | (1 << (shift_row - 1)));
		shift_state = 1;
	} else {
		TWI_Transmit(current_buf);
		shift_state = 0;
	}
	TWI_EndTransmission();
	IS31_update(1);
}

void IS31_DEC(uint8_t state){
	if(state){
		TWI_BeginTransmission();
		TWI_Transmit(ADDRESS2);
		TWI_Transmit(MATRIX2 + (DEC_col - 1));
		TWI_Transmit(1 << (DEC_row - 1));
		TWI_EndTransmission();
		} else {
		TWI_BeginTransmission();
		TWI_Transmit(ADDRESS2);
		TWI_Transmit(MATRIX2 + (DEC_col - 1));
		TWI_Transmit(0x00);
		TWI_EndTransmission();
	}
}

void IS31_HEX(uint8_t state){
	if(state){
		TWI_BeginTransmission();
		TWI_Transmit(ADDRESS2);
		TWI_Transmit(MATRIX2 + (HEX_col - 1));
		TWI_Transmit(1 << (HEX_row - 1));
		TWI_EndTransmission();
		} else {
		TWI_BeginTransmission();
		TWI_Transmit(ADDRESS2);
		TWI_Transmit(MATRIX2 + (HEX_col - 1));
		TWI_Transmit(0x00);
		TWI_EndTransmission();
	}
}

void IS31_BIN(uint8_t state){
	if(state){
		TWI_BeginTransmission();
		TWI_Transmit(ADDRESS2);
		TWI_Transmit(MATRIX2 + (BIN_col - 1));
		TWI_Transmit(1 << (BIN_row - 1));
		TWI_EndTransmission();
		} else {
		TWI_BeginTransmission();
		TWI_Transmit(ADDRESS2);
		TWI_Transmit(MATRIX2 + (BIN_col - 1));
		TWI_Transmit(0x00);
		TWI_EndTransmission();
	}
}

void IS31_setBase(uint8_t base){
	switch(base){
		case 1:
		IS31_DEC(1);
		IS31_HEX(0);
		IS31_BIN(0);
		base_state = 1;
		break;
		
		case 2:
		IS31_DEC(0);
		IS31_HEX(1);
		IS31_BIN(0);
		base_state = 2;
		break;
		
		case 3:
		IS31_DEC(0);
		IS31_HEX(0);
		IS31_BIN(1);
		base_state = 3;
		break;
		
		default:
		break;
	}
	IS31_update(2);
}

void IS31_clear(){
	for(uint8_t i =0; i < 80; i++){
		buffer[i] = 0x00;
	}
}

void IS31_scrollLeft(){
	for(uint8_t col = 169; col > 0; col--){
		buffer[col] = buffer[col - 1];
	}
	buffer[0] = 0;
}

void IS31_scrollRight(){
	for(uint8_t col = 0; col < 169; col++){
		buffer[col] = buffer[col + 1];
	}
	buffer[169] = 0;
}

void IS31_scrollUp(){
	for(uint8_t col = 0; col < 170; col++){
		buffer[col] >>= 1;
	}
}

void IS31_scrollDown(){
	for(uint8_t col = 0; col < 170; col++){
		buffer[col] <<= 1;
	}
}

void IS31_drawChar(uint8_t pos, uint8_t chr){
	for(uint8_t col = 0; col < 5; col++){
		buffer[pos + col] = FONT_5_7[chr][col];
	}
}

void IS31_writeBuffer(){
	for(uint8_t matrix = 0; matrix < 40; matrix += 5){
		if(matrix % 10){
			TWI_BeginTransmission();
			if(matrix < 10){
				TWI_Transmit(ADDRESS0);
				} else if (matrix < 20){
				TWI_Transmit(ADDRESS1);
				} else if (matrix < 30){
				TWI_Transmit(ADDRESS2);
				} else {
				TWI_Transmit(ADDRESS3);
			}
			TWI_Transmit(MATRIX1);
			for(uint8_t row = 0; row < 7; row++){
				uint8_t current_row = 0;
				for(uint8_t col = 5; col > 0; col--){
					if((1 << row) & temp_buf[matrix + col - 1]){
						current_row |= (1 << (5 - col));
					}
				}
				if(shift_state && matrix==15 && row==6){
					current_row |= (1 << (shift_row - 1));
				}
				TWI_Transmit(current_row);
			}
			TWI_EndTransmission();
		} else {
			TWI_BeginTransmission();
			if(matrix < 10){
				TWI_Transmit(ADDRESS0);
			} else if (matrix < 20){
				TWI_Transmit(ADDRESS1);
			} else if (matrix < 30){
				TWI_Transmit(ADDRESS2);
			} else {
				TWI_Transmit(ADDRESS3);
			}
			TWI_Transmit(MATRIX2);
			for(uint8_t col = 5; col > 0; col--){
				TWI_Transmit(temp_buf[matrix + col - 1]);
			}
			TWI_EndTransmission();
		}
	}
}

void IS31_Update(uint8_t view){
	uint8_t indx = 0;
	for(uint8_t elem = view; elem < view + 40; elem++){
		temp_buf[indx] = buffer[elem];
		indx++;
	}
	IS31_writeBuffer();
	for(uint8_t i =0; i < 4; i++){
		IS31_update(i);	
	}
}