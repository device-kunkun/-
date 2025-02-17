#include "stm32f10x.h"     
#include "delay.h"    // Device header

extern uint8_t Page;

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PB6×÷Îª°´¼ü
}

void Key_GetNum(void)
{
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0)
	{
		delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0);
		delay_ms(20);
		Page ++;
		Page %= 2;
	}
}
