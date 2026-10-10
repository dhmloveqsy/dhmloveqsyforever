#include "stm32f10x.h"                  // Device header
#include "key.h"
void Motor_init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE );
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef  GPIO_InitStucture;
	GPIO_InitStucture.GPIO_Mode=GPIO_Mode_AF_PP ;
	GPIO_InitStucture.GPIO_Pin= GPIO_Pin_2;
	GPIO_InitStucture.GPIO_Speed= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStucture);
	
	TIM_InternalClockConfig (TIM2);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStucture;
	TIM_TimeBaseInitStucture.TIM_ClockDivision= TIM_CKD_DIV1;
	TIM_TimeBaseInitStucture.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStucture.TIM_Period=20000-1 ;
	TIM_TimeBaseInitStucture.TIM_Prescaler=72-1 ;
	TIM_TimeBaseInitStucture.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStucture);
	
	TIM_OCInitTypeDef TIM_OC3InitStructure;
	TIM_OCStructInit(&TIM_OC3InitStructure);
	TIM_OC3InitStructure.TIM_OCMode=TIM_OCMode_PWM1 ;
	TIM_OC3InitStructure.TIM_OCPolarity= TIM_OCPolarity_High;
	TIM_OC3InitStructure.TIM_OutputState=TIM_OutputState_Enable ;
	TIM_OC3InitStructure.TIM_Pulse= 0;
	
	TIM_OC3Init(TIM2,&TIM_OC3InitStructure);
	
	TIM_Cmd(TIM2,ENABLE );
}
//void PWM_get(uint32_t  num)
//{
//	
//	TIM_SetIC3Prescaler(TIM2,num );
//	
//}

uint32_t t=500;
float  t_1=0;
uint16_t flag_1=0;
uint16_t flag_2=0;
void Motor_Start()
{
	uint8_t temp=0;
temp=key_GetNum();
	if(temp==1)
	{
		if(flag_1==0)
		{
			t=t+500;
		if(t>2500)
		{
			t=2500;
			flag_1=1;
			flag_2=1;
		}
	}
		if (flag_2)
		{
			t-=500;
			if(t==500)
			{
				flag_2=0;
			flag_1=0;
			}
		}
	
}
	if(t>=500)
	TIM_SetCompare3(TIM2,t);  
	t_1=(float )(t-500)/2000*180;
}