/*
 * Timer.cpp
 *
 *  Created on: 17 Aug 2019
 *      Author: Connor
 */
#include "Timer.h";

unsigned int MillisecondCounter = 0;
unsigned int SecondCounter = 0;
unsigned int MinuteCounter = 0;
volatile unsigned int MS_Delay = 0;

void Timer2_Init() {
	// Compare Clock Mode
	TCCR2B |= (1 << WGM01);
	// Compare Value
	//OCR2A = 124;
	OCR2B = 124;
	//128 Pre-Scaler
	TCCR2B |= (1 << CS20);
	TCCR2B &= ~(1 << CS21);
	TCCR2B |= (1 << CS22);

	//Enable Timer2 Interrupts
	TIMSK2 |= (1 << OCIE2B);
	//Enable Global Interrupts
	sei();
}

int Delay_MS(int ms_delay) {
	MS_Delay = 0;
	while (MS_Delay < ms_delay);
	return 1;
}

ISR (TIMER2_COMPB_vect) {
	MillisecondCounter += 2;
	MS_Delay++;
	if (MillisecondCounter >= 1000) {
		MillisecondCounter = 0;
		SecondCounter++;
	}
	if (SecondCounter >= 60) {
		SecondCounter = 0;
		MinuteCounter++;
	}
}
