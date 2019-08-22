#include "Shared.h";
#include "Timer.h";
#include "ADC.h";
#include "StepperMotor.h";
#include "InfraredSensor.h";

void setup();
void loop();

void Clock_Mode();
void StudentID_Mode();
void Distance_Sensor_Mode();
void Continuous_Mode();
void Step_Mode();
void Link_Mode();

unsigned char Mode = 0;
unsigned char Iteration = 0;
char Button;
char ButtonPrevious;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int main() {
	setup();
	while (1) {
		loop();
	}
	return 0;
}

void setup() {
	lcd.begin(16, 2);
	Timer2_Init();
	ADC_Init();
	Stepper_Motor_Init();

	//Serial.begin(9600);
}

void loop() {
	Button = ADC_Button();
	//Stop infinite cycles
	if ((Button == 'S') && (Button != ButtonPrevious)) {
		Mode++;
		if (Mode > 5) {
			Mode = 0;
		}
	}
	//Previous button press cleared between changing menus.
	if (Button != 0 || ButtonPrevious == 'S') {
		//Only update the previous button value if it is a new button press.
		//i.e. don't store non button presses
		//Unless previous button is select,
		// since select is for menu switching.
		ButtonPrevious = Button;
	}

	switch (Mode) {
	case 1:
		StudentID_Mode();
		Delay_MS(100);
		break;
	case 2:
		//Only update the LCD every 300 ms
		if (Iteration % 3 == 0) {
			Distance_Sensor_Mode();
		}
		Delay_MS(100);
		break;
	case 3:
		Continuous_Mode();
		break;
	case 4:
		Step_Mode();
		break;
	case 5:
		Link_Mode();
		break;
	default:
	case 0:
		Clock_Mode();
		Delay_MS(100);
		break;
	}
	Iteration++;
}

void Clock_Mode() {
	char buf[6];

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("M:Clock");
	lcd.setCursor(0, 1);
	sprintf(buf, "%02d:%02d", MinuteCounter, SecondCounter);
	lcd.print(buf);
}

void StudentID_Mode() {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Connor B");

	lcd.setCursor(0, 1);
	lcd.print("12898829");
}

void Distance_Sensor_Mode() {
	char buf[6];

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("M:Distance");

	lcd.setCursor(0, 1);
	sprintf(buf, "%03d cm", IR_Sensor_Read(100));
	lcd.print(buf);

}

void Continuous_Mode() {
	char buf[16];

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("M:Continuous");

	//Only activates whilst button is currently being pressed.
	switch (Button) {
	case 'L':
		Stepper_Motor_Direction('L');
		break;
	case 'R':
		Stepper_Motor_Direction('R');
		break;
	case 'U':
		Stepper_Motor_Increment_Speed(1);
		break;
	case 'D':
		Stepper_Motor_Increment_Speed(-1);
		break;
	}

	//Remembers the previous button press.
	//Is cleared after switching between menus.
	switch (ButtonPrevious) {
	case 'L':
		lcd.setCursor(0, 1);
		lcd.print("Anti Clockwise");
		break;
	case 'R':
		lcd.setCursor(0, 1);
		lcd.print("Clockwise");
		break;
	case 'U':
	case 'D':
		lcd.setCursor(0, 1);
		sprintf(buf, "Speed:%03d%%", Stepper_Motor_Speed());
		lcd.print(buf);
		break;
	}

	Stepper_Motor_Continuous_Move();
}

char Step_Mode_Move = 0;
void Step_Mode() {
	char buf[16];
	int position = Stepper_Motor_Position();

	switch (Button) {
	case 'U':
		Stepper_Motor_Position_Set(0);
		break;
	case 'D':
		Step_Mode_Move = 1;
		break;
	case 'L':
		Stepper_Motor_Position_Decrease();
		break;
	case 'R':
		Stepper_Motor_Position_Increase();
		break;
	}

//	if (Step_Mode_Move == 1) {
//		Stepper_Motor_Position_Move();
//	}

	if (position == 0) {
		Step_Mode_Move = 0;
	}

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("M:Step");

	lcd.setCursor(0, 1);
	sprintf(buf, "N:%04d", position);
	lcd.print(buf);

	if (Step_Mode_Move == 1) {
		int steps;
		if ((position * 2) >= 100) {
			for (int i = 0; i < 50; i++) {
				Stepper_Motor_Position_Move();
				position = Stepper_Motor_Position();
				lcd.setCursor(0, 1);
				sprintf(buf, "N:%04d", position);
				lcd.print(buf);
			}
		} else {
			int delay = 100 - (position * 2);
			for (int i = 0; i < position; i++) {
				Stepper_Motor_Position_Move();
				position = Stepper_Motor_Position();
				lcd.setCursor(0, 1);
				sprintf(buf, "N:%04d", position);
				lcd.print(buf);
			}
			Delay_MS(delay);
		}
	} else {
		Delay_MS(25);
	}
}

void Link_Mode() {
	char buf[8];
	int degrees = 0;
	int nextPosition;
	int currentPosition = Stepper_Motor_Link();
	int stepsPerRevolution = (int)MOTOR_REVOLUTION_STEPS;
	int irSensorMin = 30; //in cm
	int irSensorMax = 150; //in cm

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("M:Link");

	int irSensor = IR_Sensor_Read(50);

	if (irSensor < irSensorMin) {
		degrees = 360;
		nextPosition = stepsPerRevolution;
	} else if (irSensor > irSensorMax) {
		degrees = 0;
		nextPosition = 0;
	} else {
		degrees = 360 - ((irSensor - irSensorMin) * 3);
		nextPosition = stepsPerRevolution - ((float) (irSensor - irSensorMin) * ((float) stepsPerRevolution / (irSensorMax - irSensorMin)));
	}

	lcd.setCursor(0, 1);
	lcd.print((char) 242); //Theta Character
	sprintf(buf, "=%03d", degrees);
	lcd.print(buf);

//	Serial.print("Next Position: ");
//	Serial.println(nextPosition);
//	Serial.print("Current Position: ");
//	Serial.println(currentPosition);

	Stepper_Motor_Link_Set(nextPosition);
	if ((nextPosition - currentPosition) != 0) {
		for (int i = 0; i < 50; i++) {
			Stepper_Motor_Link_Move();
		}
	} else {
		Delay_MS(100);
	}

}

