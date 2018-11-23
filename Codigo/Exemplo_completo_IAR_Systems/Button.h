#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "io430.h"
#include "Kernel.h"
#include "Servo.h"

#define BTN BIT3

extern ptrFunc pool[BUFFSIZE];
extern int ini;
extern int fim;
extern int liga_servo;

void Config_BTN(void);
void Debounce_BTN();
#pragma vector=PORT1_VECTOR
__interrupt void Interrupcao_P1(void);

#endif