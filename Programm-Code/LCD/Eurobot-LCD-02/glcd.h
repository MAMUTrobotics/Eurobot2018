#ifndef GLCD_H_
#define GLCD_H_

#define LCDWIDTH 128
#define LCDHEIGHT 64

#define swap(a, b) { uint8_t t = a; a = b; b = t; }
extern uint8_t cursor_x;
extern uint8_t cursor_y;

#ifdef __cplusplus
extern "C" {
#endif
void setpixel(uint8_t *buff, uint8_t x, uint8_t y, uint8_t color);

void drawrect(uint8_t *buff,
	      uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
	      uint8_t color);
void drawline(uint8_t *buff,
	      uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, 
	      uint8_t color);


void drawbitmap(uint8_t *buff, uint8_t x, uint8_t y, 
		const uint8_t bitmap, uint8_t w, uint8_t h,
		uint8_t color);
void drawchar(uint8_t *buff, uint8_t x, uint8_t line, uint8_t c);
void drawcircle(uint8_t *buff,
	      uint8_t x0, uint8_t y0, uint8_t r, 
		uint8_t color);

void fillrect(uint8_t *buff,
	      uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
	      uint8_t color);
		  
void fillcircle(uint8_t *buff, uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);
void drawstring(uint8_t *buff, uint8_t x, uint8_t line, uint8_t *c);

int lcd_putchar(char c, FILE *stream);

FILE* lcd_file_stream();
#ifdef __cplusplus
}
#endif
extern uint8_t* lcd_buffer;

#endif