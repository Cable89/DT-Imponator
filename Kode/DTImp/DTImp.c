/*
 * DTImp.c
 *
 * Created: 07.02.2013 17:26:09
 *  Author: Øystein Smith
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "DTImp.h"
#include "draw.h"

void init();

volatile uint8_t oneTemp = 0;
volatile uint8_t twoTemp = 0;
volatile uint8_t threeTemp = 0;

volatile uint8_t *currentRow = &image;
volatile uint8_t currentRowNo = 0;
volatile uint8_t currentBitNo = 0;
volatile uint8_t compareMask = (1<<7);

int main(void){
	init();
	sei();
	clear();
    while(1){
        draw_color(150,0,0);
		delay_ms(1000);
		clear();
		draw_color(0,150,0);
		delay_ms(1000);
		clear();
		draw_color(0,0,150);
		delay_ms(1000);
		clear();
		draw_color(80,100,0);
		delay_ms(1000);
		clear();
    }
}

void init(){
	DDRA = 0xff;
	DDRC = 0xff;
	DDRD = 0xff;
	DDRF = 0x03;
	
	TCCR0 = (1<<WGM01)/*|(1<<CS01)|(1<<CS00)*/|(1<<CS02); //WGM-bit setter på CTC(clear on time compare), CS02 setter prescaler til 64.
	TIMSK = (1<<OCIE0); //Setter til å interrupte på compare.
	OCR0 = (1<<7);
}

ISR(TIMER0_COMP_vect){
	PORT_ONE = 0;
	PORT_TWO = 0;
	PORT_THREE = 0;
	LINE_EN &= ~((1 << 0)|(1 << 1));
	
	oneTemp = 0;
	twoTemp = 0;
	threeTemp = 0;
	
	compareMask = compareMask>>1; //Flytter ocr-register slik at vi går fra 128 til 64 til 32 etc.
	if(!compareMask){
		compareMask = 1<<7; // 128
		currentRowNo++;
		currentRow = currentRow + 24;
		if(currentRowNo > 1){
			currentRowNo = 0;
			currentRow = &image;
		}
	}
		
	if(currentRow[0]&compareMask)  twoTemp   |= (1 << 6); //R0
	if(currentRow[1]&compareMask)  twoTemp   |= (1 << 5);
	if(currentRow[2]&compareMask)  twoTemp   |= (1 << 4);
	if(currentRow[3]&compareMask)  oneTemp   |= (1 << 6); //R1
	if(currentRow[4]&compareMask)  oneTemp   |= (1 << 7);
	if(currentRow[5]&compareMask)  twoTemp   |= (1 << 7);
	if(currentRow[6]&compareMask)  oneTemp   |= (1 << 0); //R2
	if(currentRow[7]&compareMask)  oneTemp   |= (1 << 1);
	if(currentRow[8]&compareMask)  oneTemp   |= (1 << 2);
	if(currentRow[9]&compareMask)  oneTemp   |= (1 << 3); //R3
	if(currentRow[10]&compareMask) oneTemp   |= (1 << 4);
	if(currentRow[11]&compareMask) oneTemp   |= (1 << 5);
	if(currentRow[12]&compareMask) twoTemp   |= (1 << 3); //R4
	if(currentRow[13]&compareMask) twoTemp   |= (1 << 2);
	if(currentRow[14]&compareMask) twoTemp   |= (1 << 1);
	if(currentRow[15]&compareMask) twoTemp   |= (1 << 0); //R5
	if(currentRow[16]&compareMask) threeTemp |= (1 << 7);
	if(currentRow[17]&compareMask) threeTemp |= (1 << 6);
	if(currentRow[18]&compareMask) threeTemp |= (1 << 2); //R6
	if(currentRow[19]&compareMask) threeTemp |= (1 << 1);
	if(currentRow[20]&compareMask) threeTemp |= (1 << 0);
	if(currentRow[21]&compareMask) threeTemp |= (1 << 3); //R7
	if(currentRow[22]&compareMask) threeTemp |= (1 << 4);
	if(currentRow[23]&compareMask) threeTemp |= (1 << 5);
		
	PORT_ONE = oneTemp;
	PORT_TWO = twoTemp;
	PORT_THREE = threeTemp;
	LINE_EN |= (1 << currentRowNo);
	OCR0 = compareMask; // Remove +128
	TCNT0 = 0; //Setter timer til null.
	
}