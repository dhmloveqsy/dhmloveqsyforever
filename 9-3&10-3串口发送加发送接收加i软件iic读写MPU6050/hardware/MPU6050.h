#ifndef MPU6050_H_
#define MPU6050_H_
#include "mydefine.h"
void iic_scan(void);
void MPU6050_send_byte(uint8_t address,uint8_t data);
uint8_t MPU6050_read_byte(uint8_t address);
void MPU6050_Init(void);
void MPU6050_GetData(int16_t *buffer);





#endif


