#include "stm32f10x.h"                  // Device header
#include "mydefine.h"
#include "OLED.h"

int main(void)
{
	uint8_t id=0;
	SystemCoreClockUpdate();
	LED_Init();
	OLED_Init ();
	usart_init();
	iic_hardware_init();
	MPU6050_Init();
	schduler_init();
	SysTick_Config(SystemCoreClock / 1000U);
/*================ 写 0xAA 到 0x19 =================*/
	//MPU6050_send_byte(0x6B,0x00);
	//MPU6050_send_byte(0x19,0x66);
/*================ 从 0x19 读数据 =================*/
	//data=MPU6050_read_byte(0x19);
	id=MPU6050_read_byte(0x75);
	//OLED_ShowString(1,1,"data:");
	//OLED_ShowHexNum(1, 6, data, 2);
	OLED_ShowString(1,1,"id:");
	OLED_ShowHexNum(1,4, id, 2);

	while(1)
	{
		schduler_run();
		MPU6050_GetData(MPU6050_buffer);
		OLED_ShowSignedNum(2, 1, MPU6050_buffer[0], 5);					//OLED显示数据
		OLED_ShowSignedNum(3, 1, MPU6050_buffer[1], 5);
		OLED_ShowSignedNum(4, 1, MPU6050_buffer[2], 5);
		OLED_ShowSignedNum(2, 8, MPU6050_buffer[4], 5);
		OLED_ShowSignedNum(3, 8, MPU6050_buffer[5], 5);
		OLED_ShowSignedNum(4, 8, MPU6050_buffer[6], 5);
	}
}
