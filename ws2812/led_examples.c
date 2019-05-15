//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include "..\delay\delay.h"
#include "led_examples.h"
#include "ws2812.h"

// Основные цвета радуги
tRGB const RainbowColors[] =
{
  {0,   255, 0},        // Красный
  {165, 255, 0},        // Оранжевый
  {255, 255, 0},        // Жёлтый
  {255, 0,   0},        // Зелёный
  {255, 0,   255},      // Голубой
  {0,   0,   255},      // Синий
  {0,   255, 255},       // Фиолетовый
};

// Счётчики стадий алгоритмов
uint16_t AlgStep, AlgStep2, AlgStep3;

     
//==============================================================================
// Процедура с заданным шагом приводит значение *source к значению *desc
//==============================================================================
void StepChange(uint8_t *desc, uint8_t *source, uint8_t Step)
{
  if (*desc < *source)
  {
    if ((0xFF - *desc) < Step)
      *desc = 0xFF;
    else
      *desc += Step;
  }
    
  if (*desc > *source)
  {
    if (*desc > Step)
      *desc -= Step;
    else
      *desc = 0;
  }
}       
//==============================================================================


//==============================================================================
// Процедура с заданным шагом преобразует цвет pLEDsource в pLEDdesc
//==============================================================================
void StepChangeColor(tRGB *pLEDdesc, tRGB *pLEDsource, uint8_t Step)
{
  StepChange(&(pLEDdesc->R), &(pLEDsource->R), Step);
  StepChange(&(pLEDdesc->G), &(pLEDsource->G), Step);
  StepChange(&(pLEDdesc->B), &(pLEDsource->B), Step);
}
//==============================================================================


//==============================================================================
// Функция стартует новый эффект. Возвращает кол-во тактов в цикле эффекта.
// По указателю pPeriod записывает период в мс между тактами
//==============================================================================
uint16_t led_effect_start(uint8_t EffectNum, uint16_t *pPeriod)
{
  AlgStep = AlgStep2 = AlgStep3 = 0;
  
  switch (EffectNum)
  {
  // Белая бегущая точка с плавным fade-out
  case 0:
    *pPeriod = 0;
    return WS2812_LEDNUM*2; 
  // Красная бегущая точка с плавным fade-out
  case 1:
    *pPeriod = 0;
    return WS2812_LEDNUM*2; 
  // Зелёная бегущая точка с плавным fade-out
  case 2:
    *pPeriod = 0;
    return WS2812_LEDNUM*2; 
  // Синяя бегущая точка с плавным fade-out
  case 3:
    *pPeriod = 0;
    return WS2812_LEDNUM*2; 
  // Желтая бегущая точка с плавным fade-out
  case 4:
    *pPeriod = 0;
    return WS2812_LEDNUM*2; 
  // Голубая бегущая точка с плавным fade-out
  case 5:
    *pPeriod = 0;
    return WS2812_LEDNUM*2; 
  // Фиолетовая бегущая точка с плавным fade-out
  case 6:
    *pPeriod = 0;
    return WS2812_LEDNUM*2; 
  // Эффект Радуга
  case 7:
    *pPeriod = 10;
    return 500;
  // Эффект Sequental Lights
  case 8:
    ws2812_all_off();
    *pPeriod = SequentalLights_Period;
    return SequentalLights_LedNum * 2 * SequentalLights_CycleNum;
  case 9:
      *pPeriod = 0;
      return WS2812_LEDNUM*2;
  default:
    *pPeriod = 20;
    return 64;
  }
}
//==============================================================================


//==============================================================================
// Процедура, реализующая эффект с номером EffectNum. Должна вызываться с заданным периодом
//==============================================================================
void led_effect_tick(uint8_t EffectNum)
{
  tRGB *pLED = (tRGB*) ws2812_buff;

  switch(EffectNum)
  {
  // Белая бегущая точка с плавным fade-out
  case 0:
    ws2812_set_color(AlgStep / 2, 255, 255, 255);
    ws2812_fade_out_all(0);
    
    if (++AlgStep >= (WS2812_LEDNUM*2))
      AlgStep = 0;
    break;
  // Красная бегущая точка с плавным fade-out
  case 1:
    ws2812_set_color(AlgStep / 2, 255, 0, 0);
    ws2812_fade_out_all(4);
    

    if (++AlgStep >= (WS2812_LEDNUM*2))
      AlgStep = 0;
    break;
  // Зелёная бегущая точка с плавным fade-out
  case 2:
    ws2812_set_color(AlgStep / 2, 0, 255, 0);
    ws2812_fade_out_all(4);
    
    if (++AlgStep >= (WS2812_LEDNUM*2))
      AlgStep = 0;
    break;
  // Синяя бегущая точка с плавным fade-out
  case 3:
    ws2812_set_color(AlgStep / 2, 0, 0, 255);
    ws2812_fade_out_all(4);
    
    if (++AlgStep >= (WS2812_LEDNUM*2))
      AlgStep = 0;
    break;
  // Желтая бегущая точка с плавным fade-out
  case 4:
    ws2812_set_color(AlgStep / 2, 255, 255, 0);
    ws2812_fade_out_all(4);
    
    if (++AlgStep >= (WS2812_LEDNUM*2))
      AlgStep = 0;
    break;
  // Голубая бегущая точка с плавным fade-out
  case 5:
    ws2812_set_color(AlgStep / 2, 0, 255, 255);
    ws2812_fade_out_all(4);
    
    if (++AlgStep >= (WS2812_LEDNUM*2))
      AlgStep = 0;
    break;
  // Фиолетовая бегущая точка с плавным fade-out
  case 6:
    ws2812_set_color(AlgStep / 2, 255, 0, 255);
    ws2812_fade_out_all(4);
    
    if (++AlgStep >= (WS2812_LEDNUM*2))
      AlgStep = 0;
    break;
  // Эффект Радуга
  case 7:
    // Сдвиг массива пикселей вправо
    for (uint16_t i = WS2812_LEDNUM; i > 1; i--) 
    {
      pLED[i-1].R = pLED[i-2].R;
      pLED[i-1].G = pLED[i-2].G;
      pLED[i-1].B = pLED[i-2].B;
    }
    
    // Пора плавно менять цвет   
    if (++AlgStep2 > Rainbow_ColorLength)
    {
      AlgStep2 = 0;

      //loop colouers
      if (++AlgStep3 >= 7)
        AlgStep3 = 0;
    }
        
    // Плавная смена цвета 0-го пикселя с шагом не более Rainbow_FadeStep
    StepChangeColor(&(pLED[0]), (tRGB *) &(RainbowColors[AlgStep3]), Rainbow_FadeStep);
    
    if (++AlgStep >= 700)
      AlgStep = 0;
    break;
  // Эффект Sequental Lights
  case 8:
    if ((AlgStep % (SequentalLights_LedNum*2)) < SequentalLights_LedNum)
    {
      ws2812_set_color((AlgStep % (SequentalLights_LedNum*2)), 255, 112, 0);
      ws2812_fade_out_all(4);
    }
    else
      ws2812_all_off();
    
    if (++AlgStep >= SequentalLights_LedNum * 2 * SequentalLights_CycleNum)
      AlgStep = 0;
    break;
  case 9:
	  ws2812_fade_out_all(0);
  default:
    ws2812_fade_out_all(4);
    break;
  }
}
//==============================================================================
