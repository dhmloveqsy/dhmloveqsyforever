#ifndef _mydefine_H
#define _mydefine_H
#include "main.h"
#include "schduler.h"
#include "usart.h"
#include "gpio.h"
#include "spi.h"
#include "usart_app.h"
#include "led_app.h"
#include "key_app.h"
#include "dma.h"
#include "dac.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "ringbuffer.h"
#include "ebtn.h"
#include "u8g2.h" 
#include "gd25qxx.h"
#include "flash_app.h"
#include "WouoUI_user.h" 
#include "i2c.h"
#include "tim.h"
#include "rtc.h"
#include "adc_app.h"
#include "dac_app.h"
#include "btn_app.h"
#include "oled.h"
#include "oled_app.h"
#include "u8g2_app.h"
#include "rtc_app.h"
extern uint8_t ucled[6];
extern struct rt_ringbuffer uart_ringbuffer; 
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern uint8_t ring_buffer[256];
extern uint8_t uart_rx_dma_buffer[128];
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern __IO uint32_t adc_value;
extern __IO float voltage;
extern TIM_HandleTypeDef htim3;
extern DAC_HandleTypeDef hdac;
extern DMA_HandleTypeDef hdma_dac1;
extern u8g2_t u8g2; 
extern uint32_t flash_id;
extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef sDate;
extern char msg[64];
extern RTC_HandleTypeDef hrtc;
#endif






