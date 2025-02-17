#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "PID.h"

//PA8 9为TIM1，作为编码器A的输入捕获
//PC6 7为TIM8，作为编码器B的输入捕获

extern int32_t MotorA_RS,MotorB_RS;
extern int32_t MotorA_Speed,MotorB_Speed;
extern int32_t TargetA_Speed,TargetB_Speed;

void EncoderA_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1 ,&TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitInstructure;
	TIM_ICStructInit(&TIM_ICInitInstructure);
	TIM_ICInitInstructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitInstructure.TIM_ICFilter = 0x0;
	TIM_ICInit(TIM1 ,&TIM_ICInitInstructure);
	TIM_ICInitInstructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitInstructure.TIM_ICFilter = 0x0;
	TIM_ICInit(TIM1 ,&TIM_ICInitInstructure);
	
	TIM_EncoderInterfaceConfig(TIM1 ,TIM_EncoderMode_TI12 ,TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM1 ,ENABLE);
}

void EncoderB_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC , &GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM8 ,&TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitInstructure;
	TIM_ICStructInit(&TIM_ICInitInstructure);
	TIM_ICInitInstructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitInstructure.TIM_ICFilter = 0x0;
	TIM_ICInit(TIM8 ,&TIM_ICInitInstructure);
	TIM_ICInitInstructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitInstructure.TIM_ICFilter = 0x0;
	TIM_ICInit(TIM8 ,&TIM_ICInitInstructure);
	
	TIM_EncoderInterfaceConfig(TIM8 ,TIM_EncoderMode_TI12 ,TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM8 ,ENABLE);
}

//以下为速度计算部分 TIM2及计速度
void Timer2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TimeBaseStruct;
	TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBaseStruct.TIM_Period = 720-1;
	TimeBaseStruct.TIM_Prescaler = 10000-1;
	TimeBaseStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TimeBaseStruct);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM2,ENABLE);
}

//int32_t time=0;

void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		int32_t tmpA = 32768 - (int32_t)TIM_GetCounter(TIM1);
		int32_t tmpB = (int32_t)TIM_GetCounter(TIM8) - 32768;
		MotorA_RS = tmpA*60/132;
		MotorB_RS = tmpB*60/132;
		
		MotorA_Speed = tmpA*65*3.14159/132;
		MotorB_Speed = tmpB*65*3.14159/132;
		TIM_SetCounter(TIM1, 32768);
		TIM_SetCounter(TIM8, 32768);
		
		uint16_t data_into_PWMA = PI_ACalculate(TargetA_Speed ,MotorA_Speed ,Spd_Limit);
		PWM_ChangeA_CCR(data_into_PWMA);
		
		uint16_t data_into_PWMB = PI_BCalculate(TargetB_Speed ,MotorB_Speed ,Spd_Limit);
		PWM_ChangeB_CCR(data_into_PWMB);
		
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

void Encoder_Init()
{
	EncoderA_Init();
	EncoderB_Init();
	TIM_SetCounter(TIM1, 32768);
	TIM_SetCounter(TIM8, 32768);
	Timer2_Init();
}
