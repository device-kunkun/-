#include "stm32f10x.h"

//极其重要！！！GPIO_Remap_SWJ_Disable不要随便调用，会将所有调试端口变为普通IO口，STlink就下载不进去程序了，必须使用串口下载

void PWM_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB , &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM5);
	TIM_InternalClockConfig(TIM4);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	//TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure); 						//结构体赋初始值
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 2000-1;											//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720-1;									//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);														//结构体赋初始值以确保不会调用到未定义的数据
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;																//CCR
	TIM_OC1Init(TIM5,&TIM_OCInitStructure);
	TIM_OC2Init(TIM5,&TIM_OCInitStructure);
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	
	TIM_Cmd(TIM4,ENABLE);
	TIM_Cmd(TIM5,ENABLE);
}

void PWM_Change_Speed(uint32_t NEW_CCR)
{
	TIM_SetCompare1(TIM5 , NEW_CCR);
	TIM_SetCompare2(TIM5 , NEW_CCR);
}

void PWM_Change_Degree(uint32_t NEW_CCR)
{
	TIM_SetCompare1(TIM4 , NEW_CCR);
}