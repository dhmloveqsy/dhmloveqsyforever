#include "stm32f10x.h"                  // Device header
void encoder_init()
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init (GPIOA,&GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
//  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init (GPIOA,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource7);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);
	
	EXTI_InitTypeDef  EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line= EXTI_Line8|EXTI_Line7;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE ;
	EXTI_InitStructure.EXTI_Mode= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
//	EXTI_InitTypeDef  EXTI_InitStructure;
//	EXTI_InitStructure.EXTI_Line= EXTI_Line8;
//	EXTI_InitStructure.EXTI_LineCmd=ENABLE ;
//	EXTI_InitStructure.EXTI_Mode= EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Falling;
//	EXTI_Init(&EXTI_InitStructure);
	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 2;
	NVIC_Init(&NVIC_InitStructure);
//	NVIC_InitTypeDef  NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE ;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0;
//	NVIC_Init(&NVIC_InitStructure);
}
int32_t   date_1=0;
uint32_t  num=0;
void EXTI9_5_IRQHandler(void)
{
	//中断9产生的中断标志位（用于中断函数） 
	if(EXTI_GetITStatus(EXTI_Line9)==SET)
	{
		//清除中断位
		EXTI_ClearITPendingBit(EXTI_Line9);
		//记录中断发生次数
		num=num+1;
		
	}
	if(EXTI_GetITStatus(EXTI_Line8)==SET )
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0)
		{
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0)
			date_1+=1;
			
			
		}
	EXTI_ClearITPendingBit(EXTI_Line8);
		
	}
	if(EXTI_GetITStatus(EXTI_Line7)==SET )
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0)
		{
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0)
			date_1-=1;
			
			
		}
	EXTI_ClearITPendingBit(EXTI_Line7);
		
	}
}

