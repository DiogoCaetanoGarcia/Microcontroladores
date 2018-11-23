#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <msp430g2553.h>
#include <legacymsp430.h>
#include "Servo.h"
#include "Kernel.h"

#define BTN BIT3

extern int liga_servo;

void Config_BTN(void);
void Debounce_BTN();
interrupt(PORT1_VECTOR) Interrupcao_P1(void);

#endif