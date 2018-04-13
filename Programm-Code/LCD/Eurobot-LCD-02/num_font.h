#ifndef NUM_FONT_H_
#define NUM_FONT_H_
	#ifdef __cplusplus
	extern "C" {
	#endif
		#include <avr/pgmspace.h>

		void drawbitmap_local(uint8_t *buff, uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color);
		void draw_num_single(uint8_t *buff, uint8_t i, uint8_t pos);
		void draw_num(uint8_t *buff, uint16_t i);
		void draw_num_signed(uint8_t *buff, int16_t i);

		extern const uint8_t PROGMEM num_0[];
		extern const uint8_t PROGMEM num_1[];

		extern const uint8_t *num[];
	#ifdef __cplusplus
	}
	#endif
#endif




