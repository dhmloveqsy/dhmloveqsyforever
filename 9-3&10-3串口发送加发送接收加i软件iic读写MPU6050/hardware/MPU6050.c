#include "MPU6050.h"
//遍历0x00~0x7F的I2C地址(7位地址)，打印出有响应的设备地址
void iic_scan(void)
{
	uint16_t address;
	uint8_t device_count = 0;
	uint8_t ack;
	my_printf(USART1, "I2C scan start\r\n");
	for(address = 0x00; address <= 0x7F; address++)
	{
		device_count++;
		iic_hardware_start();
		iic_sendbyte((uint8_t)(address<<1));	//发送地址+写位
		ack = iic_receiveack();
		if(ack == 0)
		{
			my_printf(USART1, "found: 7-bit=0x%02X, write=0x%02X\r\n",
				(unsigned int)(address), (unsigned int)(address<<1));
		}
		iic_hardware_stop();
	}
	my_printf(USART1, "I2C scan done, count=%u\r\n", (unsigned int)device_count);
}
/*================ 写 数据 到 寄存器地址 =================*/
void MPU6050_send_byte(uint8_t address,uint8_t data)
{
	uint8_t iic_ack=0;
iic_hardware_start();

iic_sendbyte(0xD0);              // 0x68 + Write
iic_ack = iic_receiveack();

if(iic_ack == 0)
{
    iic_sendbyte(address);          // 寄存器地址
    iic_ack = iic_receiveack();
}

if(iic_ack == 0)
{
    iic_sendbyte(data);          // 写入数据
    iic_ack = iic_receiveack();
}
iic_hardware_stop();
}

/*================ 从 寄存器地址 读数据 =================*/
uint8_t MPU6050_read_byte(uint8_t address)
{
	uint8_t iic_ack=0,data=0;
iic_hardware_start();

iic_sendbyte(0xD0);              // 0x68 + Write
iic_ack = iic_receiveack();

if(iic_ack == 0)
{
    iic_sendbyte(address);          // 指定要读取的寄存器
    iic_ack = iic_receiveack();
}

if(iic_ack == 0)
{
    iic_hardware_start();        // Repeated START

    iic_sendbyte(0xD1);          // 0x68 + Read
    iic_ack = iic_receiveack();
    if(iic_ack == 0)
    {
        data = iic_receivebyte();
		iic_sendack(1);
		
    }
}

iic_hardware_stop();
return data;
}
int16_t MPU6050_buffer[7]={0};
void MPU6050_Init(void)
{
	/*MPU6050寄存器初始化，需要对照MPU6050手册的寄存器描述配置，此处仅配置了部分重要的寄存器*/
	MPU6050_send_byte(MPU6050_PWR_MGMT_1, 0x01);		//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	MPU6050_send_byte(MPU6050_PWR_MGMT_2, 0x00);		//电源管理寄存器2，保持默认值0，所有轴均不待机
	MPU6050_send_byte(MPU6050_SMPLRT_DIV, 0x09);		//采样率分频寄存器，配置采样率
	MPU6050_send_byte(MPU6050_CONFIG, 0x06);			//配置寄存器，配置DLPF
	MPU6050_send_byte(MPU6050_GYRO_CONFIG, 0x18);	//陀螺仪配置寄存器，选择满量程为±2000°/s
	MPU6050_send_byte(MPU6050_ACCEL_CONFIG, 0x18);	//加速度计配置寄存器，选择满量程为±16g
}
void MPU6050_GetData(int16_t *MPU6050_buffer)
{
    uint8_t i=0;
    uint8_t DataH=0, DataL=0;								//定义数据高8位和低8位的变量
	
    for(uint8_t address=0x3B;address<=0x47;address+=2)
    {
        DataH = MPU6050_read_byte(address);		//读取加速度计X轴的高8位数据
	    DataL = MPU6050_read_byte(address+1);		//读取加速度计X轴的低8位数据
        /*前三个数据是加速度计（Acc）的16位数据 后三个数据为陀螺仪（Gyro）的16位数据 顺序为xyz*/
        MPU6050_buffer[i]=(DataH << 8) | DataL;	
        i++;
    }
}

