#include "usart_app.h"

uint8_t uart_rx_buf[128]; // 定义一个接收缓冲区，用于存储USART接收到的数据
uint16_t uart_rx_index = 0; // 定义一个索引变量，用于跟踪接收缓冲区中的数据位置
uint32_t uart_rx_get_time = 0; // 定义一个变量，用于记录接收数据的时间戳
#define UART_RX_TIMEOUT 100 // 定义一个超时时间，单位为毫秒

uint8_t uart_rx_dma_buffer[128]; // 定义一个接收缓冲区，用于存储USART接收到的数据
uint8_t uart_dma_buffer[256] ; // 定义一个索引变量，用于跟踪接收缓冲区中的数据位置
volatile uint8_t uart_flag = 0;// 定义一个标志变量，用于指示是否接收到数据

uint8_t ring_buffer[256]; // 定义一个环形缓冲区(数据池子)，用于存储USART接收到的数据
struct rt_ringbuffer uart_ringbuffer; // 定义一个环形缓冲区结构体，用于管理环形缓冲区


// 定义一个自定义的printf函数，用于通过USART发送格式化的字符串
int my_printf(UART_HandleTypeDef  *huart, const char *format, ...)
{
    char buffer[256]; // 定义一个缓冲区来存储格式化后的字符串
    va_list args; // 定义一个可变参数列表
    int length; // 定义一个变量来存储格式化后的字符串长度
    // 安全地格式化字符串到 buffer
    va_start(args, format); // 初始化可变参数列表
    length = vsnprintf(buffer, sizeof(buffer), format, args);// 使用 vsnprintf 来格式化字符串，确保不会溢出缓冲区
    va_end(args); // 清理可变参数列表

    // 通过USART发送缓冲区中的字符串 通过 HAL 库发送 buffer 中的内容
    HAL_UART_Transmit(huart, (uint8_t *)buffer, (uint16_t)length, 0xFF);
    return length; // 返回发送的字符数
}

// USART 接收完成回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef  *huart)
{
    if(huart->Instance == USART1) // 检查是否是 USART1 的回调
    {
        uart_rx_get_time = HAL_GetTick(); // 更新接收数据的时间戳
        uart_rx_index++;
       HAL_UART_Receive_IT(huart, &uart_rx_buf[uart_rx_index], 1); // 继续接收下一个字节
    }
}

//通过回调函数和超时解析完成串口的接收
//void uart_task()
//{
//        if(uart_rx_index == 0)
//        return; // 如果没有接收到数据，则直接返回
//        
//        // 检查是否超过超时时间
//         //当前的时间减上一次接收数据的时间戳，如果超过了超时时间，则认为接收完成
//        if(HAL_GetTick()-uart_rx_get_time>UART_RX_TIMEOUT)
//       {
//          uart_rx_buf[uart_rx_index] = '\0'; // 在接收缓冲区末尾添加字符串结束符
//          my_printf(&huart1, "uart receive: %s\r\n", uart_rx_buf); // 打印接收到的数据
//          memset(uart_rx_buf, 0, uart_rx_index); // 清空接收缓冲区
//          uart_rx_index = 0; // 重置接收索引以准备下一次接收
//          huart1.pRxBuffPtr = uart_rx_buf;// 重置接收缓冲区指针
//       }
//   //如果未超时那么就继续接收数据
//}

// USART 接收 DMA搬运完成回调函数
/*void Hal_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart->Instance == USART1) // 检查是否是 USART1 的回调
    {
        uart_flag = 1; // 设置标志，表示接收到数据
        //    因为空闲中断意味着发送方已经停止，防止 DMA 继续等待或出错
        HAL_UART_DMAStop(huart);
        memcpy(uart_dma_buffer, uart_rx_dma_buffer, Size); // 将 DMA 接收到的数据复制到待处理数据数组中 
        my_printf(&huart1, "uart receive: %s\r\n", uart_dma_buffer); // 打印接收到的数据
        memset(uart_rx_dma_buffer, 0, sizeof(uart_rx_dma_buffer)); // 清空接收缓冲区
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart_rx_dma_buffer, sizeof(uart_rx_dma_buffer)); // 重新启动 DMA 接收
    }
}
*/
// 通过DMA回调函数和空闲中断解析完成串口的接收
/*void uart_dma_task()
{
    if(uart_flag==0) // 检查是否接收到数据
    return; // 如果没有接收到数据，则直接返回

    uart_flag = 0; // 重置标志，准备下一次接收
    my_printf(&huart1, "dma data: %s\r\n", uart_dma_buffer); // 打印接收到的数据
    memset(uart_dma_buffer, 0, sizeof(uart_dma_buffer)); // 清空待处理数据数组
}
*/

// 通过ringbuffer DMA和空闲中断解析完成串口的接收
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart->Instance == USART1) // 检查是否是 USART1 的回调
    {
        
        //    因为空闲中断意味着发送方已经停止，防止 DMA 继续等待或出错
        HAL_UART_DMAStop(huart);
        // 将 DMA 接收到的有效数据放入环形缓冲区
        rt_ringbuffer_put(&uart_ringbuffer, uart_rx_dma_buffer, Size); // 将 DMA 接收到的数据放入环形缓冲区
			
			/*清空 DMA 接收缓冲区，为下次接收做准备*/
		//    虽然 memcpy 只复制了 Size 个，但清空整个缓冲区更保险
		memset(uart_rx_dma_buffer, 0, sizeof(uart_rx_dma_buffer));
         
			//设置为空闲状态，准备下一次接收
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart_rx_dma_buffer, sizeof(uart_rx_dma_buffer)); // 重新启动 DMA 接收
        __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);// 禁用半传输中断
    }
}

void uart_ringbuffer_task()
{
    uint16_t length = 0;
	length=rt_ringbuffer_data_len(&uart_ringbuffer); // 获取环形缓冲区中数据的长度

    /* 如果环形缓冲区中没有数据，则直接返回 */
    if(length==0)
        return;

    /*从ringbuffer中获取数据并且清除数据 将数据转移到待处理数组中*/
    rt_ringbuffer_get(&uart_ringbuffer, uart_dma_buffer, length);
		uart_dma_buffer[length]='\0';
		/* 去掉串口助手发送的 \r 和 \n */
    while(length > 0 &&
          (uart_dma_buffer[length - 1] == '\r' ||
           uart_dma_buffer[length - 1] == '\n'))
    {
        uart_dma_buffer[length - 1] = '\0';
        length--;
    }

    my_printf(&huart1,
              "uart receive: [%s], len=%d\r\n",
              uart_dma_buffer,
              length);
    /*数据处理区域*/
    //my_printf(&huart1,"uart receive: %s\r\n", uart_dma_buffer); // 打印接收到的数据
		if(strcmp((char *)uart_dma_buffer,"test")==0)
    {
			my_printf(&huart1,"=====system selftest=====\r\n");
			my_printf(&huart1,"flash............ok\r\n");
			my_printf(&huart1,"TF card............ok\r\n");
			my_printf(&huart1,"flash ID: 0x%lX\r\n", flash_id);
			my_printf(&huart1,"TF card memory:\r\n");
			RTC_PrintTime();
			my_printf(&huart1,msg, sizeof(msg),
             "RTC:%04d-%02d-%02d %02d:%02d:%02d\r\n",
             2000 + sDate.Year, sDate.Month, sDate.Date,
             sTime.Hours, sTime.Minutes, sTime.Seconds);
			my_printf(&huart1,"=====system selftest=====\r\n");
		}
    /* 清空待处理数据数组*/
    memset(uart_dma_buffer, 0, sizeof(uart_dma_buffer)); 
}





