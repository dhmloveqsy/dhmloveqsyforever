#include "stm32f10x.h"                  // Device header
#include "Delay.h"//调用Delay函数添加头文件
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启时钟并选择外设 PB12用GPIOB结尾
	
		GPIO_InitTypeDef GPIO_Initstucture;//结构体命名
		GPIO_Initstucture.GPIO_Mode= GPIO_Mode_Out_PP;//GPIO工作模式 蜂鸣器低电平响
		GPIO_Initstucture.GPIO_Pin=GPIO_Pin_13 ;//根据引脚编号 选择要配置的引脚
		GPIO_Initstucture.GPIO_Speed= GPIO_Speed_50MHz;//设置GPIO的输出速度
		GPIO_Init(GPIOB,& GPIO_Initstucture);//将上面配置好的结构体参数写入GPIOA的寄存器，完成PA0引脚的初始化配置
//	GPIO_SetBits(GPIOA,GPIO_Pin_0);//填入外设 并将端口设置为高电平
//	GPIO_ResetBits(GPIOA,GPIO_Pin_0);//填入外设 并将端口设置为低电平
	
	while(1)
	{
//		GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);//设置单个端口 枚举变量最后是清除端口值，低电平
//		Delay_ms(500);//添加延时函数实现间断响
//		GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);//设置单个端口 枚举变量最后是设置端口值，高电平
//		Delay_ms(500);
		
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
		Delay_ms(100);
		GPIO_SetBits(GPIOB,GPIO_Pin_13);
		Delay_ms(100);
		
		GPIO_WriteBit(GPIOB,GPIO_Pin_13,(BitAction)0);//设置单个端口 枚举变量最后强制转换将0转变成BitAction枚举类型
		Delay_ms(100);
		GPIO_WriteBit(GPIOB,GPIO_Pin_13,(BitAction)1);//设置单个端口 枚举变量最后强制转换将1转变成BitAction枚举类型
		Delay_ms(700);
}
}
