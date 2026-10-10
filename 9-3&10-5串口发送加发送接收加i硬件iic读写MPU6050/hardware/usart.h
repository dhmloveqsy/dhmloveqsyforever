#ifndef USART_H_
#define USART_H_
#include "mydefine.h"
void usart_init(void);
void my_printf(USART_TypeDef*USART, const char *format, ...);
void usart_proc(void);
#endif




