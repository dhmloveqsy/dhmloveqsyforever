#include "key_app.h"

uint8_t key_val, key_old, key_up, key_down;

uint8_t key_read_task(void)
{
    uint8_t key_value = 0;

    if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
    {
        key_value = 1;
    }
    if (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)
    {
        key_value = 2;
    }
    if (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET)
    {
        key_value = 3;
    }
    if (HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == GPIO_PIN_RESET)
    {
        key_value = 4;
    }
    if (HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin) == GPIO_PIN_RESET)
    {
        key_value = 5;
    }
    if (HAL_GPIO_ReadPin(KEY6_GPIO_Port, KEY6_Pin) == GPIO_PIN_RESET)
    {
        key_value = 6;
    }

    return key_value;
}

void key_task(void)
{
    key_val = key_read_task();
    key_down = key_val & (key_val ^ key_old);
    key_up = ~key_val & (key_val ^ key_old);
    key_old = key_val;

    if ((key_down >= 1) && (key_down <= 6))
    {
        ucled[key_down - 1] ^= 1;
    }
}

typedef struct {
    GPIO_TypeDef  *GPIOx;//句柄 是哪一端口
    uint32_t GPIO_Pin;//引脚
    uint16_t key_tick;//用于判断长按还是短按 还可以用来判断按键是否空闲
    uint32_t key_id;//按键的身份证
    uint8_t level; //1是抬起 初始状态未按下
    uint8_t state; //-对应初始状态未按下 1是按下的状态 2是抬起的状态
    uint8_t debouce_cnt;//用于按键消抖的变量
    uint8_t repeat; //记录短按的次数
}button;
button my_button[6]={
    {GPIOB,KEY1_Pin,0,1,1,0,0,0},
    {GPIOB,KEY2_Pin,0,2,1,0,0,0},
    {GPIOB,KEY3_Pin,0,3,1,0,0,0},
    {GPIOB,KEY4_Pin,0,4,1,0,0,0},
    {GPIOB,KEY5_Pin,0,5,1,0,0,0},
    {GPIOB,KEY6_Pin,0,6,1,0,0,0},
};//初始化按键

void key_text(button* key)
{
    uint8_t key_value=HAL_GPIO_ReadPin(key->GPIOx,key->GPIO_Pin);
    if(key->state!=0)
    key->key_tick++;

    if(key_value!=key->level)
    {
        if(++(key->debouce_cnt)>=3)
        {
            key->level=key_value;
            key->debouce_cnt=0;
        }
        
    }
    else
    key->debouce_cnt=0;

    switch (key->state)
    {
    case 0:
    if(key->level==0)
    {
    key->key_tick=0;
    key->state=1;
		key->repeat=1;
    }
    break;
    case 1:
    if(key->level==1)
    {
    if(key->key_tick>=15)
    {
    key->repeat=0;
    }
		/*双击*/
    else if (key->repeat == 2)
        {
           ucled[2]^=1;
            key->repeat = 0;  // 双击事件已处理
        }
    key->key_tick=0;
    key->state=2;
    }  
    break;
    case 2:
    if(key->key_tick>=10)
    {
        key->state=0;
    }
    else if(key->level==0)
		{ 
			 key->repeat++;
			 key->state=1;
			 key->key_tick=0;
		}
    break;
    }
		
}
void key_proc(void)
{
    for(uint8_t i=0;i<6;i++)
    {
        key_text(&my_button[i]);
    }
}



