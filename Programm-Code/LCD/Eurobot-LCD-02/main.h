/*
 * main.h
 *
 * Created: 11.02.2018 01:14:07
 *  Author: Nils
 */ 


#ifndef MAIN_H_
#define MAIN_H_
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/wdt.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/eeprom.h>
#include "stlcd.h"
#include "glcd.h"
#include "icon.h"
#include "uart.h"
#include "mydefs.h"
#include "twislave.h"
#include "num_font.h"

//###################### Slave-Adresse
#define SLAVE_ADRESSE 0x29 								// Slave-Adresse

//###################### Macros
#define uniq(LOW,HEIGHT)	((HEIGHT << 8)|LOW)			// 2x 8Bit 	--> 16Bit
#define LOW_BYTE(x)        	(x & 0xff)					// 16Bit 	--> 8Bit
#define HIGH_BYTE(x)       	((x >> 8) & 0xff)			// 16Bit 	--> 8Bit


#define sbi(ADDRESS,BIT) 	((ADDRESS) |= (1<<(BIT)))	// set Bit
#define cbi(ADDRESS,BIT) 	((ADDRESS) &= ~(1<<(BIT)))	// clear Bit
#define	toggle(ADDRESS,BIT)	((ADDRESS) ^= (1<<BIT))		// Bit umschalten

#define	bis(ADDRESS,BIT)	(ADDRESS & (1<<BIT))		// bit is set?
#define	bic(ADDRESS,BIT)	(!(ADDRESS & (1<<BIT)))		// bit is clear?
//######################

#define	BLA_OUT	SBIT( PORTC, 0 )
#define	BLA_DDR	SBIT( DDRC,  0 )

#define SID_DDR DDRB
#define SID_PIN PINB
#define SID_PORT PORTB
#define SID 3

#define SCLK_DDR DDRB
#define SCLK_PIN PINB
#define SCLK_PORT PORTB
#define SCLK 5

#define A0_DDR DDRB
#define A0_PIN PINB
#define A0_PORT PORTB
#define A0 0

#define RST_DDR DDRB
#define RST_PIN PINB
#define RST_PORT PORTB
#define RST 1

#define CS_DDR DDRB
#define CS_PIN PINB
#define CS_PORT PORTB
#define CS 2


extern volatile uint8_t flag_1ms;
#ifdef __cplusplus

class entprellen{
	private:
	volatile u8* p;
	const u8 b;
	u8 counter;
	u8 changed_;
	public:
	u8 state;
	void update();
	u8 changed();
	u8 rised();
	u8 fallen();
	entprellen(volatile uint8_t *pin, uint8_t bit);
};

enum var_type { v_u_char, v_s_char, v_u_int, v_s_int, v_bool, v_team, v_u_char_ro, v_s_char_ro, v_bool_ro };
struct var {
  const var_type type;
  char value;
  var(var_type type):type(type){}
} ;

#endif
void Timer_io_init(void);
ISR (TIMER1_COMPA_vect);


#endif /* MAIN_H_ */