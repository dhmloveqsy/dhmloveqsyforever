#include "stm32f10x.h" 

void MH_init()
{
	//开启端口时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE );
	//开启AFIO的时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO,ENABLE );
	//初始化GPIO
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//配置AFIO
	//选择GPIO线作为外部时钟线
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource9);
	//配置EXTI参数
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line=EXTI_Line9;//EXTI外部中断线
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;//指定EXTI外部中断线的状态 使能
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//EXTI模式 中断模式还是事件模式
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;//触发方式 双边沿触发
	EXTI_Init(&EXTI_InitStructure);
	//配置nvic（内核外设）中断 不需要打开时钟 默认开启
	//分组放在主函数
	 NVIC_InitTypeDef NVIC_InitStructure;
	 NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn ;//指定要开启的中断通道
	 NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//启用通道
	 //通过分组得到优先级的范围从范围里选择个数
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1 ;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	
	 NVIC_Init (&NVIC_InitStructure);
	
}
//uint32_t  num=0;
//void  EXTI9_5_IRQHandler(void )
//{
//	
//	//中断9产生的中断标志位（用于中断函数） 
//	if(EXTI_GetITStatus(EXTI_Line9)==SET)
//	{
//		//清除中断位
//		EXTI_ClearITPendingBit(EXTI_Line9);
//		//记录中断发生次数
//		num=num+1;
//		
//	}
//}




