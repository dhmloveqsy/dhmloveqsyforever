#include "stm32f10x.h"                  // Device header
#include "Delay.h"//调用Delay函数添加头文件
#include "buzzer.h" 
#include "lightsensor.h"
#define  A buzzer_ON//宏定义
#define  B buzzer_OFF 
int main(void)
{
	buzzer_Init();
	light_Init();
	while(1)
	{
		if(light()==0){
		B();
		}else{
			A();
}
}
	}
            