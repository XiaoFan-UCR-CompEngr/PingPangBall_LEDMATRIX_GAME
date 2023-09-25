/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
 //ass1+2 speed up and botton control.      https://youtu.be/W09EiYxxPaw
//ass3 random AI control top side           https://youtu.be/iXHnhQh8STk

  #define F_CPU 1000000      
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "keypad.h"
#include "bit.h"
#include "scheduler.h"
#include "timer.h"
#include "stdlib.h"
#include <stdio.h>
#endif
static int PORT[2];
static int ballRow;
static int ballCol;
static int ROW[4] ={0x38,0x28,0x28,0x38};
static int mode;
	static unsigned char str;	
	static unsigned char left;	
static unsigned char right ;	
static int speed;
void A2D_init() {
      ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant
	//	    analog to digital conversions.
}

enum Demo_States {shift,Shift2};
int Demo_Tick_1(int state) {

	static unsigned char pattern = 0x1C;	// LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0x80;  	// Row(s) displaying pattern. 
		unsigned short input = ADC;	
		unsigned char random = rand() % 6;				// 0: display pattern on row
	static unsigned char countA1 = 0x00;	
	static unsigned char countA2 = 0x00;	
static unsigned char countA3 = 0x00;	
static unsigned char countA4 = 0x00;					// 1: do NOT display pattern on row
	// Transitions
	switch (state) {
		case shift:
		if(PINA & 0x04)
		state = Shift2;
		break;
		case Shift2:
		break;

		default:	
state = shift;
			break;
	}	
	// Actions
	switch (state) {
		case shift:	
			if(random < 4)
			{
			for(random;random>0;random--)
			{pattern = pattern << 1;
			if (pattern == 0xC0) { // Reset demo 
				//pattern = 0x0;		    
				pattern = 0xE0;}}
			}
			else if (random > 3 )
			{
			for(random;random>3;random--)	
			{pattern = pattern >> (1);
			if (pattern == 0x03) { // Reset demo 
				//pattern = 0x0;		    
				pattern = 0x07;}}}
			PORT[0] = pattern;
			
		
			break; 
		case Shift2://using botton


 			if(~PINA & 0x04)
			{pattern = pattern << 1;
			if (pattern == 0xC0) { // Reset demo 
				//pattern = 0x0;		    
				pattern = 0xE0;}
			}
			else if (~PINA & 0x10)
			{pattern = pattern >> 1;
			if (pattern == 0x03) { // Reset demo 
				//pattern = 0x0;		    
				pattern = 0x07;}}
			PORT[0] = pattern;

		break;
		default: 
	break;
	}
	
	
	return state;
}
int Demo_Tick_2(int state) {
	
	static unsigned char pattern = 0x1C;	// LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0x80;  	// Row(s) displaying pattern. 
		unsigned short input = ADC;					// 0: display pattern on row
	static unsigned char countA1 = 0x00;	
	static unsigned char countA2 = 0x00;	
static unsigned char countA3 = 0x00;	
static unsigned char countA4 = 0x00;					// 1: do NOT display pattern on row
	// Transitions
	switch (state) {
		case shift:
		
		break;

		default:	
state = shift;
			break;
	}	
	// Actions
	switch (state) {
		case shift:	
			
			 if(input >600)
			{pattern = pattern << 1;
			if (pattern == 0xC0) { // Reset demo 
				//pattern = 0x0;		    
				pattern = 0xE0;}
			}
			else if (input < 500)
			{pattern = pattern >> 1;
			if (pattern == 0x03) { // Reset demo 
				//pattern = 0x0;		    
				pattern = 0x07;}}
			PORT[1] = pattern;
			
			break; 

		default: 
	break;
	}

	//PORTC = ~pattern;// Pattern to display
	//PORTD = row;		// Row(s) displaying pattern	
	
	return state;
}
int Demo_Tick_3(int state) {
	
	static unsigned char pattern = 0x08;	// LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0x08;  	// Row(s) displaying pattern. 
	
static unsigned char down = 0x01;					// 1: do NOT display pattern on row
	// Transitions
	switch (state) {
		case shift:
		
		break;

		default:	
state = shift;
			break;
	}	
	// Actions
	switch (state) {
		case shift:	
			if(~PINA & 0x08)
			{pattern = 0x08;
			row = 0x08;
			str=1;left=0;right=0;down=1;

			}







			if(str)
			{ 
				if(down)
				{row = row >> 1; //go down
				}
				else
				row = row <<1; //go up
				if(row == 0x80) //player's side
				{
					if(pattern & PORT[0])
				{ if(((pattern >> 1) & PORT[0]) && ((pattern << 1) & PORT[0]))
					{ down = !down;speed = 4;
					   str = 1;left=0;right=0;}
				else if(((pattern >> 2) & PORT[0]))
					{ down = !down;speed = 1;
					   str = 0;left=1;right=0;}
				else if(((pattern << 2) & PORT[0]))
					{ down = !down;speed = 1;
					   str = 0;left=0;right=1;}
				
				}//if hit
				else {PORTB = 0xFF;}
				}
				if(row == 0x01) //AI's side
				{
					if(pattern & PORT[1])
				{ if(((pattern >> 1) & PORT[1]) && ((pattern << 1) & PORT[1]))
					{ down = !down;speed = 4;
					   str = 1;left=0;right=0;}
				else if(((pattern >> 2) & PORT[1]))
					{ down = !down;speed = 1;
					   str = 0;left=1;right=0;}
				else if(((pattern << 2) & PORT[1]))
					{ down = !down;speed = 1;
					   str = 0;left=0;right=1;}
				
				}//if hit
				else {PORTB = 0xF0;}}}
				

				else if(left)
			{ 	
				if(down)
				{row = row >> 1; //go down
				pattern = pattern >> 1; //go left
			
				if(pattern == 0x00)
				{pattern = 0x01;
				left = 0;
				right = 1;} //shift direction

				}
				else
				{row = row <<1; //go up
				pattern = pattern >> 1; //go left
			
				if(pattern == 0x00)
				{pattern = 0x01;
				left = 0;
				right = 1;}} //shift direction
			
				if(row == 0x80) //player's side
				{
					if(pattern & PORT[0])
				{ if(((pattern >> 1) & PORT[0]) && ((pattern << 1) & PORT[0]))
					{ down = !down;speed = 4;
					   str = 1;left=0;right=0;}
				else if(((pattern >> 2) & PORT[0]))
					{ down = !down;speed = 1;
					   str = 1;left=1;right=0;}
				else if(((pattern << 2) & PORT[0]))
					{ down = !down;speed = 1;
					   str = 1;left=0;right=1;}
				
				}//if hit
				else {PORTB = 0xFF;}
				}
				if(row == 0x01) //AI's side
				{
					if(pattern & PORT[1])
				{ if(((pattern >> 1) & PORT[1]) && ((pattern << 1) & PORT[1]))
					{ down = !down;speed = 4;
					   str = 1;left=0;right=0;}
				else if(((pattern >> 2) & PORT[1]))
					{ down = !down;speed = 1;
					   str = 0;left=1;right=0;}
				else if(((pattern << 2) & PORT[1]))
					{ down = !down;speed = 1;
					   str = 0;left=0;right=1;}
				
				}//if hit
				else {PORTB = 0xF0;}}}

				else if(right)
			{ 
				if(down)
				{row = row >> 1; //go down
				pattern = pattern << 1; //go r
			
				if(pattern == 0x00)
				{pattern = 0x80;
				left = 1;
				right = 0;} //shift direction

				}
				else
				{row = row <<1; //go up
				pattern = pattern << 1; //go r
			
				if(pattern == 0x00)
				{pattern = 0x80;
				left = 1;
				right = 0;}} //shift direction
			
				if(row == 0x80) //player's side
				{
					if(pattern & PORT[0])
				{ if(((pattern >> 1) & PORT[0]) && ((pattern << 1) & PORT[0]))
					{ down = !down;speed = 4;
					   str = 1;left=0;right=0;}
				else if(((pattern >> 2) & PORT[0]))
					{ down = !down;speed = 1;
					   str = 0;left=1;right=0;}
				else if(((pattern << 2) & PORT[0]))
					{ down = !down;speed = 1;
					   str = 0;left=0;right=1;}
				
				}//if hit
				else {PORTB = 0xFF;}
				}
				if(row == 0x01) //AI's side
				{
					if(pattern & PORT[1])
				{ if(((pattern >> 1) & PORT[1]) && ((pattern << 1) & PORT[1]))
					{ down = !down;speed = 4;
					   str = 1;left=0;right=0;}
				else if(((pattern >> 2) & PORT[1]))
					{ down = !down;speed = 1;
					   str = 0;left=1;right=0;}
				else if(((pattern << 2) & PORT[1]))
					{ down = !down;speed = 1;
					   str = 0;left=0;right=1;}
				
				}//if hit
				else {PORTB = 0xF0;}}}
					

					
















			ballRow = row;
			ballCol = pattern;
			break; 
			
		default: 
	break;
	}
	
	return state;
}
int Demo_Tick_4(int state) {

	static unsigned char pattern = 0x1C;	// LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0x80;  	// Row(s) displaying pattern. 
		unsigned short input = ADC;					// 0: display pattern on row
	static unsigned char countA1 = 0x00;	
	static unsigned char countA2 = 0x00;	
static unsigned char countA3 = 0x00;	
static unsigned char countA4 = 0x00;					// 1: do NOT display pattern on row
	// Transitions
	switch (state) {
		case shift:
		
		break;

		default:	
state = shift;
			break;
	}	
	// Actions
	switch (state) {
		case shift:	
			
			 for (int n=0;n<200;n++)//execute 200 times for the eye to catch

                {

                                for (int j=0;j<2;j++)

                                {

                                                pattern = PORT[1];// ROW

                                                row = 0x01;//show half of character (COLUMN)

                                                _delay_us(500);
						
						PORTC = ~pattern;// Pattern to display
						PORTD = row;	

                                }

                               pattern=0xFF;

				 for (int i=0;i<2;i++)

                                {

                                                pattern = PORT[0];// ROW

                                                row = 0x80;//show half of character (COLUMN)

                                                _delay_us(500);
						
						PORTC = ~pattern;// Pattern to display
						PORTD = row;	

                                }
				pattern = 0xFF;
				for (int a=0;a<2;a++)

                                {

                                                pattern = ballCol;// ROW

                                                row = ballRow;//show half of character (COLUMN)

                                                _delay_us(500);
						
						PORTC = ~pattern;// Pattern to display
						PORTD = row;	

                                }
                }
			break; 

		default: 
	break;
	}

	//PORTC = ~pattern;// Pattern to display
	//PORTD = row;		// Row(s) displaying pattern	
	
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
DDRC = 0xFF; PORTC = 0x00;
DDRD = 0xFF; PORTD = 0x00;
DDRB = 0xFF; PORTB = 0x00;
DDRA - 0x00; PORTA = 0xFF;
str = 1;
speed = 4;
    /* Insert your solution below */
   static task task1,task2,task3,task4;
        task *tasks[] = {&task1,&task2,&task3,&task4};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	task1.state = 0;
	task1.period = 100;
	task1.elapsedTime = task1.period;
	task1.TickFct = &Demo_Tick_1;
	task2.state = 0;
	task2.period = 100;
	task2.elapsedTime = task2.period;
	task2.TickFct = &Demo_Tick_2;
	task3.state = 0;
	task3.period = 100;
	task3.elapsedTime = task3.period;
	task3.TickFct = &Demo_Tick_3;
	task4.state = 0;
	task4.period = 100;
	task4.elapsedTime = task4.period;
	task4.TickFct = &Demo_Tick_4;
	
     TimerSet(100);
     TimerOn();	
    unsigned short i;
srand(time(NULL));
A2D_init();
   while(1){ 

	 for(i = 0; i < numTasks; i++)
      {
          if(tasks[i]->elapsedTime == tasks[i]->period)
          {
            tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
            tasks[i]->elapsedTime = 0;
          }
          tasks[i]->elapsedTime += 100;
	  
      } // for
     while(!TimerFlag){}
     TimerFlag = 0;
    
   }
     return 1;
}

