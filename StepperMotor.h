/*
 * StepperMotor.h
 *
 *  Created on: 17 Aug 2019
 *      Author: Connor
 */

#ifndef STEPPERMOTOR_H_
#define STEPPERMOTOR_H_

#include "Shared.h";
#include "Timer.h";

void Stepper_Motor_Init();
void Stepper_Motor_Direction(char direction);

int Stepper_Motor_Speed();
int Stepper_Motor_Increment_Speed(signed char speed);
void Stepper_Motor_Continuous_Move();

int Stepper_Motor_Position();
void Stepper_Motor_Position_Increase();
void Stepper_Motor_Position_Decrease();
void Stepper_Motor_Position_Set(int position);
void Stepper_Motor_Position_Move();

void Stepper_Motor_Link_Set(int step);
int Stepper_Motor_Link();
void Stepper_Motor_Link_Move();

#endif /* STEPPERMOTOR_H_ */
