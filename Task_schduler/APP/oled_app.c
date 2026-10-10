#include "oled_app.h"


int my_oled_printf(uint8_t x,uint8_t y,const char *format, ...)
{
	char buffer[512];
	int len;
	va_list arg;
	va_start(arg,format);
	len=vsnprintf(buffer,sizeof(buffer),format,arg);
	va_end(arg);
	OLED_ShowStr(x,y,buffer,8);
	return len;
}

void oled_task(void)
{
	WouoUI_Proc(10);
	//my_oled_printf(0,1,"Hello World");
}

/**
 * @brief 将 WouoUI 缓冲区的数据发送到 OLED 显示
 * @param buff WouoUI 提供的缓冲区指针，大小为 [高度/8][宽度] 或 [4][128] for 128x32
 */
void OLED_SendBuff(uint8_t buff[4][128])
{  
//    // 遍历每一页 (0-3)
//    for(uint8_t page = 0; page < 4; page++)  
//    {  
//        // 设置 OLED 的页地址
//        OLED_Write_cmd(0xb0 + page); // 0xB0 - 0xB3
//        
//        // 设置 OLED 的列地址 (从第 0 列开始)
//        OLED_Write_cmd(0x00); // 低半字节列地址
//        OLED_Write_cmd(0x10); // 高半字节列地址 (0x10 | 0 = 0)

//        // 循环写入该页的 128 列数据
//        for (uint8_t column = 0; column < 128; column++)  
//        {
//            // 从 WouoUI 缓冲区取数据，并通过基础驱动的数据写入函数发送
//            OLED_Write_data(buff[page][column]); 
//        }
//    } 
		// 1. 获取 u8g2 内部 RAM 缓冲区的指针
    uint8_t *u8g2_buffer = u8g2_GetBufferPtr(&u8g2);

    // 2. 将 WouoUI 缓冲区的内容完整地复制到 u8g2 的缓冲区中
    //    注意：确保两个缓冲区大小完全一致 (4 * 128 bytes)
    memcpy(u8g2_buffer, buff, 4 * 128);

    // 3. 调用 u8g2 的发送函数，将 u8g2 缓冲区的内容发送到 OLED
    //    这将使用 u8g2 配置的高效 I2C 回调函数来完成传输
    u8g2_SendBuffer(&u8g2);
}

