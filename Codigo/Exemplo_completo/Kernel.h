#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <msp430g2553.h>
#include <legacymsp430.h>

typedef void(*ptrFunc)(void);

#define BUFFSIZE 16
ptrFunc pool[BUFFSIZE];
int ini;
int fim;

#define next(elem, tam) ((elem+1)%tam)

void Null_func(void);
void InicializaKernel(void);
void AddKernel(ptrFunc newFunc);
void ExecutaKernel(void);
interrupt(TIMER1_A1_VECTOR) TA1_ISR(void);

#endif