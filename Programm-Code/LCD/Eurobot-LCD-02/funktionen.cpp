#include "main.h"

volatile uint8_t flag_1ms;

void entprellen::update(){
	if((*p & (1 << b)) == 0 && counter <= 5) counter++;
	if((*p & (1 << b)) != 0 && counter != 0) counter--;
	if(counter == 0 && state != 0){ state = 0; changed_ = 1;}
	if(counter > 5 && state != 1){ state = 1; changed_ = 1;}
}
u8 entprellen::changed(){
	if(changed_){
		changed_ = 0;
		return 1;
		}else{
		return 0;
	}
}
u8 entprellen::rised(){
	if(changed_){
		changed_ = 0;
		if (state)
		{
			return 1;
		}else{
			return 0;
		}		
	}else{
		return 0;
	}
}
u8 entprellen::fallen(){
	if(changed_){
		changed_ = 0;
		if (state)
		{
			return 0;
		}else{
			return 1;
		}		
	}else{
		return 0;
	}
}
entprellen::entprellen(volatile uint8_t *pin, uint8_t bit):p(pin),b(bit){
	//Initialisierung
	volatile u8* reg;
	reg = pin;
	reg++;				//Pointer von Pin auf DDR verschieben
	*reg &= ~(1 << bit);//Pin als Eingang einstellen
	reg++;				//Pointer von DDR auf Port verschieben
	*reg |= (1 << bit);	//Pull-up aktivieren
	changed_ = 0;
}
	
void Timer_io_init(void)  
{  
  // Table 39 im Atmega8 Datenblatt  
  // Waveform generation mode:   
  // Mode 9: PWM Phase and frequency correct  
  TCCR1A |= (0 << WGM11) | (1 << WGM10);  
  TCCR1B |= (1 << WGM13) | (0 << WGM12);  
   
  // Table 40 im Atmega8 Datenblatt  
  // Prescaler: clk/256
  // TCCR1B |= (1 << CS12)  | (0 << CS11)  | (0 << CS10);
  // Prescaler: clk/64
  TCCR1B |= (0 << CS12)  | (1 << CS11)  | (1 << CS10);
  // 16MHz / 256 = 62500
  // 16MHz / 64  = 250000
  /* 
    Wunsch: ca. 1 Hz Blinken 
    1 Hz genau - siehe: http://www.mikrocontroller.net/articles/AVR_-_Die_genaue_Sekunde_/_RTC 
  
    Setzen von TOP (OCR1A) = Anzahl der Timer1-Takte von IRQ zu IRQ 
    Figure 40 im Atmega8 Datenblatt 
    1. /2 wegen 2 Halbperioden pro Sekunde bei 1 Hz 
    2. /2 wegen Runterzählen von TOP (=OCR1A) nach BOTTOM (=0)  
       und Hochzählen von BOTTOM (=0) bis TOP (=OCR1A) zwischen  
       den Interrupts 
  */  
   
  //#define DELAY (3125)//F_CPU /256 /2 / 10 = 0,1s
  #define DELAY (125)//F_CPU /64 /2 / 1000 = 1ms
  OCR1A = DELAY;   
  TCNT1 = - DELAY; // Anlauf für 1. Interrupt verlängern  
   
  TIMSK1 = (1 << OCIE1A);  
} 
ISR (TIMER1_COMPA_vect)
{
	flag_1ms = 1;
}