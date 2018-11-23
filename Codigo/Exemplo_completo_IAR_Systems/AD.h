#ifndef _AD_H_
#define _AD_H_

#include "io430.h"
#include "Servo.h"
#include "Charlieplex.h"

#define IN_AD BIT4
#define IN_AD_CH INCH_4

extern unsigned int escala;
extern int liga_servo;
extern unsigned int pos_servo; 

void Mede_Potenciometro(void);
void Config_AD(void);

#endif