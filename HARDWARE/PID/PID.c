#include "stm32f10x.h"
#include "PID.h"

extern int32_t MotorA_RS,MotorB_RS;
extern int32_t MotorA_Speed,MotorB_Speed;

const double Kp = 0.7;
const double Ki = 0.07;
const double Kd = 0;

uint16_t P_Calculate(int32_t target,int32_t present,int32_t limit)
{
	if(target > limit) target = limit;
	
	int32_t err = target - present;
	double result = Kp*err + present;
	
	uint16_t data_into_PWM = (uint16_t)result*100/limit;
	
	return data_into_PWM;
}

uint16_t PI_ACalculate(int32_t target,int32_t present,int32_t limit)
{
	static double sumA = 0;
	if(target > limit) target = limit;
	
	int32_t err = target - present;
	sumA += err;
	if(sumA > 1000) sumA = 1000;
	else if(sumA < -1000) sumA = -1000;
	double result = Kp*err + Ki*sumA + present;
	
	uint16_t data_into_PWM = (uint16_t)result*100/limit;
	
	return data_into_PWM;
}

uint16_t PI_BCalculate(int32_t target,int32_t present,int32_t limit)
{
	static double sumB = 0;
	if(target > limit) target = limit;
	
	int32_t err = target - present;
	sumB += err;
	if(sumB > 1000) sumB = 1000;
	else if(sumB < -1000) sumB = -1000;
	double result = Kp*err + Ki*sumB + present;
	
	uint16_t data_into_PWM = (uint16_t)result*100/limit;
	
	return data_into_PWM;
}