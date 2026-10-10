#include "stm32f10x.h"                  // Device header

void LED_Init(void) 					//初始化LED
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_2;//选中1和2两个端口
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_2);
	
}
#define  A LED1_ON//宏定义
#define  B LED1_OFF
#define  C LED2_ON
#define D LED2_OFF

void A(void)//直接写A()就调用了{}内的内容
{  GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}
void B(void)
{	GPIO_SetBits(GPIOA,GPIO_Pin_1);
}
void LED1_Turn(void)//实现电平翻转 用于输出
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_1)==0)
	{
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	}else{
		 GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}
	}
void C(void)
{  GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}
void D(void)
{	GPIO_SetBits(GPIOA,GPIO_Pin_2);
}
	void LED2_Turn(void)//实现电平翻转 用于输出
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_2)==0)
	{
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
	}else{
		 GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}
	}