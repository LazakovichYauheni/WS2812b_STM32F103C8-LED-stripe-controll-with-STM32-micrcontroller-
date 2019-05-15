//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
/*
#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "main.h"
#include "delay\delay.h"
#include "ws2812\ws2812.h"
#include "ws2812\led_examples.h"


void main()
{
  uint8_t EffectNum = 0;
  
  SystemInit();

  ws2812_init();

  while (1)
  {
    uint16_t Period;
    uint16_t TickNum = led_effect_start(EffectNum, &Period);
    while (TickNum--)
    {    
      led_effect_tick(EffectNum);
      ws2812_update();

      delay_ms(Period);
    }
    

    if (++EffectNum == LIGHT_EFFECTS_NUM)
      EffectNum = 0;
  }
}
*/
