#include "schduler.h"
uint8_t  time_get;//获取任务数量


 typedef struct{
	void (*task_func)(void);
	uint32_t  rate_ms;
	uint32_t  last_run;
}tast_t;//任务结构体


//任务列表
static  tast_t schduler_tast[]=
{
	{led_disp_task,1,0},
	{btn_task,5,0},
	{uart_ringbuffer_task,5,0},
	{oled_task,10,0},
	
//	{adc_proc,10,0}
};

//任务数量
void schduler_init()
{
	time_get =sizeof (schduler_tast)/sizeof (tast_t);//获取任务数量
}

//任务调度
void schduler_run()
{
	for(uint8_t i=0;i<time_get;i++)//遍历任务列表
	{
	uint32_t  now_time=HAL_GetTick ();//获取当前时间
	if(now_time>=schduler_tast[i].rate_ms+schduler_tast[i].last_run)//判断是否到达任务执行时间
	{
		schduler_tast[i].last_run=now_time;//更新任务上次执行时间
		schduler_tast[i].task_func();//执行任务
	}
	}
}


