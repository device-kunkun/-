#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "delay.h"
#include "TB6612.h"
#include "UART.h"
#include "PWM.h"

//degree 20-250(range) / 2000(total)
//实际上 150趋近于0度 100-230 advice range
//因为设计原因 角度增量并不是简单的y=kx

int8_t LR = 0;
int Speed = 0;

int main(void)
{
	//Encoder_Init();
	OLED_Init();
	delay_init();
	TB6612_Init();
	OLED_Clear();
	Serial_Init();
	
	TB6612_Advance();
	OLED_ShowString(1,1,"dir:");
	OLED_ShowString(2,1,"speed:");
	
	while(1)
	{
		OLED_ShowSignedNum(1,5,LR,2);
		OLED_ShowSignedNum(2,7,Speed,4);
		
		if(LR == -1) PWM_Change_Degree(100);
		if(LR == 1) PWM_Change_Degree(230);
		if(LR == 0) PWM_Change_Degree(150);
		
		if(Speed > 0) {PWM_Change_Speed(Speed); TB6612_Advance();}
		if(Speed < 0) {PWM_Change_Speed(-Speed); TB6612_Back();}
		if(Speed == 0) PWM_Change_Speed(0);
	}
}