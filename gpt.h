/*
 * gpt.h
 *
 * Created: 14.01.2016 14:59:33
 *  Author: Tom
 */ 

#include <avr/io.h>

#ifndef GPT_H_
#define GPT_H_

#define GPT_NUM_TIMERS	15

typedef void (*gptFunc)(void);

struct GPTimerStruct
{
	uint16_t overflowTime;
	uint16_t remainingTime;
	gptFunc callBack;
};

/************************************************************************/
/*		gpt_init                                                        */
/*																		*/
/*Function:																*/
/*		initialies general purpose timer								*/
/*																		*/
/*Pre Conditions:														*/
/*		None															*/
/*																		*/
/*Paramete:																*/
/*		None															*/
/*																		*/
/*Return																*/
/*		None															*/
/************************************************************************/
void gpt_init();

/************************************************************************/
/*		getTime                                                         */
/*																		*/
/*Function:																*/
/*		returnes actuall Time in ms										*/
/*																		*/
/*Pre Conditions:														*/
/*		gpt_init														*/
/*																		*/
/*Paramete:																*/
/*		None															*/
/*																		*/
/*Return																*/
/*		actual Time														*/
/************************************************************************/
uint32_t getTime();

/************************************************************************/
/*		gpt_requestTimer                                                */
/*																		*/
/*Function:																*/
/*		request & start new 'general purpose timer'						*/
/*																		*/
/*Pre Conditions:														*/
/*		gpt_init														*/
/*																		*/
/*Paramete:																*/
/*		OVFtime: Time in ms untile Callback get started					*/
/*		callBack: Function with get started when OVFtime got achieved	*/
/*																		*/
/*Return																*/
/*		TimerID															*/
/************************************************************************/
int8_t gpt_requestTimer(uint16_t OVFtime, gptFunc callBack);

/************************************************************************/
/*		gpt_releaseTime                                                 */
/*																		*/
/*Function:																*/
/*		stops & releases the 'general purpose timer' with this TimerID	*/
/*																		*/
/*Pre Conditions:														*/
/*		gpt_init														*/
/*		gpt_requestTimer												*/
/*																		*/
/*Paramete:																*/
/*		TimerID: ID of the Timer with will be stoped and released		*/
/*																		*/
/*Return																*/
/*		None															*/
/************************************************************************/
void gpt_releaseTime(int8_t timerID);

/************************************************************************/
/*		gpt_setTime		                                                */
/*																		*/
/*Function:																*/
/*		changes OVFtime of a 'general purpose timer' with this TimerID	*/
/*																		*/
/*Pre Conditions:														*/
/*		gpt_init														*/
/*		gpt_requestTimer												*/
/*																		*/
/*Paramete:																*/
/*		OVFtime: new Time in ms untile Callback get started				*/
/*		timeID: ID of the Timer with gets the new OVFtime				*/
/*																		*/
/*Return																*/
/*		TimerID															*/
/************************************************************************/
void gpt_setTime(uint16_t OVFtime, int8_t timeID);

void delay(uint16_t time);
//void delay_us(uint16_t time);

#endif /* GPT_H_ */