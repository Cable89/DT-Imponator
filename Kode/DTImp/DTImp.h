/*
 * DTImp.h
 *
 * Created: 12.02.2013 21:44:29
 *  Author: Øystein Smith
 */ 


#ifndef DTIMP_H_
#define DTIMP_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define F_CPU 8000000

#define PORT_ONE PORTA
#define PORT_TWO PORTC
#define PORT_THREE PORTD
#define LINE_EN PORTF

volatile uint8_t image[48];

#endif /* DTIMP_H_ */