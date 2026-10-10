#include "stm32f10x.h"                  // Device header
void buzzer_Init(void) 					//初始化LED
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;//选中1和2两个端口
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	
}
#define  A buzzer_ON//宏定义
#define  B buzzer_OFF


void A(void)//直接写A()就调用了{}内的内容
{  GPIO_ResetBits(GPIOB,GPIO_Pin_13);
}
void B(void)
{	GPIO_SetBits(GPIOB,GPIO_Pin_13);
}
void buzzer_Turn(void)//实现电平翻转 用于输出
{
	if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_13)==0)
	{
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	}else{
		 GPIO_ResetBits(GPIOB,GPIO_Pin_13);
}
	}