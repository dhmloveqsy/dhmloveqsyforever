#include "MPU6050.h"
#define MPU6050_address_write 0xD0
#define MPU6050_address_read 0xD1
/*================ 写 数据 到 寄存器地址 =================*/
void time_out(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t time_out=0,time_now=0;
	time_out=Scheduler_GetTick();
	while(I2C_CheckEvent(I2Cx,I2C_EVENT)==ERROR)//EV5事件
	{
		time_now=Scheduler_GetTick();
		if(time_now-time_out>=1000)
		{
		I2C_GenerateSTOP(I2Cx,ENABLE);
		break;
		}
	}
}
void MPU6050_send_byte(uint8_t reg_address,uint8_t data)
{	
	I2C_GenerateSTART(I2C2,ENABLE);
	time_out(I2C2, I2C_EVENT_MASTER_MODE_SELECT);

	I2C_SendData(I2C2,MPU6050_address_write);
	time_out(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//EV6事件

	I2C_SendData(I2C2,reg_address);
	time_out(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);//EV8事件

	I2C_SendData(I2C2,data);
	time_out(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);//EV8_2事件

	I2C_GenerateSTOP(I2C2,ENABLE);
}

/*================ 从 寄存器地址 读数据 =================*/
uint8_t MPU6050_read_byte(uint8_t reg_address)
{
	uint8_t data=0;
	I2C_GenerateSTART(I2C2,ENABLE);
	time_out(I2C2, I2C_EVENT_MASTER_MODE_SELECT);//EV5事件
	
	
	I2C_SendData(I2C2,MPU6050_address_write);
	time_out(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//EV6事件
	
	
	I2C_SendData(I2C2,reg_address);
	time_out(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);//EV8_2事件
	
	
	I2C_GenerateSTART(I2C2,ENABLE);
	time_out(I2C2, I2C_EVENT_MASTER_MODE_SELECT);//EV5事件
	
	
	I2C_SendData(I2C2,MPU6050_address_read);
	time_out(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);//EV6事件
	
	
	I2C_AcknowledgeConfig(I2C2,DISABLE);//单次读取数据(如果需要连续读 就是最后一个数据之后不连续读此时最后一个数据被视为单次读取数据)要在读取数据之前给ACK置非使能 停止位为1
	I2C_GenerateSTOP(I2C2,ENABLE);//申请产生终止条件
	time_out(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);//EV7事件 数据通过移位寄存器转移到DR数据寄存器
	data=I2C_ReceiveData(I2C2);//读取数据寄存器
	
	I2C_AcknowledgeConfig(I2C2,ENABLE);
	return data;
}

int16_t MPU6050_buffer[7]={0};
void MPU6050_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Ack=I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed=100000;//0-100kHz是标准速度 100-400是快速
	I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_2;//标准速度下时钟占空比低电平：高电平=1：1
	I2C_InitStructure.I2C_Mode=I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1=0x00;
	I2C_Init(I2C2,&I2C_InitStructure);
	
	I2C_Cmd(I2C2,ENABLE);
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

