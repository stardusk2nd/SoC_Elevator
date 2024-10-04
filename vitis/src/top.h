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

#define INTC_ID 		XPAR_INTC_0_DEVICE_ID
#define TIMER_VEC_ID	XPAR_INTC_0_MYIP_TIMER_COUNTER_0_VEC_ID
#define MOTOR_VEC_ID	XPAR_INTC_0_GPIO_2_VEC_ID
#define BTN_VEC_ID		XPAR_INTC_0_GPIO_0_VEC_ID
#define PHOTO_VEC_ID	XPAR_INTC_0_GPIO_1_VEC_ID

extern XGpio gpio_instance0;	// motor
extern XGpio gpio_instance1;	// btn
extern XGpio gpio_instance2;	// photo
extern bool start;
extern uint8_t i;
extern uint8_t cur_floor;
extern uint8_t target_floor;
