#include "stm32f10x.h"   // Device header
#include "Lpin.h"
#include "key.h"
void Lmotor_init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStucture;
	GPIO_InitStucture.GPIO_Mode= GPIO_Mode_AF_PP;
 	GPIO_InitStucture.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStucture.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStucture);
	
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision= TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode= TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period= 10000-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler= 72-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter= 0 ;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	
	TIM_OCInitTypeDef TIM_OC4InitStucture;
	TIM_OCStructInit(&TIM_OC4InitStucture);
	TIM_OC4InitStucture.TIM_OCMode=TIM_OCMode_PWM1 ;
	TIM_OC4InitStucture.TIM_OCPolarity= TIM_OCPolarity_High;
	TIM_OC4InitStucture.TIM_OutputState= TIM_OutputState_Enable;
	TIM_OC4InitStucture.TIM_Pulse=0 ;
	TIM_OC4Init(TIM2,&TIM_OC4InitStucture);
	
	TIM_Cmd(TIM2,ENABLE );
	
	
}
uint32_t  speed=0;
uint32_t  speed_1=0;
void Motor_start()
{
	uint8_t temp=0;
	temp=key_GetNum();
	
	if(temp==1)
	{
		Motor_rset();
		speed+=1000;
		if(speed<=10000)
		TIM_SetCompare4(TIM2,speed);
		else if(speed>10000)
		{speed=0;TIM_SetCompare4(TIM2,speed);}
	}
	if(temp==2)
	{
		Motor_lset();
		speed_1+=1000;
		if(speed_1<=10000)
		TIM_SetCompare4(TIM2,speed_1);
		else if(speed_1>10000)
			{speed_1=0;TIM_SetCompare4(TIM2,speed_1);}
	}
	
}



