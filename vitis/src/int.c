#include "int.h"

XIntc intc_instance;
uint8_t i = 0;

void TIMER_ISR(void *CallBackRef){
	if(start){
		RotateStep(i);
		if(i < 3)
			i++;
		else
			i = 0;
	}
}

// tx complete interrupt
void ISR_SPI(void *CallBackRef){
	CONTROL &= ~(1 << ENABLE);
	TxDone = true;
}

void BTN_ISR(void *CallBackRef){
	XGpio *Gpio_ptr = (XGpio *)CallBackRef;

	uint8_t btn_state = XGpio_DiscreteRead(Gpio_ptr, BTN_CH);
	if(btn_state == 0b0001){
		TargetFloor = 1;		// 1F
	}
	else if(btn_state == 0b0010){
		TargetFloor = 2;		// 2F
	}
	else if(btn_state == 0b0100){
		TargetFloor = 3;		// 3F
	}
	else if(btn_state & (1 << 3)){
		PwmFlag = true;
	}
	ArrowFlag = true;
	CheckFloor();

	XGpio_InterruptClear(Gpio_ptr, BTN_CH);

	return;
}

void PHOTO_ISR(void *CallBackRef){
	XGpio *Gpio_ptr = (XGpio *)CallBackRef;
	uint8_t photo_state = XGpio_DiscreteRead(Gpio_ptr, PHOTO_CH);

	if(photo_state & (1 << 0)){
		CurrentFloor = 1;		// 1F
	}
	else if(photo_state & (1 << 1)){
		CurrentFloor = 2;		// 2F
	}
	else if(photo_state & (1 << 2)){
		CurrentFloor = 3;		// 3F
	}

	PrintFlag = true;
	ArrowFlag = true;
	CheckFloor();

	XGpio_InterruptClear(Gpio_ptr, PHOTO_CH);

	return;
}

void IntInit(){
	/* interrupt init */
	XIntc_Initialize(&intc_instance, INTC_ID);
	XIntc_Connect(&intc_instance, TIMER_VEC_ID, (XInterruptHandler)TIMER_ISR, (void *)NULL);
	XIntc_Connect(&intc_instance, SPI_VEC_ID, (XInterruptHandler)ISR_SPI, (void *)NULL);
	XIntc_Connect(&intc_instance, BTN_VEC_ID, 	(XInterruptHandler)BTN_ISR,   (void *)&gpio_instance1);
	XIntc_Connect(&intc_instance, PHOTO_VEC_ID, (XInterruptHandler)PHOTO_ISR, (void *)&gpio_instance2);

	XIntc_Enable(&intc_instance, TIMER_VEC_ID);
	XIntc_Enable(&intc_instance, SPI_VEC_ID);
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
