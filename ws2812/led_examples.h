//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef _LED_EXAMPLES_H
#define _LES_EXAMPLES_H

#include "..\types.h"

#define LIGHT_EFFECTS_NUM               9

// Настройки анимации SequentalLights
#define SequentalLights_LedNum          20
#define SequentalLights_Period          20
#define SequentalLights_CycleNum        15

// Настройки анимации Радуги
#define Rainbow_ColorLength             30
#define Rainbow_FadeStep                8

typedef struct
{
  uint8_t G;
  uint8_t R;
  uint8_t B;
} tRGB;

uint16_t led_effect_start(uint8_t EffectNum, uint16_t *pPeriod);
void led_effect_tick(uint8_t EffectNum);

#endif
