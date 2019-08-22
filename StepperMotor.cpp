/*
 * StepperMotor.cpp
 *
 *  Created on: 17 Aug 2019
 *      Author: Connor
 */
#include "StepperMotor.h";

char StepPhase = 0;
signed int TotalStepCount = 0; // Always updated, counts number of steps from zero
unsigned int SingleStepsCount = 0; // cycles from 0 to MOTOR_REVOLUTION_STEPS.
unsigned int LinkStep = 0; // cycles from 0 to MOTOR_REVOLUTION_STEPS.
unsigned int DesiredSteps = 0; // set number of steps to move clockwise.
char StepDirection = 'R'; //L (left) or R (right)
char StepDelay = 2;
char StepSpeed = 1;
int MaxDelay = 100;

//Private
void Stepper_Motor_Move(int steps, int delay);
void Stepper_Motor_Step();

void Stepper_Motor_Init() {
	//Output for stepper motor
	DDRD |= (1 << DDD3); // Pin 4
	DDRB |= (1 << DDD3); // Pin 11
	DDRB |= (1 << DDD4); // Pin 12
	DDRB |= (1 << DDB5); // Pin 13
}

void Stepper_Motor_Direction(char direction) {
	if (direction == 'L' || direction == 'l') {
		StepDirection = 'L';
		return;
	}
	if (direction == 'R' || direction == 'r') {
		StepDirection = 'R';
		return;
	}
}

int Stepper_Motor_Speed(){
	return (51 - StepSpeed) * 2;
}

int Stepper_Motor_Increment_Speed(signed char speed) {
	speed = speed * -1;
	if (speed > 0) {
		if (StepSpeed < 50) {
			StepSpeed += speed; //Increase speed up to 50
		}
	} else if (speed < 0) {
		if (StepSpeed > 1) {
			StepSpeed += speed; //Decrease speed down to 1
		}
	}
	return (51 - StepSpeed) * 2;
}

void Stepper_Motor_Continuous_Move() {
	int delay = StepSpeed * StepDelay;
	int steps = MaxDelay / delay;

	Stepper_Motor_Move(steps, delay);
}

int Stepper_Motor_Position() {
	return DesiredSteps;
}

void Stepper_Motor_Position_Increase() {
	DesiredSteps++;
}

void Stepper_Motor_Position_Decrease() {
	if (DesiredSteps > 0) {
		DesiredSteps--;
	}
}

void Stepper_Motor_Position_Set(int position) {
	DesiredSteps = position;
}

void Stepper_Motor_Position_Move() {
	if (DesiredSteps > 0) {
		Stepper_Motor_Direction('R');
		Stepper_Motor_Move(1, StepDelay);
		DesiredSteps--;
	}
}

void Stepper_Motor_Link_Set(int step) {
	LinkStep = step;
}
int Stepper_Motor_Link() {
	return SingleStepsCount;
}
void Stepper_Motor_Link_Move() {
//	Serial.begin(9600);
//	Serial.print("Link Step: ");
//	Serial.println(LinkStep);
//	Serial.print("Current Position: ");
//	Serial.println(SingleStepsCount);

	if (LinkStep == SingleStepsCount) {
		//Do nothing at position
		return;
	}
	if (LinkStep < SingleStepsCount) {
		Stepper_Motor_Direction('L');
		SingleStepsCount--;
	} else {
		Stepper_Motor_Direction('R');
		SingleStepsCount++;
	}
	Stepper_Motor_Move(1, StepDelay);
}

void Stepper_Motor_Move(int steps, int delay) {
	for (int i = 0; i < steps; i++) {
		Stepper_Motor_Step();
		Delay_MS(delay);

	}
}

void Stepper_Motor_Step() {
	if (StepDirection == 'R') {
		StepPhase++;
		TotalStepCount++;
		if (StepPhase > 7) {
			StepPhase = 0;
		}
	} else {
		StepPhase--;
		TotalStepCount--;
		if (StepPhase < 0) {
			StepPhase = 7;
		}
	}

	switch (StepPhase) {
	case 0:
		PORTD |= (1 << PORTD3);
		PORTB &= ~(1 << PORTB3);
		PORTB &= ~(1 << PORTB4);
		PORTB &= ~(1 << PORTB5);
		break;
	case 1:
		PORTD |= (1 << PORTD3);
		PORTB |= (1 << PORTB3);
		PORTB &= ~(1 << PORTB4);
		PORTB &= ~(1 << PORTB5);
		break;
	case 2:
		PORTD &= ~(1 << PORTD3);
		PORTB |= (1 << PORTB3);
		PORTB &= ~(1 << PORTB4);
		PORTB &= ~(1 << PORTB5);
		break;
	case 3:
		PORTD &= ~(1 << PORTD3);
		PORTB |= (1 << PORTB3);
		PORTB |= (1 << PORTB4);
		PORTB &= ~(1 << PORTB5);
		break;
	case 4:
		PORTD &= ~(1 << PORTD3);
		PORTB &= ~(1 << PORTB3);
		PORTB |= (1 << PORTB4);
		PORTB &= ~(1 << PORTB5);
		break;
	case 5:
		PORTD &= ~(1 << PORTD3);
		PORTB &= ~(1 << PORTB3);
		PORTB |= (1 << PORTB4);
		PORTB |= (1 << PORTB5);
		break;
	case 6:
		PORTD &= ~(1 << PORTD3);
		PORTB &= ~(1 << PORTB3);
		PORTB &= ~(1 << PORTB4);
		PORTB |= (1 << PORTB5);
		break;
	case 7:
		PORTD |= (1 << PORTD3);
		PORTB &= ~(1 << PORTB3);
		PORTB &= ~(1 << PORTB4);
		PORTB |= (1 << PORTB5);
		break;
	default:
		StepPhase = 0;
		break;
	}
}
