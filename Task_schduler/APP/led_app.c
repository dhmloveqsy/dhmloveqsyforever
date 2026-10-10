#include "led_app.h"

uint8_t ucled[6]={0,0,0,0,0,0};
void led_disp(uint8_t *ucled)
{
    // 定义状态存储变量以及更新变量
    uint8_t temp=0x00;
    // 定义一个静态变量来存储上一次的状态值(避免每次刷新)
    static uint8_t temp_old=0xff;

    // 将ucled数组的状态值转换为一个字节的状态值
    for(uint8_t i=0;i<6;i++)
    {
        if(ucled[i])
            temp|=(1<<i);
    }
    
    // 如果状态值发生变化，则更新LED灯的状态
    if(temp!=temp_old)
    {
        temp_old=temp;// 更新状态值
        // 根据状态值控制LED灯的开关
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,(temp&0x01)?GPIO_PIN_SET:GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,(temp&0x02)?GPIO_PIN_SET:GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,(temp&0x04)?GPIO_PIN_SET:GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,(temp&0x08)?GPIO_PIN_SET:GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_5,(temp&0x10)?GPIO_PIN_SET:GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,(temp&0x20)?GPIO_PIN_SET:GPIO_PIN_RESET);
    }
}
void led_disp_task(void)
{
    led_disp(ucled);
}



