//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <stdlib.h>
//#define PUFFER_GROESSE 50   //Puffergröße in Byte
#include "uart.h"

unsigned char puffer[PUFFER_GROESSE];
unsigned char* lesezeiger;
unsigned char* schreibzeiger;

FILE uart_stdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

//=======================================================================
//Serielle Schnittstelle mit 8 Bit, no parity, 1 Stoppbit  initialisieren
//Den Datenempfang erledigt eine Interrupt-Service-Routine.
//=======================================================================
void usart_init_intr(){
//void usart_init_intr(uint16_t baud){
/* 
  UART-Init: 
Berechnung des Wertes für das Baudratenregister 
aus Taktrate und gewünschter Baudrate
*/
 
#ifndef F_CPU
/* In neueren Version der WinAVR/Mfile Makefile-Vorlage kann
   F_CPU im Makefile definiert werden, eine nochmalige Definition
   hier wuerde zu einer Compilerwarnung fuehren. Daher "Schutz" durch
   #ifndef/#endif 
 
   Dieser "Schutz" kann zu Debugsessions führen, wenn AVRStudio 
   verwendet wird und dort eine andere, nicht zur Hardware passende 
   Taktrate eingestellt ist: Dann wird die folgende Definition 
   nicht verwendet, sondern stattdessen der Defaultwert (8 MHz?) 
   von AVRStudio - daher Ausgabe einer Warnung falls F_CPU
   noch nicht definiert: */
#warning "F_CPU war noch nicht definiert, wird nun nachgeholt mit 8000000"
#define F_CPU 8000000UL  // Systemtakt in Hz - Definition als unsigned long beachten 
                         // Ohne ergeben sich unten Fehler in der Berechnung
#endif
 
//#define BAUD 38400UL      // Baudrate
 
// Berechnungen
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.
 
#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch! 
#endif



	//const uint32_t fosz=16000000L;
	//uint16_t baud_wert;

	lesezeiger=puffer;
	schreibzeiger=puffer;



	////Baudrate einstellen
	//baud_wert=fosz/(baud*16L)-1;
	//UBRRH=(uint8_t)(baud_wert>>8);
	//UBRRL=(uint8_t)baud_wert;
	 UBRR0H = UBRR_VAL >> 8;
	 UBRR0L = UBRR_VAL & 0xFF;

	//8N1 Daten
	UCSR0C|=(1<<UCSZ00)|(1<<UCSZ01);
	//Empfangen und Senden und Empfangsinterrupt aktivieren
	UCSR0B=(1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
}

//=============================================================
// Interrupt-Service-Routine
//=============================================================
ISR (USART_RX_vect){

	//Automatisch empfangene Daten in den Puffer schreiben:

	*schreibzeiger=UDR0;
	schreibzeiger++;
	if(schreibzeiger==puffer+PUFFER_GROESSE) schreibzeiger=puffer;

}
//=============================================================
//Funktionen für den interrupt-gesteuerten Datenempfang
//=============================================================
uint8_t usart_byte_avail_intr(void){
	//Prüft, ob ein Datenbyte im Puffer vorhanden ist
	// 1 = vorhanden, 0 = nicht vorhanden
	_delay_ms(10);
	//warten10ms();
	if(schreibzeiger!=lesezeiger)
	return 1;
	else return 0;
}
//--------------------------------------------------------------
uint8_t usart_string_avail_intr(void){
	//Hilfsfunktion für die Funktion usart_gets_intr
	//Prüfen, ob ein kompletter String im Puffer verfügbar ist.
	//Rückgabewert 1 = verfügbar, 0 = nicht verfügbar
	uint8_t avail=0;
	uint8_t *r=lesezeiger;
	while(r!=schreibzeiger){
		if(*r==0) avail=1;
		r++; if(r==puffer+PUFFER_GROESSE) r=puffer;
	}
	return avail;
}
//--------------------------------------------------------------
uint8_t usart_getc_intr(void){
	//Liest ein Byte aus dem Puffer
	//Nur aufrufen, wenn usart_byte_avail_intr() vorher aufgerufen wurde und
	//eine 1 lieferte.
	uint8_t datenbyte=0;

	if(schreibzeiger!=lesezeiger){

		datenbyte=*lesezeiger;
		lesezeiger++;
		if(lesezeiger==puffer+PUFFER_GROESSE) lesezeiger=puffer;
		
	}
	return datenbyte;
}
//--------------------------------------------------------------
uint8_t usart_gets_intr(char *s){
	//Liest  eine Zeichenkette aus dem Puffer
	//einschließlich Endmarke 0 in die Stringvariable s
	//Wenn der Rückgabewert ok = 1 ist, war das Lesen erfolgreich,
	//bei ok = 0 nicht erfolgreich.
	char c;
	uint8_t ok=0;
	if(usart_string_avail_intr()){
		ok=1;
		do{
			c = usart_getc_intr();
			*s=c;
			s++;
		}while(c!=0);
	}
	return ok;
}
//
//Hinweise zum Empfang Zahlen (int, long, foat und double)
//--------------------------------------------------------
// Zahlen werden vor dem Senden in Zeichenketten umgewandelt.
// Sie müssen deshalb mit der obigen Funktion usart_gets_intr als
// Zeichenketten empfangen werden.
//
// Die Rückumwandung dieser Zeichenkette in Zahlen kann dann mit
// den Funktionen aus <stdlib.h> erfolgen:
//
//    int    atoi(const char* s);    => Umwandlung in int
//    long   atol(const char* s);    => Umwandlung in long
//    double atof(const char* s);    => Umwandlung in double oder float
//
//=============================================================
//Die nachfolgenden Sendefunktionen sind nicht interrupt-gesteuert
//=============================================================
void usart_putc(uint8_t byte){
	//Ein Byte senden
	while(!(UCSR0A&(1<<UDRE0)));//warten auf Datenregister empty
	UDR0=byte;
}
//--------------------------------------------------------------
void usart_puts(char *s)
//Einen String mit Endmarke 0 senden
{

	while (*s!=0) {
		usart_putc(*s);
		s++;
	}
	//usart_putc(0); //Endmarke 0 wird übertragen!
}
//---------------------------------------------------------------
void usart_puti(int zahl, int sges){
	//Senden der Integerzahl zahl als Zeichenkette formatiert  mit sges Stellen
	//als Zeichenkette
	char buffer[16];
	uint8_t l=0,n;
	char *z=buffer;
	itoa(zahl,buffer,10);
	while(*z!=0){l++; z++;}//Bufferlänge l
	for(n=l;n<sges;n++) usart_putc(' ');
	usart_puts(buffer);
}
//--------------------------------------------------------------
void usart_putf(float zahl, int sges, int snach){
	//Senden einer Fließkommazahl als Zeichenkette mit sges Gesamtstellen
	//als Zeichenkette
	//Hiervon sind snach Nachkommastellen.
	//Die Nachkollastellen werden gerundet.
	char buffer[16];
	dtostrf(zahl,sges,snach,buffer);
	usart_puts(buffer);
}
//--------------------------------------------------------------
void usart_putui(unsigned int zahl, int sges){
	//Senden der Integerzahl zahl als Zeichenkette formatiert  mit sges Stellen
	//als Zeichenkette
	char buffer[16];
	uint8_t l=0,n;
	char *z=buffer;
	utoa(zahl,buffer,10);
	while(*z!=0){l++; z++;}//Bufferlänge l
	for(n=l;n<sges;n++) usart_putc(' ');
	usart_puts(buffer);
}
//--------------------------------------------------------------
void usart_putui_0(unsigned int zahl, int sges){
	//Senden der Integerzahl zahl als Zeichenkette formatiert  mit sges Stellen
	//als zeichekette
	//Leerstellen werden mit 0 aufgefüllt (TSC)
	char buffer[16];
	uint8_t l=0,n;
	char *z=buffer;

	utoa(zahl,buffer,10);
	while(*z!=0){l++; z++;}//Bufferlänge l
	for(n=l;n<sges;n++) usart_putc('0');

	usart_puts(buffer);
}
//--------------------------------------------------------------
void usart_putui_hex(unsigned int zahl, int sges){
	//Senden der nur-positiven Integerzahl zahl als Zeichenkette formatiert  mit sges Stellen
	// das Ausgabeformat ist hex (TSC)
	char buffer[17];
	uint8_t l=0,n;
	char *z=buffer;
	utoa(zahl,buffer,16);
	while(*z!=0){l++; z++;}//Bufferlänge l
	for(n=l;n<sges;n++) usart_putc(' ');
	usart_puts(buffer);
}
//--------------------------------------------------------------
void usart_putui_bin(unsigned int zahl, int sges){
	//Senden der Integerzahl zahl  als Zeichenkette formatiert  mit sges Stellen
	// Das Ausgabeformat ist binär. Leerstellen werden mit 0 aufgefüllt.
	char buffer[17];
	uint8_t l=0,n;
	char *z=buffer;
	utoa(zahl,buffer,2);
	while(*z!=0){l++; z++;}//Bufferlänge l
	for(n=l;n<sges;n++) usart_putc('0');
	usart_puts(buffer);
}
//--------------------------------------------------------------

void usart_putb(uint8_t a){
	//Senden einer 8-Bit-Zahl binär(TSC)
	usart_putui_bin(a,8);
}

int uart_putchar(char c, FILE *stream) // // bindet printf() ein
{
	usart_putc(c); //UART
	return 0;
}

FILE* uart_file_stream(){
	return &uart_stdout;
}