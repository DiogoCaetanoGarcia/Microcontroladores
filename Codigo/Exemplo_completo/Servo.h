#ifndef _SERVO_H_
#define _SERVO_H_

#include <msp430g2553.h>

#define SERVO BIT6

extern int liga_servo;
extern unsigned int pos_servo; 

void Servo_On(void);
void Servo_Off(void);
void Atualiza_Servo(void);

#endif