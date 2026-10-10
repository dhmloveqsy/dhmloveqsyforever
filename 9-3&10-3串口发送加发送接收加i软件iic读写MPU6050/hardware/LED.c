#include "LED.h"               // Device header

void LED_Init(void) 					//初始化LED
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_2;//选中1和2两个端口
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
	
	
}
void LED1_Turn(void)//实现电平翻转 用于输出
{	
	
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_1)==0)
	{
	LED1_OFF;
	}else{
		 LED1_ON;
}
	}
	void LED2_Turn(void)//实现电平翻转 用于输出
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_2)==0)
	{
	LED2_OFF;
	}else{
		 LED2_ON;
}
	}

void led_task(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_1)==1)
	{
		LED1_ON;
	}
	else
	{
		LED1_OFF;
	}
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_2)==0)
	{
		LED2_OFF;
	}
	else
	{
		LED2_ON;
	}	

}		





