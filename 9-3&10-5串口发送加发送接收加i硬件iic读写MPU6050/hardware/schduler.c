#include "schduler.h"


volatile uint32_t g_tick_ms = 0;

void Scheduler_Tick(void)
{
    g_tick_ms++;
}

uint32_t Scheduler_GetTick(void)
{
    return g_tick_ms;
}
static uint8_t task_num;
typedef struct{
	void (*task_func)(void);
	uint32_t task_period;
	uint32_t task_last_time;
}task_t;
void task(void)
{

}
static task_t schduler_t[]={
	//{led_task,500,0 },
	{usart_proc,10,0 }
};

void schduler_init(void)
{
	task_num=sizeof(schduler_t)/sizeof(task_t);
}
void schduler_run(void)
{
	static uint32_t time_get=0;
	uint8_t i=0;
	time_get=Scheduler_GetTick();
	for(;i<task_num;i++)
	{
		if((uint32_t)(time_get-schduler_t[i].task_last_time)>=schduler_t[i].task_period)
		{
			schduler_t[i].task_last_time=time_get;
			schduler_t[i].task_func();
		}
	}
	
	
}




