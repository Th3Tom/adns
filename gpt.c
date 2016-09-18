/*
 * gpt.c
 *
 * Created: 14.01.2016 14:59:21
 *  Author: Tom
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "gpt.h"
#include "general.h"

volatile static uint32_t gptTime;
volatile static uint8_t overflowDelay = 0;
volatile static uint8_t overflowDelay_us = 0;

struct GPTimerStruct gptTimer[GPT_NUM_TIMERS];

void gpt_init()
{
	uint8_t i;
	
	/****************************************/
	/* Timer2								*/
	/*		ctc 1ms							*/
	/*		prescale:	64					*/
	/*		ctc overflow value:	250			*/
	/****************************************/
	
	//ctc
	TCCR2A |=  (1 << WGM21);
	TCCR2A &= ~(1 << WGM20);
	TCCR2B &= ~(1 << WGM22);
	
	//ctc overflow value
	OCR2A = 249;
	
	//prescaler 64
	TCCR2B |= (1 << CS22);
	TCCR2B &= ~((1 << CS20) | (1 << CS21));
	
	//enable interrupt
	TIMSK2 |= (1 << OCIE2A);
	
	gptTime = 0;
	
	for(i = 0; i < GPT_NUM_TIMERS; i++)
		gptTimer[i].overflowTime = 0;
	
	/****************************************/
	/* Timer0								*/
	/*		ctc 1us							*/
	/*		prescale:	1					*/
	/*		ctc overflow value:	16			*/
	/****************************************/
	/*
	//ctc
	TCCR0A |=  (1 << WGM10);
	TCCR0A &= ~(1 << WGM00);
	TCCR0B &= ~(1 << WGM20);
	
	//ctc overflow value
	OCR0A = 16;
	
	//prescaler 1 & start Timer 0
	//TCCR0B |= (1 << CS00);
	//TCCR0B &= ~((1 << CS02) | (1 << CS01));
	
	//enable interrupt
	TIMSK0 |= (1 << OCIE0A);
	//*/

	sei();
}

uint32_t getTime()
{
	return gptTime;
}

int8_t gpt_requestTimer(uint16_t OVFtime, gptFunc callBack)
{
	int8_t i = -1;
	
	do 
	{
		i++;
	}
	while ((gptTimer[i].overflowTime != 0) || (i >= GPT_NUM_TIMERS));
	
	if(gptTimer[i].overflowTime == 0 && i < GPT_NUM_TIMERS)
	{
		gptTimer[i].overflowTime = OVFtime;
		gptTimer[i].callBack = callBack;
		gptTimer[i].remainingTime = OVFtime;
		
		return i;
	}
	else
		return -1;
}

void gpt_releaseTime(int8_t timerID)
{
	gptTimer[timerID].overflowTime = 0;
	gptTimer[timerID].callBack = 0;
}

void gpt_setTime(uint16_t OVFtime, int8_t timeID)
{
	gptTimer[timeID].overflowTime = OVFtime;
	gptTimer[timeID].remainingTime = OVFtime;
}

void callBackDelay()
{
	overflowDelay = 1;
}

void delay(uint16_t time)
{
	uint8_t timer;
	
	overflowDelay = 0;
	timer = gpt_requestTimer(time, callBackDelay);
	while(overflowDelay == 0);
	gpt_releaseTime(timer);
}

/*
void delay_us(uint16_t time)
{
	if(time > 32000)
	time = 32000;

	uint16_t total_clocks;
	uint16_t overflows;
	uint8_t clocks;

	total_clocks = time << 1;

	overflows = total_clocks / 256;
	clocks = total_clocks % 256;

	TCNT0 = 0;
	overflowDelay_us = 0;
	
	//prescaler 8 & start Timer 0
	TCCR0B |= (1 << CS01);
	TCCR0B &= ~((1 << CS02) | (1 << CS00));
	
	while((overflowDelay_us <= overflows) || (TCNT0 < clocks));
	
	//prescaler 0 & stop Timer 0
	TCCR0B &= ~(1 << CS01);
	//TCCR0B &= ~((1 << CS02) | (1 << CS01));
}
//*/

ISR(TIMER2_COMPA_vect)
{
	uint8_t i = 0;
	
	gptTime++;
	
	for(i = 0; i < GPT_NUM_TIMERS; i++)
	{
		if(gptTimer[i].overflowTime != 0)
		{
			gptTimer[i].remainingTime--;
			
			if(gptTimer[i].remainingTime == 0)
			{
				gptTimer[i].remainingTime = gptTimer[i].overflowTime;
				(*gptTimer[i].callBack)();
			}
		}
	}
}

/*
ISR(TIMER0_COMPA_vect)
{
	overflowDelay_us++;
}
//*/
