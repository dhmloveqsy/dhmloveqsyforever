#include "stm32f10x.h"    // Device header
#include "Delay.h"
void Timer_init()
{
	//开启RCC内部时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE );
	//选择内部时钟源
	TIM_InternalClockConfig(TIM2);
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision= TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStructure.TIM_Period= 9999;//ARR 0-65535
	TIM_TimeBaseInitStructure.TIM_Prescaler= 7199;//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0 ;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	//配置终端输出控制
	TIM_ClearFlag (TIM2,TIM_IT_Update);//防止刚更新就触发中断
	TIM_ITConfig (TIM2,TIM_IT_Update,ENABLE );
	
	//配置NVIC 先分组后配置参数
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel= TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE  ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 1;
	NVIC_Init(&NVIC_InitStructure);
//	//运行控制（计数器）
//	
//	TIM_UpdateDisableConfig (TIM2,ENABLE );
	//开启定时器
	TIM_Cmd(TIM2, ENABLE );
	
}
uint32_t data_2=0;

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET  )
	{
		
		
		data_2+=1;
		
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		
	}
	
	
}