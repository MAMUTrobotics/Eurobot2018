#ifndef UART_H_
#define UART_H_

#define BAUD 38400UL      // Baudrate
#define F_CPU 16000000UL  

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdio.h>
#define PUFFER_GROESSE 50   //Puffergröße in Byte

#ifdef __cplusplus
extern "C" {
#endif

void usart_init_intr();
//=============================================================
//Funktionen für den interrupt-gesteuerten Datenempfang
//=============================================================
uint8_t usart_byte_avail_intr(void);
//--------------------------------------------------------------
uint8_t usart_string_avail_intr(void);
//--------------------------------------------------------------
uint8_t usart_getc_intr(void);
//--------------------------------------------------------------
uint8_t usart_gets_intr(char *s);

//=============================================================
//Die nachfolgenden Sendefunktionen sind nicht interrupt-gesteuert
//=============================================================
void usart_putc(uint8_t byte);
//--------------------------------------------------------------
void usart_puts(char *s);
//---------------------------------------------------------------
void usart_puti(int zahl, int sges);
//--------------------------------------------------------------
void usart_putf(float zahl, int sges, int snach);
//--------------------------------------------------------------
void usart_putui(unsigned int zahl, int sges);
//--------------------------------------------------------------
void usart_putui_0(unsigned int zahl, int sges);
//--------------------------------------------------------------
void usart_putui_hex(unsigned int zahl, int sges);
//--------------------------------------------------------------
void usart_putui_bin(unsigned int zahl, int sges);
//--------------------------------------------------------------
void usart_putb(uint8_t a);

int uart_putchar(char c, FILE *stream);

FILE* uart_file_stream();

#ifdef __cplusplus
}
#endif




#endif