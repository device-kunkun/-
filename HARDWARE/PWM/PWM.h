#ifndef __PWM_H
#define __PWM_H

void PWM_Init();
void PWM_Change_Speed(uint32_t NEW_CCR);
void PWM_Change_Degree(uint32_t NEW_CCR);

#endif
