#include "stm32f10x.h"                  // Device header
void Signal_init()
{
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3,ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	TIM_InternalClockConfig (TIM3);
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStucture;
	TIM_TimeBaseInitStucture.TIM_ClockDivision= TIM_CKD_DIV1;
	TIM_TimeBaseInitStucture.TIM_CounterMode=TIM_CounterMode_Up ;
	/*输入捕获的计时范围为了防止溢出直接设置为上限值*/
	TIM_TimeBaseInitStucture.TIM_Period=65536-1 ;
	/*标准频率为主频率除psc（预分频系数）的值*/
	TIM_TimeBaseInitStucture.TIM_Prescaler= 72-1;
	TIM_TimeBaseInitStucture.TIM_RepetitionCounter= 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStucture);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1 ;
	TIM_ICInitStructure.TIM_ICFilter=0xF ;
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising ;
	TIM_ICInitStructure.TIM_ICPrescaler= TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI ;//直连通道
	TIM_PWMIConfig (TIM3,&TIM_ICInitStructure);//同时配置两个通道 配好一个另一个自动配置为相反
//	TIM_ICInit(TIM3,&TIM_ICInitStructure);

	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);//TRGI（主模式）选择触发源
	TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);//从模式给RESET每一次上升沿就复位
	
	TIM_Cmd (TIM3,ENABLE );
}


uint32_t  IC_GetFreq()
{
	
	return 1000000/ (TIM_GetCapture1(TIM3) +1);
	
}
uint8_t IC_GetDuty()
{
	
	return (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1);
	
}
