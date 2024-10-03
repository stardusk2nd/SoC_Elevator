#include "int.h"

XIntc intc_instance;

void TIMER_ISR(void *CallBackRef){
	if(start){
		RotateStep();
		if(i < 3)
			i++;
		else
			i = 0;
	}
}

void BTN_ISR(void *CallBackRef){
	XGpio *Gpio_ptr = (XGpio *)CallBackRef;

	uint8_t btn_state = XGpio_DiscreteRead(Gpio_ptr, BTN_CH);
	if(btn_state == 0b0001){
		target_floor = 1;
	}
	else if(btn_state == 0b0010){
		target_floor = 2;
	}
	else if(btn_state == 0b0100){
		target_floor = 3;
	}
	XGpio_InterruptClear(Gpio_ptr, BTN_CH);
	return;
}

void PHOTO_ISR(void *CallBackRef){
	XGpio *Gpio_ptr = (XGpio *)CallBackRef;
	uint8_t photo_state = XGpio_DiscreteRead(Gpio_ptr, PHOTO_CH);

	if(photo_state == 0b001){
		cur_floor = 1;
	}
	else if(photo_state == 0b010){
		cur_floor = 2;
	}
	else if(photo_state == 0b100){
		cur_floor = 3;
	}

	XGpio_InterruptClear(Gpio_ptr, PHOTO_CH);
	return;
}

void IntInit(){
	/* interrupt init */
	XIntc_Initialize(&intc_instance, INTC_ID);
	XIntc_Connect(&intc_instance, TIMER_VEC_ID, (XInterruptHandler)TIMER_ISR, (void *)NULL);
	XIntc_Connect(&intc_instance, BTN_VEC_ID, 	(XInterruptHandler)BTN_ISR,   (void *)&gpio_instance1);
	XIntc_Connect(&intc_instance, PHOTO_VEC_ID, (XInterruptHandler)PHOTO_ISR, (void *)&gpio_instance2);

	XIntc_Enable(&intc_instance, TIMER_VEC_ID);
	XIntc_Enable(&intc_instance, BTN_VEC_ID);
	XIntc_Enable(&intc_instance, PHOTO_VEC_ID);
	XIntc_Start(&intc_instance, XIN_REAL_MODE);

	XGpio_InterruptEnable(&gpio_instance1, BTN_CH);
	XGpio_InterruptEnable(&gpio_instance2, PHOTO_CH);
	XGpio_InterruptGlobalEnable(&gpio_instance1);
	XGpio_InterruptGlobalEnable(&gpio_instance2);

	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XIntc_InterruptHandler, (void *)&intc_instance);
	Xil_ExceptionEnable();
}
