/*
 * InfraredSensor.cpp
 *
 *  Created on: 17 Aug 2019
 *      Author: Connor
 */
#include "InfraredSensor.h";
#include "ADC.h";

char DefaultSampleAmount = 30;
float SensorConstant = 10871; //9185.7;
float SensorExponent = 0.967; //0.916;

void IR_Sensor_Init() {

}

int IR_Sensor_Read(int sampleAmount) {
	unsigned long int sampleSum = 0;
	if (sampleAmount == 0)
	{
		sampleAmount = DefaultSampleAmount;
	}

	//Change to A1
	ADMUX &= ~(1 << MUX0 | 1 << MUX1 | 1 << MUX2 | 1 << MUX3);
	ADMUX |= (1 << MUX0);

	//Set the frequency to 128
	ADCSRA &= ~(1 << ADPS0);
	ADCSRA &= ~(1 << ADPS1);
	ADCSRA |= (1 << ADPS2);

	//Sample and average the sensor values
	for (int i = 0; i < sampleAmount; i++) {
		sampleSum += ADC_Read();
	}

	float sample = (sampleSum / sampleAmount);

//	Serial.begin(9600);
//	Serial.println(sample);

	int value = (int)SensorConstant * pow(sample, -SensorExponent);


	if (value > 400) {
		return 400;
	}
	if (value < 0)
	{
		return 0;
	}
	return (int)SensorConstant * pow(sample, -SensorExponent);
}
