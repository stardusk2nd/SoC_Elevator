/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xintc.h"
#include "xil_exception.h"
#include "stdbool.h"

#define MOTOR_CH 		1
#define BTN_CH 			1
#define PHOTO_CH 		1

#define TIMER_BASEADDR 	XPAR_MYIP_TIMER_COUNTER_0_S00_AXI_BASEADDR

#define TIMER_ID 		XPAR_MYIP_TIMER_COUNTER_0_DEVICE_ID
#define MOTOR_ID 		XPAR_STEPPING_MOTOR_DEVICE_ID
#define BTN_ID			XPAR_BUTTON_DEVICE_ID
#define PHOTO_ID		XPAR_PHOTO_INTERRUPTER_DEVICE_ID

#define INTC_ID 		XPAR_INTC_0_DEVICE_ID
#define TIMER_VEC_ID	XPAR_INTC_0_MYIP_TIMER_COUNTER_0_VEC_ID
#define MOTOR_VEC_ID	XPAR_INTC_0_GPIO_2_VEC_ID
#define BTN_VEC_ID		XPAR_INTC_0_GPIO_0_VEC_ID
#define PHOTO_VEC_ID	XPAR_INTC_0_GPIO_1_VEC_ID

XGpio gpio_instance0;	// motor
XGpio gpio_instance1;	// btn
XGpio gpio_instance2;	// photo
XIntc intc_instance;

volatile unsigned int *tim_reg;
uint8_t i = 0;
bool direction = false;
bool start = false;
uint8_t cur_floor = 1;
uint8_t target_floor = 1;

void RotateStep();
void TIMER_ISR(void *CallBackRef);
void BTN_ISR(void *CallBackRef);
void PHOTO_ISR(void *CallBackRef);
void CheckFloor();

int main()
{
    init_platform();

    /* timer/counter register init */
	tim_reg = (volatile unsigned int *)TIMER_BASEADDR;
	tim_reg[0] = 0b01;
	tim_reg[1] = 999;
	tim_reg[2] = 199;

	/* gpio init */
	XGpio_Config *cfg_ptr;

	cfg_ptr = XGpio_LookupConfig(MOTOR_ID);
	XGpio_CfgInitialize(&gpio_instance0, cfg_ptr, cfg_ptr->BaseAddress);
	cfg_ptr = XGpio_LookupConfig(BTN_ID);
	XGpio_CfgInitialize(&gpio_instance1, cfg_ptr, cfg_ptr->BaseAddress);
	cfg_ptr = XGpio_LookupConfig(PHOTO_ID);
	XGpio_CfgInitialize(&gpio_instance2, cfg_ptr, cfg_ptr->BaseAddress);

	XGpio_SetDataDirection(&gpio_instance0, MOTOR_CH, 0);
	XGpio_SetDataDirection(&gpio_instance1, BTN_CH, 0b1111);
	XGpio_SetDataDirection(&gpio_instance2, PHOTO_CH, 0b111);

	/* interrupt init */
	XIntc_Initialize(&intc_instance, INTC_ID);
	XIntc_Connect(&intc_instance, TIMER_VEC_ID, (XInterruptHandler)TIMER_ISR, (void *)NULL);
//	XIntc_Connect(&intc_instance, MOTOR_VEC_ID, (XInterruptHandler)MOTOR_ISR, (void *)&gpio_instance0);
	XIntc_Connect(&intc_instance, BTN_VEC_ID, 	(XInterruptHandler)BTN_ISR,   (void *)&gpio_instance1);
	XIntc_Connect(&intc_instance, PHOTO_VEC_ID, (XInterruptHandler)PHOTO_ISR, (void *)&gpio_instance2);

	XIntc_Enable(&intc_instance, TIMER_VEC_ID);
//	XIntc_Enable(&intc_instance, MOTOR_VEC_ID);
	XIntc_Enable(&intc_instance, BTN_VEC_ID);
	XIntc_Enable(&intc_instance, PHOTO_VEC_ID);
	XIntc_Start(&intc_instance, XIN_REAL_MODE);

//	XGpio_InterruptEnable(&gpio_instance0, MOTOR_CH);
	XGpio_InterruptEnable(&gpio_instance1, BTN_CH);
	XGpio_InterruptEnable(&gpio_instance2, PHOTO_CH);
	XGpio_InterruptGlobalEnable(&gpio_instance1);
	XGpio_InterruptGlobalEnable(&gpio_instance2);

	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XIntc_InterruptHandler, (void *)&intc_instance);
	Xil_ExceptionEnable();

    while(1){
    	CheckFloor();
    }

    cleanup_platform();
    return 0;
}

void RotateStep(){
	if(!direction){
		XGpio_DiscreteWrite(&gpio_instance0, MOTOR_CH, 0b0001 << i);
	}
	else{
		XGpio_DiscreteWrite(&gpio_instance0, MOTOR_CH, 0b1000 >> i);
	}
}

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

//	uint8_t btn_state = XGpio_DiscreteRead(Gpio_ptr, BTN_CH);
//	if(btn_state){
		if(XGpio_DiscreteRead(Gpio_ptr, BTN_CH) == 0b0001){
			target_floor = 1;
		}
		else if(XGpio_DiscreteRead(Gpio_ptr, BTN_CH) == 0b0010){
			target_floor = 2;
		}
		else if(XGpio_DiscreteRead(Gpio_ptr, BTN_CH) == 0b0100){
			target_floor = 3;
		}
//	}
//	if(btn_state == 0b0001)
//		direction = true;
//	else if(btn_state == 0b0010)
//		direction = false;
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
