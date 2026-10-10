#include "stm32f10x.h"                  // Device header
void Encoder_init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		//将PA0和PA1引脚初始化为上拉输入
	
	TIM_InternalClockConfig (TIM2);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision= TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode= TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period= 65536-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler=1-1 ;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter= 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel= TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter= 0x2;
	
	TIM_ICInitStructure.TIM_Channel= TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter= 0x2;
	
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	//配置编码器模式以及两个输入通道是否反相
	//注意此时参数的Rising和Falling已经不代表上升沿和下降沿了，而是代表是否反相
	//此函数必须在输入捕获初始化之后进行，否则输入捕获的配置会覆盖此函数的部分配置
	TIM_Cmd (TIM2,ENABLE );
}
int32_t Encoder_Get(void)
{
	int32_t temp;
	temp=TIM_GetCounter(TIM2);
	TIM_SetCounter(TIM2,0);
	return temp;
	
}




