#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "io430.h"

typedef void(*ptrFunc)(void);

#define BUFFSIZE 16
extern ptrFunc pool[BUFFSIZE];
extern int ini;
extern int fim;

#define next(elem, tam) ((elem+1)%tam)

void Null_func(void);
void InicializaKernel(void);
void AddKernel(ptrFunc newFunc);
void ExecutaKernel(void);
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TA1_ISR(void);

#endif