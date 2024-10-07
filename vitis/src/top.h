#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xintc.h"
#include "xil_exception.h"
#include "stdbool.h"

/* define variable */
#define MOTOR_CH 		1
#define BTN_CH 			1
#define PHOTO_CH 		1

extern XGpio gpio_instance0;	// motor
extern XGpio gpio_instance1;	// btn
extern XGpio gpio_instance2;	// photo
extern bool start;
extern uint8_t i;
