/*

Created: 12/10/2022 16:50:48
Author : Youssef Ibrahim

Bitwise Claculator (c) by Youssef Ibrahim

Bitwise Claculator is licensed under a
Creative Commons Attribution-ShareAlike 4.0 International License.

You should have received a copy of the license along with this
work.  If not, see <http://creativecommons.org/licenses/by-sa/4.0/>.

*/

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "IS31FL3730.h"
#include "key_matrix.h"

void update_reg();
volatile uint8_t head = 10;
volatile uint8_t view = 10;
volatile uint32_t reg = 0;
volatile uint32_t regB = 0;
uint32_t regC = 0;
uint8_t valid = 0;
volatile uint8_t operation = 0;
volatile uint8_t execute = 0;

ISR(INT0_vect) {
	EIMSK &= ~0b00000001;
	if(shift_state == 1){
		reg <<= 1;
		update_reg();
		EIMSK |= 0b00000001;
		} else {
		operation = 4;
		execute = 0;
		regB = reg;
		IS31_clear();
		view = 10;
		head = 10;
		reg = 0;
	}
}

int main(void)
{
	TWI_Setup();
	EICRA = 0b00000000;
	EIMSK = 0b00000001;
	sei();
	IS31_begin();
	IS31_setMaxCurrent(11);
	IS31_setBrightness(128);
	key_begin();
	IS31_setBase(1);
	
	while (1)
	{
		while(key_scan() == -1);
		TWI_Reset();
		
		uint8_t key_input= key_scan();
		
		switch(key_input){
			case 10:
			break;
			
			case 11:
			if(execute != 1){
				regC = reg;
				execute = 1;
			}
			switch(operation){
				case 0:
				break;
				
				case 1: // Add
				regB += regC;
				break;
				
				case 2: // Multiply
				regB *= regC;
				break;
				
				case 3: // Subtract
				if(regB > regC){
					regB -= regC;
					} else {
					regB = 0;
				}
				break;
				
				case 4:
				regB /= regC;
				EIMSK = 0b00000001;
				break;
				
				case 5: // Bitwise AND
				regB &= regC;
				break;
				
				case 6: // Bitwise OR
				regB |= regC;
				break;
				
				case 7: // Bitwise XOR
				regB ^= regC;
				break;
				
				case 8: // Modulo
				regB %= regC;
				break;
				
				default:
				break;
			}
			reg = regB;
			update_reg();
			EIMSK = 0b00000001;
			break;
			
			case 12:
			operation = 1;
			execute = 0;
			regB = reg;
			IS31_clear();
			view = 10;
			head = 10;
			reg = 0;
			break;
			
			case 13:
			if(shift_state == 1){
				reg >>= 1;
				update_reg();
				} else {
				operation = 2;
				execute = 0;
				regB = reg;
				IS31_clear();
				view = 10;
				head = 10;
				reg = 0;
			}
			break;
			
			case 14:
			if(shift_state == 1){
				uint8_t mx = 0;
				uint32_t scanner = 1;
				for(uint8_t bt = 0; bt < 32 ; bt++){
					if(reg & (scanner<<bt)){
						mx = bt;
					}
				}
				mx++;
				scanner = 1;
				while(mx < 32){
					reg |= (scanner<<mx);
					mx++;
				}
				reg = ~reg;
				update_reg();
				} else {
				operation = 3;
				execute = 0;
				regB = reg;
				IS31_clear();
				view = 10;
				head = 10;
				reg = 0;
			}
			break;
			
			case 15:
			if(shift_state == 0){
			IS31_clear();
			view = 10;
			IS31_Update(view);
			head = 10;
			reg = 0;
			regB = 0;
			regC = 0;
			execute = 0;
			operation = 0;
			EIMSK = 0b00000001;
			} else {
				operation = 8;
				execute = 0;
				regB = reg;
				IS31_clear();
				view = 10;
				head = 10;
				reg = 0;
			}
			break;
			
			case 16:
			if(view<129 && (view + 40) < head){
				for(uint8_t i = 0; i < 5; i++){
					view++;
					IS31_Update(view);
					_delay_ms(10);
				}
			}
			break;
			
			case 17:
			IS31_setBase(base_state == 3? 1 : base_state + 1);
			update_reg();
			break;
			
			case 18:
			IS31_shift(!shift_state);
			break;
			
			case 19:
			if(view>=15){
				for(uint8_t i = 0; i < 5; i++){
					view--;
					IS31_Update(view);
					_delay_ms(10);
				}
			}
			break;
			
			default:
			valid = 0;
			if(shift_state == 1 && key_input > 6 ){
				switch(key_input){
					case 7:
					operation = 5;
					execute = 0;
					regB = reg;
					IS31_clear();
					view = 10;
					head = 10;
					reg = 0;
					break;
					
					case 8:
					operation = 6;
					execute = 0;
					regB = reg;
					IS31_clear();
					view = 10;
					head = 10;
					reg = 0;
					break;
					
					case 9:
					operation = 7;
					execute = 0;
					regB = reg;
					IS31_clear();
					view = 10;
					head = 10;
					reg = 0;
					break;
					
					default:
					break;
				}
			} else {
				switch(base_state){
					case 1:
					if(shift_state == 0 || key_input == 0){
						reg = (reg * 10) + key_input;
						IS31_drawChar(5, key_input);
						valid = 1;
					}
					break;
					
					case 2:
					if(shift_state == 0 || key_input == 0){
						reg = (reg * 16) + key_input;
						IS31_drawChar(5, key_input);
						valid = 1;
						} else if(key_input <= 6){
						reg = (reg * 16) + (key_input + 9);
						IS31_drawChar(5, key_input + 9);
						valid = 1;
					}
					break;
					
					case 3:
					if(key_input <= 1){
						reg = (reg * 2) + key_input;
						IS31_drawChar(5, key_input);
						valid = 1;
					}
					break;
				}
			}
			
			if(valid == 1){
				for(uint8_t i = 0; i < 5; i++){
					head++;
					IS31_scrollLeft();
					IS31_Update(view);
					_delay_ms(10);
				}
			}
			break;
		}
		
		while(key_scan() != -1);
	}
}

void update_reg(){
	for(uint8_t i = 0; i < 7; i++){
		IS31_scrollUp();
		IS31_Update(view);
		_delay_ms(10);
	}
	
	view = 10;
	if(reg != 0){
		for(uint8_t j = 0; j < 7; j++){
			head = 10;
			IS31_clear();
			uint32_t reg_temp = reg;
			uint32_t divisor = 1;
			uint8_t base = 0;
			switch(base_state){
				case 1:
				base = 10;
				break;
				
				case 2:
				base = 16;
				break;
				
				case 3:
				base = 2;
				break;
				
				default:
				break;
			}
			while(divisor <= (reg / base)){
				divisor *= base;
			}
			while(divisor > 0){
				for(uint8_t i = 0; i < 5; i++){
					IS31_scrollLeft();
					head++;
				}
				IS31_drawChar(10, reg_temp / divisor);
				reg_temp %= divisor;
				divisor /= base;
			}
			for(uint8_t k = 0; k < 6 - j; k++){
				IS31_scrollDown();
			}
			IS31_Update(view);
		}
	}
}