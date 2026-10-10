#include "stm32f10x.h"                  // Device header
#include "Delay.h"//调用Delay函数添加头文件
#include "LED.h"
#include "key.h"
#define  A LED1_ON
#define  B LED1_OFF
#define  C LED2_ON
#define D LED2_OFF
uint8_t key_Num;//定义全局变量 存储键码返回值 外部的为全局变量 函数内为局部变量 内部优先用局部变量 
int main(void)
{
	LED_Init( );//GPIO配置好后默认是低电平 初始化
	key_Init();//初始化
	while(1)
	{
		key_Num=key_GetNum();//按键1按下（键码为1） 按键2按下（键码为2）
		if(key_Num==1){
			LED1_Turn();
		}
		if(key_Num==2){
			LED2_Turn();
		}
}
}
