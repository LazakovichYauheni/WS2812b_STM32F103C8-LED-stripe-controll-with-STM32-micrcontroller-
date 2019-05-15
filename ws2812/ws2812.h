//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef _WS2812_H
#define _WS2812_H

#include "..\types.h"

#define WS2812_LEDNUM           120
#define WS2812_COLORSNUM        3

#define LED_LEV0  30//15//3//4
#define LED_LEV1  60//30//7//6//9



extern uint8_t ws2812_buff[];


void ws2812_init(void);
void ws2812_sendarray(uint8_t *data, uint16_t datalen);
void ws2812_update(void);
void ws2812_set_color(uint16_t LED_Num, uint8_t R, uint8_t G, uint8_t B);
void ws2812_fade_in_all(uint16_t fade_step);
void ws2812_fade_out_all(uint16_t fade_step);
void ws2812_off(uint16_t fade_step);
void ws2812_all_off(void);


#endif
