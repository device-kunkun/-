#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "OLED.h"

#define GPIO_TB6612 GPIOA

#define GPIO_IN1 GPIO_Pin_2
#define GPIO_IN2 GPIO_Pin_3
#define GPIO_IN3 GPIO_Pin_4
#define GPIO_IN4 GPIO_Pin_5

#define GPIO_UP(x) GPIO_WriteBit(GPIO_TB6612 ,x ,(BitAction)1);
#define GPIO_DOWN(x) GPIO_WriteBit(GPIO_TB6612 ,x ,(BitAction)0);

void TB6612_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);
	
	GPIO_InitTypeDef GPIO_TypeInstructure;
	GPIO_TypeInstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_TypeInstructure.GPIO_Pin = GPIO_IN1 | GPIO_IN2 | GPIO_IN3 | GPIO_IN4 ;
	GPIO_TypeInstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA , &GPIO_TypeInstructure);
	
	PWM_Init();
}

void TB6612_Advance(void)
{
	GPIO_UP(GPIO_IN1);
	GPIO_DOWN(GPIO_IN2);
	GPIO_UP(GPIO_IN3);
	GPIO_DOWN(GPIO_IN4);
}

void TB6612_Back(void)
{
	GPIO_UP(GPIO_IN2);
	GPIO_DOWN(GPIO_IN1);
	GPIO_UP(GPIO_IN4);
	GPIO_DOWN(GPIO_IN3);
}
//PWM由PWMA和PWMB控制通电占空比，这样的话就无需更改IN，使其再通点是仍然能维持之前的状态
