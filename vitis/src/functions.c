#include "functions.h"

bool direction = false;		// motor direction
bool start = false;			// motor start
uint8_t cur_floor = 2;
uint8_t target_floor = 1;
uint8_t i = 0;

void RotateStep(){
	if(!direction){
		XGpio_DiscreteWrite(&gpio_instance0, MOTOR_CH, 0b0001 << i);	// down stair
	}
	else{
		XGpio_DiscreteWrite(&gpio_instance0, MOTOR_CH, 0b1000 >> i);	// up stair
	}
}

void CheckFloor(){
	if(cur_floor != target_floor){
		start = true;
		if(cur_floor < target_floor){
			direction = 1;
		}
		else{
			direction = 0;
		}
	}
	else
		start = false;
}
