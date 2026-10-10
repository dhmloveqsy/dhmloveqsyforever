#include "adc_app.h"
#define ADC_DMA_BUFFER_SIZE 32
__IO uint32_t adc_value=0;
__IO float voltage=0;
uint32_t adc_dma_buffer[ADC_DMA_BUFFER_SIZE]; //定义DMA目标缓冲区
#define BUFFER_SIZE 1000
// --- 全局变量 ---
uint32_t dac_val_buffer[BUFFER_SIZE / 2]; // 用于存储处理后的 ADC 数据
__IO uint32_t adc_val_buffer[BUFFER_SIZE]; // DMA 目标缓冲区 (存储原始 ADC 数据)

__IO uint8_t AdcConvEnd = 0;             // ADC 转换完成标志 (一个块完成)
#define ADC_VREF_VOLTAGE  3.3f
#define ADC_MAX_VALUE     4095.0f

//单次转换轮询方式
void adc_read_by_polling(void)
{
    HAL_ADC_Start(&hadc1);//启动ADC转换
    /*等待转换完成（阻塞式，最多等 1000ms）*/
    if(HAL_ADC_PollForConversion(&hadc1, 1000) == HAL_OK)
    {
        adc_value = HAL_ADC_GetValue(&hadc1);
        voltage = (float)adc_value * ADC_VREF_VOLTAGE / ADC_MAX_VALUE; //计算电压值，假设参考电压为3.3V，12位ADC分辨率为4095
        my_printf(&huart1, "adc_value=%lu voltage=%.2fV\r\n", adc_value, voltage);
    }

    else
    {
        //转换超时的处理
    }
   // HAL_ADC_Stop(&hadc1);//若配置为连续转换模式，读取完adc_value则需要手动停止ADC
}




//单通道连续转换DMA搬运的方式
void adc_dma_init(void)
{
     // 启动 ADC 并使能 DMA 传输
    // hadc1: ADC 句柄
    // (uint32_t*)adc_dma_buffer: DMA 目标缓冲区地址 (HAL库通常需要uint32_t*)
    // ADC_DMA_BUFFER_SIZE: 本次传输的数据量 (缓冲区大小)
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_dma_buffer, ADC_DMA_BUFFER_SIZE);
}
// 单通道连续转换DMA搬运的方式，读取DMA缓冲区数据并计算平均值
void adc_task()
{
    uint32_t sum = 0;
    for (int i = 0; i < ADC_DMA_BUFFER_SIZE; i++)
    {
        sum += (uint16_t)adc_dma_buffer[i];
    }
    adc_value = sum / ADC_DMA_BUFFER_SIZE; // 计算平均值
    voltage = (float)adc_value * ADC_VREF_VOLTAGE / ADC_MAX_VALUE; //计算电压值，假设参考电压为3.3V，12位ADC分辨率为4095
    my_printf(&huart1, "adc_value=%lu voltage=%.2fV\r\n", adc_value, voltage);
}




//定时器触发DMA搬运ADC数据的方式
void adc_tim_dma_init(void)
{
    // 注意：如果使用定时器触发，需要在此处或之前启动定时器
    HAL_TIM_Base_Start(&htim3); // 替换 htimX 为实际定时器句柄
    // 启动 ADC 的 DMA 传输，请求 BUFFER_SIZE 个数据点
    // 注意：这里假设 hadc1 已经配置为合适的触发模式 (定时器或软件)
    // 且 DMA 配置为 Normal 模式  
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_val_buffer, BUFFER_SIZE);
     // 显式禁用 DMA 半传输中断 (如果不需要处理半满事件)
    __HAL_DMA_DISABLE_IT(&hdma_adc1, DMA_IT_HT);
}
// --- ADC 转换完成回调函数 (由 DMA TC 中断触发) ---
// 当 DMA 完成整个缓冲区的传输 (Normal 模式下传输 BUFFER_SIZE 个点) 时触发
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    // 检查是否是由我们关心的 ADC (hadc1) 触发的
    if (hadc->Instance == ADC1) // 或 if(hadc == &hadc1)
    {
        HAL_ADC_Stop_DMA(hadc);

        // 设置转换完成标志，通知后台任务数据已准备好
        AdcConvEnd = 1;
    }
}

void adc_proc()
{
    if(AdcConvEnd)
    {
         // 处理数据: 从原始 ADC 缓冲区提取数据到 dac_val_buffer
        // 示例逻辑：提取扫描转换中第二个通道的数据 (?)
        for(uint16_t i = 0; i < BUFFER_SIZE / 2; i++)
        {
            // 假设 adc_val_buffer[0] 是通道1, adc_val_buffer[1] 是通道2, ...
            dac_val_buffer[i] = adc_val_buffer[i * 2 + 1];
        }

        // 打印处理后的数据 (示例)
        for(uint16_t i = 0; i < BUFFER_SIZE / 2; i++)
        {
            // 注意: my_printf 是自定义函数, 需确保其存在且可用
            my_printf(&huart1, "{dac}%d\r\n", (int)dac_val_buffer[i]);
        }
        
        // 清理处理后的缓冲区 (可选)
        memset(dac_val_buffer, 0, sizeof(uint32_t) * (BUFFER_SIZE / 2));

        // 清除转换完成标志，准备下一次采集
        AdcConvEnd = 0;

        // 重新启动 ADC 的 DMA 传输，采集下一个数据块
        // 注意: 需要确保 ADC 状态适合重启 (例如没有错误)
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_val_buffer, BUFFER_SIZE);
        // 再次禁用半传输中断 (如果 Start_DMA 会重新启用它)
        __HAL_DMA_DISABLE_IT(&hdma_adc1, DMA_IT_HT);


    }

}




