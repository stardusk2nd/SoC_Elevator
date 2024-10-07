#include "functions.h"

bool direction = false;		// motor direction
bool start = false;			// motor start
uint8_t CurrentFloor = 1;
uint8_t TargetFloor = 1;

void RotateStep(uint8_t i){
	if(!direction){
		XGpio_DiscreteWrite(&gpio_instance0, MOTOR_CH, 0b0001 << i);	// down stair
	}
	else{
		XGpio_DiscreteWrite(&gpio_instance0, MOTOR_CH, 0b1000 >> i);	// up stair
	}
}

void CheckFloor(){
	if(CurrentFloor != TargetFloor){
		start = true;
		if(CurrentFloor < TargetFloor){
			direction = 1;
		}
		else{
			direction = 0;
		}
	}
	else
		start = false;
}
