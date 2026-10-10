 #include "key.h"            // Device header

void key_Init(void)// 明确告诉编译器，这个函数不需要传入任何参数 调用时引用内部的内容即可
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入为高电平
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

uint8_t key_GetNum(void)//在 STM32 等嵌入式编程里，寄存器的位宽通常是 8 位，unsigned char 用来直接操作寄存器的字节值，避免符号位干扰。
// 函数声明：返回值是uint8_t（8位无符号整数） 读取按键值的函数
{
	uint8_t key_Num=0; // 局部变量也用uint8_t，和返回值类型一致 
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)//如果满足则说明按键按下
	{
		Delay_ms(20);//刚按下会有抖动通过延时避免
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);//对是否松按键的检测 未松则停止在这一行 松开进行下一行
		Delay_ms(20);//按键松开也有抖动
		key_Num=1;//传递按键1按下（键码为1）
	}		
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)//如果满足则说明按键按下
	{
		Delay_ms(20);//刚按下会有抖动通过延时避免
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0);//对是否松按键的检测 未松则停止在这一行 松开进行下一行
		Delay_ms(20);//按键松开也有抖动
		key_Num=2;//传递按键2按下（键码为2）
	}
	return key_Num;
}
