#ifndef _SERVO_H_
#define _SERVO_H_

#include "io430.h"

#define SERVO BIT6

extern unsigned int pos_servo; 
extern int liga_servo;

void Servo_On(void);
void Servo_Off(void);
void Atualiza_Servo(void);

#endif