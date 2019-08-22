/*
 * InfraredSensor.h
 *
 *  Created on: 17 Aug 2019
 *      Author: Connor
 */

#ifndef INFRAREDSENSOR_H_
#define INFRAREDSENSOR_H_

#include "Shared.h";
#include <math.h>;

void IR_Sensor_Init();
int IR_Sensor_Read(int sampleAmount);

#endif /* INFRAREDSENSOR_H_ */
