#ifndef __LED_H__
#define __LED_H__
#include "stm32f10x.h"
#define   LED1_ON GPIO_ResetBits(GPIOA,GPIO_Pin_1)//宏定义
#define   LED1_OFF GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define   LED2_ON GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define   LED2_OFF GPIO_SetBits(GPIOA,GPIO_Pin_2)
void LED_Init(void);
void LED1_Turn(void);
void LED2_Turn(void);
void led_task(void);
#endif




