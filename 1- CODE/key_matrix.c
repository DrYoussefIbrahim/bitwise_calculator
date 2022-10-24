/*
Created: 12/10/2022 16:50:48
Author : Youssef Ibrahim

Bitwise Claculator (c) by Youssef Ibrahim

Bitwise Claculator is licensed under a
Creative Commons Attribution-ShareAlike 4.0 International License.

You should have received a copy of the license along with this
work.  If not, see <http://creativecommons.org/licenses/by-sa/4.0/>.
 */ 

#include "key_matrix.h"

void key_begin(){
	PORTC &= ~(1 << 3);
	DDRC |= (1 << 3);
	
	PORTD = (1 << 2) | (1 << 7);
	DDRD |= (0x1F << 3);
}

int8_t key_scan(){
	unsigned long key_states = 0;
	
	for(uint8_t col = 0; col < 4; col++){
		PORTD &= 0b10000111;
		PORTD |= (8 << col);
		_delay_us(100);
		
		PORTD &= ~(1 << 7);
		_delay_us(100);
		PORTD |= (1 << 7);	
		_delay_us(100);
		
		for(uint8_t row = 0; row < 8; row++){
			key_states <<= 1;
			
			if(PINC & (1<< 2)){
				key_states |= 1;
			}
			
			PORTC |= (1 << 3);
			_delay_us(100);
			PORTC &= ~(1 << 3);
			_delay_us(100);
		}
	}
	
	if(key_states == 0){
		return -1;
	}
	
	uint8_t i;
	for(i = 0 ; i < 31; i++){
		if(key_states >> i & 1){
			break;
		}
	}
	
	switch(i){
		case ZERO:
		return 0;
		break;
		
		case ONE:
		return 1;
		break;
		
		case TWO:
		return 2;
		break;
		
		case THREE:
		return 3;
		break;
		
		case FOUR:
		return 4;
		break;
		
		case FIVE:
		return 5;
		break;
		
		case SIX:
		return 6;
		break;
		
		case SEVEN:
		return 7;
		break;
		
		case EIGHT:
		return 8;
		break;
		
		case NINE:
		return 9;
		break;
		
		case PM:
		return 10;
		break;
		
		case EQUALS:
		return 11;
		break;
		
		case PLUS:
		return 12;
		break;
		
		case MULT:
		return 13;
		break;
		
		case MINUS:
		return 14;
		break;
		
		case CLR:
		return 15;
		break;
		
		case LEFT:
		return 16;
		break;
		
		case BASE:
		return 17;
		break;
		
		case SHFT:
		return 18;
		break;
		
		case RIGHT:
		return 19;
		break;
		
		default:
		return -1;
		break;
	}

}