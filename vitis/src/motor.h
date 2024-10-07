#include "top.h"

/* define variable */
#define MOTOR_CH 		1
#define BTN_CH 			1
#define PHOTO_CH 		1

void RotateStep(uint8_t i);		// motor drive function
void CheckFloor();		// check elevator location

extern XGpio gpio_instance0;	// motor
