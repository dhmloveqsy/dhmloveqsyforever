#ifndef IIC_H_
#define IIC_H_
#include "mydefine.h"
void iic_hardware_init(void);
void iic_hardware_start(void);
void iic_hardware_stop(void);
void iic_w_scl(uint8_t bitvalue);
void iic_w_sda(uint8_t bitvalue);
uint8_t iic_r_sda(void);
void iic_sendbyte(uint8_t data);
uint8_t iic_receivebyte(void);
void iic_sendack(uint8_t ackbit);
uint8_t iic_receiveack(void);    
#endif


