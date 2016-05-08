#ifndef _STOS_H_
#define _STOS_H_

extern double stos[];

int stack_push(double wartosc);
int stack_pop();
double stack_top();
int stack_isempty();
int stack_clear();
int stack_print();

#endif