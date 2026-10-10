#include "stm32f10x.h"                  // Device header
#include "Delay.h"//调用Delay函数添加头文件
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启时钟并选择外设 如PA0选择GPIOA结尾，PC13选GPIOC结尾
	
		GPIO_InitTypeDef GPIO_Initstucture;//结构体命名
		GPIO_Initstucture.GPIO_Mode= GPIO_Mode_Out_PP;//GPIO工作模式 点灯用推挽输出模式
		GPIO_Initstucture.GPIO_Pin=GPIO_Pin_All ;//选择所有引脚 也可以写为0xFFFF
		GPIO_Initstucture.GPIO_Speed= GPIO_Speed_50MHz;//设置GPIO的输出速度
		GPIO_Init(GPIOA,& GPIO_Initstucture);//将上面配置好的结构体参数写入GPIOA的寄存器，完成PA0引脚的初始化配置
//	GPIO_SetBits(GPIOA,GPIO_Pin_0);//填入外设 并将端口设置为高电平
//	GPIO_ResetBits(GPIOA,GPIO_Pin_0);//填入外设 并将端口设置为低电平
	
	while(1)
		{//0x后面四个0从左到右依次为16引脚的四个分组最右边为0~3引脚
		GPIO_Write(GPIOA,~0x0001);//0000 0000 0000 0001 低电平点亮要加按位取反符号~使一个点亮其他不亮
		Delay_ms(100);
		GPIO_Write(GPIOA,~0x0002);//0000 0000 0000 0010
		Delay_ms(100);
		GPIO_Write(GPIOA,~0x0004);//0000 0000 0000 0100
		Delay_ms(100);
		GPIO_Write(GPIOA,~0x0008);//0000 0000 0000 1000
		Delay_ms(100);
		GPIO_Write(GPIOA,~0x0010);//0000 0000 0001 0000
		Delay_ms(100);
		GPIO_Write(GPIOA,~0x0020);//0000 0000 0010 0000
		Delay_ms(100);
		GPIO_Write(GPIOA,~0x0040);//0000 0000 0100 0000
		Delay_ms(100);
		GPIO_Write(GPIOA,~0x0080);//0000 0000 1000 0000
		Delay_ms(100);
		
		
}
}
