#ifndef __PID_H
#define __PID_H

#define Spd_Limit 1200
#define Rev_Limit 355

uint16_t P_Calculate(int32_t target,int32_t present,int32_t limit);

#endif
