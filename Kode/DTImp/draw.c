/*
 * draw.c
 *
 * Created: 12.02.2013 21:12:14
 *  Author: Øystein Smith
 */ 
#include "draw.h"
#include <util/delay.h>

void draw_color(int r, int g, int b){
	for (int i=0; i<16; i++){
			image[i*3]   = r;
			image[i*3+1] = g;
			image[i*3+2] = b;
	}
}

void clear(){
	for (int i=0; i<48; i++){
		image[i] = 0;
	}
}

void delay_ms(int t){
	for (int i=0; i<t; i++){
		_delay_ms(1);
	}
}