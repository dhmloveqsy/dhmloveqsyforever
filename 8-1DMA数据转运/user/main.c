#include "stm32f10x.h"                  // Device header
#include "Delay.h"//调用Delay函数添加头文件
#include "OLED.h"
#include "MyDMA.h"
 uint16_t app[]={0x01,0x02,0x03,0x04};
  uint16_t apple[4]={0};
int main(void)
{
	OLED_Init ();
	MyDMA_Init(app, apple, 4);
//	OLED_ShowString(1, 1, "want.love");
//	OLED_ShowString(2, 1, "be.fridendly");
//	OLED_ShowString(3, 1, "have.courage");
//	OLED_ShowString(4, 1, "try.confidence");
	/*OLED显示*/
//	OLED_ShowChar(1, 1, 'A');				//1行1列显示字符A
//	
//	OLED_ShowString(1, 3, "HelloWorld!");	//1行3列显示字符串HelloWorld!
//	
//	OLED_ShowNum(2, 1, 12345, 5);			//2行1列显示十进制数字12345，长度为5
//	
//	OLED_ShowSignedNum(2, 7, -66, 2);		//2行7列显示有符号十进制数字-66，长度为2
//	
//	OLED_ShowHexNum(3, 1, 0xAA55, 4);		//3行1列显示十六进制数字0xA5A5，长度为4
//	
//	OLED_ShowBinNum(4, 1, 0xAA55, 16);		//4行1列显示二进制数字0xA5A5，长度为16
//											//C语言无法直接写出二进制数字，故需要用十六进制表示
	while(1)
	{
		OLED_ShowHexNum(1,1,apple[0],2);
		OLED_ShowHexNum(1,4,apple[1],2);
		OLED_ShowHexNum(1,7,apple[2],2);
		OLED_ShowHexNum(1,10,apple[3],2);
		}
}
