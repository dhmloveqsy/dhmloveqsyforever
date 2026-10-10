#include "iic_hardware.h"
#define SCL_PORT GPIOB
#define SCL_PIN GPIO_Pin_13
#define SDA_PORT GPIOA
#define SDA_PIN GPIO_Pin_8
void iic_hardware_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SCL_PIN;  // SCL pins
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // Open-drain output
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = SDA_PIN;  // SDA pins
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // Open-drain outpu
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(SCL_PORT, SCL_PIN); // Set SCL high
    GPIO_SetBits(SDA_PORT, SDA_PIN); // Set SDA high
}
void iic_hardware_start(void)
{
    iic_w_sda(1);//先拉高SDA确保在SCL高低平时能产生下降沿
    iic_w_scl(1);//
    iic_w_sda(0);//
    iic_w_scl(0);//此时就可以写SDA了
}
void iic_hardware_stop(void)
{
    iic_w_sda(0);//先拉低SDA确保在SCL高低平时能产生上升沿
    iic_w_scl(1);//
    iic_w_sda(1);//
}
void iic_w_scl(uint8_t bitvalue)
{
    GPIO_WriteBit(SCL_PORT, SCL_PIN, (BitAction)bitvalue);
    Delay_us(10); // Delay for a short time
}
void iic_w_sda(uint8_t bitvalue)
{
    GPIO_WriteBit(SDA_PORT, SDA_PIN, (BitAction)bitvalue);
    Delay_us(10); // Delay for a short time
}
uint8_t iic_r_sda(void)
{
    uint8_t bitvalue;
    bitvalue = GPIO_ReadInputDataBit(SDA_PORT, SDA_PIN);
    Delay_us(10); // Delay for a short time
    return bitvalue;
}
void iic_sendbyte(uint8_t data)
{
    for (int i = 0; i < 8; i++)
    {
        if (data & (0x80>>i)) // Check the MSB
            iic_w_sda(1); // Send 1
        else
            iic_w_sda(0); // Send 0
        iic_w_scl(1); // Set SCL high to clock in the bit
        iic_w_scl(0); // Set SCL low
        
    }
}
uint8_t iic_receivebyte(void)
{
    uint8_t data = 0;
    iic_w_sda(1);
    for(uint8_t i = 0; i < 8; i++)
    {
        iic_w_scl(1); // Set SCL high to read bit
        if (iic_r_sda()) // Read SDA
            data |= (0x80>>i); // Set MSB if SDA is high
        iic_w_scl(0); // Set SCL low after reading bit
    }
    return data;
}
void iic_sendack(uint8_t ackbit)
{
    iic_w_sda(ackbit); // Send ACK or NACK
    iic_w_scl(1); // Set SCL high to clock in the ACK 
    iic_w_scl(0); // Set SCL low after sending ACK  
}
uint8_t iic_receiveack(void)
{
    uint8_t ackbit = 0;
    iic_w_sda(1);
    iic_w_scl(1); // Set SCL high to read ACK
    ackbit = iic_r_sda(); // Read ACK bit`
    iic_w_scl(0); // Set SCL low after reading ACK
    return ackbit;
}
