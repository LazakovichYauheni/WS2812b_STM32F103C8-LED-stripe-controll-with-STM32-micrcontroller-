//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "..\delay\delay.h"
#include "ws2812.h"


uint8_t ws2812_buff[WS2812_LEDNUM * 3];
int16_t DMA_buf[WS2812_LEDNUM + 2][WS2812_COLORSNUM][8];


//==============================================================================
// Процедура настраивает ножку GPIO, к которой подключена цепочка ws2812
//==============================================================================
void GPIO_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    
  GPIO_Init(GPIOB, &GPIO_InitStruct);
}
//==============================================================================


//==============================================================================
// Timer4 генерирует импульсы на светодиодную полосу
// Канал 4 таймера используется в режиме Compare с загрузкой по DMA регистра CCR3 для формирования битовых сигналов ws2812
//==============================================================================
void Timer4_init(void)
{
  TIM_TypeDef *tim = TIM4;
  
  // Включаем тактирование таймера
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
  // Устанавливаем основные параметры тактирования таймера
  TIM_TimeBaseInitTypeDef TBInitStruct;
  TBInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TBInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TBInitStruct.TIM_Period = 90 - 1;       // 72 МГц / 0.8 МГц = 90 // Перегрузка таймера каждые 1.25 мкс
  TBInitStruct.TIM_Prescaler = 0;         // Без предделителя
  TIM_TimeBaseInit(tim, &TBInitStruct);

  // Настраиваем режим работы таймера
  TIM_OCInitTypeDef OCInitStruct;
  OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
  OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  OCInitStruct.TIM_Pulse = 0;
  TIM_OC3Init(tim, &OCInitStruct);
  
  TIM_OC3PreloadConfig(tim, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(tim, ENABLE);

  TIM_SetCounter(tim, 0);
  
  TIM_DMACmd(tim, TIM_DMA_CC3, ENABLE);         // Разрешаем запросы DMA
  TIM_Cmd(tim, ENABLE);                         // Запускаем таймер               
}
//==============================================================================


//==============================================================================
// Процедура настраивает DMA1 для загрузки значений в регистр CCR3 таймера4
//==============================================================================
void DMA1_Stream7_Mem_to_TMR4_init(void)
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  DMA_Cmd(DMA1_Channel5, DISABLE);
    
  DMA_InitTypeDef DMA_InitStruct;
  DMA_StructInit(&DMA_InitStruct);
  
  DMA_InitStruct.DMA_BufferSize = (WS2812_LEDNUM + 2) * WS2812_COLORSNUM * 8;
  DMA_InitStruct.DMA_MemoryBaseAddr = (unsigned long)&DMA_buf;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;        // MSIZE[1:0]: Memory data size | 00: 8-bit. Memory data size
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;             // MINC: Memory increment mode | 1: Memory address pointer is incremented after each data transfer (increment is done according to MSIZE)
  DMA_InitStruct.DMA_PeripheralBaseAddr = (unsigned int)&(TIM4->CCR3) + 1;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;    // PSIZE[1:0]: Peripheral data size | 00: 8-bit. Peripheral data size
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;         // PINC: Peripheral increment mode | 0: Peripheral address pointer is fixed
  DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;  // PL[1:0]: Priority level | 11: Very high.  PL[1:0]: Priority level
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;       // DIR[1:0]: Data transfer direction | 01: Memory-to-peripheral
  DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;          // CIRC: Circular mode | 1: Circular mode enabled
  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    
  DMA_Init(DMA1_Channel5, &DMA_InitStruct);
  
  DMA_Cmd(DMA1_Channel5, ENABLE);
}
//==============================================================================


//==============================================================================
// Процедура инициализирует буферы
//==============================================================================
void ws2812_buff_init(void)
{
  int32_t i, j, k; 

  // Инициализация массива в буфере DMA значениями, соответствующими нулям
  for (i = 0; i < WS2812_LEDNUM; i++)
  {
    for (j = 0; j < WS2812_COLORSNUM; j++)
    {
      for (k = 0; k < 8; k++)
        DMA_buf[i][j][k] = LED_LEV0;
    }
  }

  // Сигнал сброса цепочки ws2812
  for (i = WS2812_LEDNUM; i < WS2812_LEDNUM + 2; i++)
  {
    for (j = 0; j < WS2812_COLORSNUM; j++)
    {
      for (k = 0; k < 8; k++)
        DMA_buf[i][j][k] = 0;
    }
  }
}
//==============================================================================


//==============================================================================
// Процедура инициализирует периферию микроконтроллера для работы с ws2812
//==============================================================================
void ws2812_init(void)
{
  GPIO_init();
  Timer4_init();
  DMA1_Stream7_Mem_to_TMR4_init();
  ws2812_buff_init();
}
//==============================================================================


//==============================================================================
// Процедура устанавливает RGB-значение в LED с индексом LED_Num
//==============================================================================
void ws2812_set_color(uint16_t LED_Num, uint8_t R, uint8_t G, uint8_t B)
{
  LED_Num *= 3;
  
  ws2812_buff[LED_Num] = G;
  ws2812_buff[LED_Num + 1] = R;
  ws2812_buff[LED_Num + 2] = B;
}
//==============================================================================
/*void led_off(void);
void*/

//==============================================================================
// Процедура реализует плавный розжиг всех светодиодов цепочки с заданным шагом
//==============================================================================
void ws2812_fade_in_all(uint16_t fade_step)
{
  for (uint16_t i = 0; i < (WS2812_LEDNUM * 3); i++)
  {
    if ((0xFF - ws2812_buff[i]) < fade_step)
      ws2812_buff[i] = 0xFF;
    else
      ws2812_buff[i] += fade_step;
  }
}
//==============================================================================


//==============================================================================
// Процедура реализует плавное гашение всех светодиодов цепочки с заданным шагом
//==============================================================================
void ws2812_fade_out_all(uint16_t fade_step)
{
  for (uint16_t i = 0; i < (WS2812_LEDNUM * 3); i++)
  {
    if (ws2812_buff[i] > fade_step)
      ws2812_buff[i] -= fade_step;
    else
      ws2812_buff[i] = 0;
  }
}
//==============================================================================


//==============================================================================
// Процедура гасит все светодиоды цепочки
//==============================================================================
void ws2812_all_off(void)
{
  for (uint16_t i = 0; i < (WS2812_LEDNUM*3); i++)
    ws2812_buff[i] = 0;
}
//==============================================================================


//==============================================================================
// Процедура обновляет состояние цепочки светодиодов
//==============================================================================
void ws2812_update(void)
{
  int32_t i, j, k;

  for (i = 0; i < WS2812_LEDNUM; i++)
  {
    for (j = 0; j < WS2812_COLORSNUM; j++)
    {
      for (k = 0; k < 8; k++)
          DMA_buf[i][j][k] = (ws2812_buff[i * 3 + j] & (1 << (7 - k))) ? LED_LEV1 : LED_LEV0;
    }
  }
}
//==============================================================================
