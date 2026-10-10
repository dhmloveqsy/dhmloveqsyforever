#include "stm32f10x.h"                  // Device header
uint16_t MyDMA_Size;
void MyDMA_Init(uint16_t *SrcAddr, uint16_t *DstAddr, uint16_t Size)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE );
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr= (uint32_t)SrcAddr;
	DMA_InitStructure.DMA_PeripheralDataSize= DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc= DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_MemoryBaseAddr= (uint32_t)DstAddr;
	DMA_InitStructure.DMA_MemoryDataSize= DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC ;
	DMA_InitStructure.DMA_BufferSize=Size;
	DMA_InitStructure.DMA_Mode= DMA_Mode_Normal;
	DMA_InitStructure.DMA_M2M=DMA_M2M_Enable ;
	DMA_InitStructure.DMA_Priority=DMA_Priority_Medium ;
		DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1,ENABLE );
	MyDMA_Size=Size;
	
}
void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);					//DMAʧ�ܣ���д�봫�������֮ǰ����ҪDMA��ͣ����
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);	//д�봫���������ָ����Ҫת�˵Ĵ���
	DMA_Cmd(DMA1_Channel1, ENABLE);						//DMAʹ�ܣ���ʼ����
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);	//�ȴ�DMA�������
	DMA_ClearFlag(DMA1_FLAG_TC1);						//���������ɱ�־λ
}
