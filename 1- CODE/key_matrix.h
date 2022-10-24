/*
 * key_matrix.h
 *
 * Created: 17/10/2022 17:15:01
 *  Author: youss
 */ 


#ifndef KEY_MATRIX_H_
#define KEY_MATRIX_H_

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define ZERO 15
#define ONE 5
#define TWO 13
#define THREE 21
#define FOUR 4
#define FIVE 12
#define SIX 20
#define SEVEN 2
#define EIGHT 10
#define NINE 18
#define BASE 8
#define LEFT 0
#define MULT 9
#define SHFT 16
#define MINUS 17
#define PM 23
#define RIGHT 24
#define CLR 25
#define PLUS 27
#define EQUALS 30



void key_begin();
int8_t key_scan();


#endif /* KEY_MATRIX_H_ */