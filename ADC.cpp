/*
 * ADC.cpp
 *
 *  Created on: 17 Aug 2019
 *      Author: Connor
 */
#include "ADC.h";

void ADC_Init() {

	//Set all analogue ports to inputs
	DDRC = 0x00;

	//ADC Enable
	ADCSRA |= (1 << ADEN);
	//ADC Frequency
	ADCSRA |= (1 << ADPS0);
	ADCSRA |= (1 << ADPS1);
	ADCSRA |= (1 << ADPS2);

	// AREF = AVcc
	ADMUX |= (1 << REFS0);

	//Do first read so as to reduce future conversion time
	ADCSRA |= (1 << ADSC);
}

int ADC_Read() {
	//Start conversion
	ADCSRA |= (1 << ADSC);

	//Wait for conversion to complete
	while (ADCSRA & (1 << ADSC));

	return (ADC);
}

char ADC_Button() {
	//Change to A0
	ADMUX &= ~(1 << MUX0 | 1 << MUX1 | 1 << MUX2 | 1 << MUX3);

	//Set the frequency to 128
	ADCSRA |= (1 << ADPS0);
	ADCSRA |= (1 << ADPS1);
	ADCSRA |= (1 << ADPS2);

	int adcValue = ADC_Read();
	if (adcValue <= 40) {
		//Real value 0
		return 'R'; //Right
	}
	if (adcValue <= 110) {
		//Real value 99
		return 'U'; //Up
	}
	if (adcValue <= 270) {
		//Real value 256
		return 'D'; //Down
	}
	if (adcValue <= 420) {
		//Real value 410
		return 'L'; //Left
	}
	if (adcValue <= 650) {
		//Real value 640
		return 'S'; //Select
	}
	return 0; //Nothing selected (NULL)
}
