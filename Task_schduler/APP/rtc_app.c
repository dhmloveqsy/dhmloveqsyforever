#include "rtc_app.h"
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
char msg[64];
void rtc_hal_init(void)
{
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat     = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv   = 127;   /* (127+1)*(255+1) = 32768 */
    hrtc.Init.SynchPrediv    = 255;
    hrtc.Init.OutPut         = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;

    if (HAL_RTC_Init(&hrtc) != HAL_OK) Error_Handler();
}

void rtc_hal_set(void)
{
    RTC_TimeTypeDef t = {0};
    RTC_DateTypeDef d = {0};

    t.Hours          = 12;
    t.Minutes        = 30;
    t.Seconds        = 0;
    t.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    t.StoreOperation = RTC_STOREOPERATION_RESET;
    HAL_RTC_SetTime(&hrtc, &t, RTC_FORMAT_BIN);

    d.WeekDay  = RTC_WEEKDAY_MONDAY;
    d.Month    = RTC_MONTH_JANUARY;
    d.Date     = 5;
    d.Year     = 25;                 /* 2025 */
    HAL_RTC_SetDate(&hrtc, &d, RTC_FORMAT_BIN);
}


void RTC_PrintTime(void)
{
    /* 注意顺序：先读时间，再读日期 */
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 1000);
}


