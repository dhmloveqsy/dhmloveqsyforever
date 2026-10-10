#include "stm32f10x.h"                 // Device header
void PWM_Init()
{
	//开启时钟 选择时钟源 配置时基单元
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE );
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE );
	GPIO_InitTypeDef GPIO_InitStucture;
	GPIO_InitStucture.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStucture.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStucture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStucture);
	
	
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStucture;
	TIM_TimeBaseInitStucture.TIM_ClockDivision=TIM_CKD_DIV1 ;
	TIM_TimeBaseInitStucture.TIM_CounterMode= TIM_CounterMode_Up ;
	TIM_TimeBaseInitStucture.TIM_Period= 100-1;
	TIM_TimeBaseInitStucture.TIM_Prescaler= 720-1;
	TIM_TimeBaseInitStucture.TIM_RepetitionCounter= 0;
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStucture);
	//配置输出比较单元
	TIM_OCInitTypeDef TIM_OC2InitStructure;
	TIM_OCStructInit(&TIM_OC2InitStructure);
	TIM_OC2InitStructure.TIM_OCMode=TIM_OCMode_PWM1 ;
	TIM_OC2InitStructure.TIM_OCPolarity= TIM_OCPolarity_Low;
	TIM_OC2InitStructure.TIM_OutputState= TIM_OutputState_Enable;
	TIM_OC2InitStructure.TIM_Pulse= 0;
	
	TIM_OC2Init(TIM2,&TIM_OC2InitStructure);
	
	
	
	
	TIM_Cmd(TIM2,ENABLE );
}
void PWM_SET(uint32_t num)
{
	TIM_SetCompare2(TIM2,num);
	
}

