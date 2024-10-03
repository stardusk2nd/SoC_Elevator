#include "functions.h"

bool direction = false;
bool start = false;
uint8_t cur_floor = 1;
uint8_t target_floor = 1;
uint8_t i = 0;

void RotateStep(){
	if(!direction){
		XGpio_DiscreteWrite(&gpio_instance0, MOTOR_CH, 0b0001 << i);
	}
	else{
		XGpio_DiscreteWrite(&gpio_instance0, MOTOR_CH, 0b1000 >> i);
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
