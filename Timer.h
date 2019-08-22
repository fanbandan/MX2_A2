/*
 * Timer.h
 *
 *  Created on: 17 Aug 2019
 *      Author: Connor
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "Shared.h";

extern unsigned int MillisecondCounter;
extern unsigned int SecondCounter;
extern unsigned int MinuteCounter;
extern volatile unsigned int MS_Delay;

void Timer2_Init();
int Delay_MS(int ms_delay);

#endif /* TIMER_H_ */
