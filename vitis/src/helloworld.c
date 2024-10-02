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

#define STEPPER_CH 1
#define BTN_CH 2

#define TIMER_BASEADDR XPAR_MYIP_TIMER_COUNTER_0_S00_AXI_BASEADDR
#define TIMER_ID XPAR_MYIP_TIMER_COUNTER_0_DEVICE_ID

#define GPIO_ID XPAR_AXI_GPIO_1_DEVICE_ID

#define INTC_ID XPAR_INTC_0_DEVICE_ID
#define TIMER_VEC_ID XPAR_INTC_0_MYIP_TIMER_COUNTER_0_VEC_ID
#define BTN_VEC_ID XPAR_INTC_0_GPIO_0_VEC_ID

XGpio stepper_instance;

uint8_t i = 0;
bool direction = 0;

void RotateStep(){
	if(!direction){
		XGpio_DiscreteWrite(&stepper_instance, STEPPER_CH, 0b0001 << i);
	}
	else{
		XGpio_DiscreteWrite(&stepper_instance, STEPPER_CH, 0b1000 >> i);
	}
}

void TIMER_ISR(void *CallBackRef){
	XGpio *Gpio_ptr = (XGpio *)CallBackRef;

//	XGpio_DiscreteWrite(&stepper_instance, STEPPER_CH, 0b0001 << i);
	RotateStep();
	if(i < 3)
		i++;
	else
		i = 0;
}

void BTN_ISR(void *CallBackRef){
	XGpio *Gpio_ptr = (XGpio *)CallBackRef;

	if(XGpio_DiscreteRead(Gpio_ptr, BTN_CH)){
		if(XGpio_DiscreteRead(Gpio_ptr, BTN_CH) == 0b0001){
			direction = 0;
		}
		else if(XGpio_DiscreteRead(Gpio_ptr, BTN_CH) == 0b0010){
			direction = 1;
		}
//		else if(XGpio_DiscreteRead(Gpio_ptr, BTN_CH) == 0b0100){
//
//		}
	}

	XGpio_InterruptClear(Gpio_ptr, BTN_CH);
	return;
}

XIntc intc_instance;
volatile unsigned int *tim_reg;

int main()
{
    init_platform();

    XGpio_Config *cfg_ptr;
	u32 data;

	tim_reg = (volatile unsigned int *)TIMER_BASEADDR;
	tim_reg[0] = 0b01;
	tim_reg[1] = 999;
	tim_reg[2] = 199;

	cfg_ptr = XGpio_LookupConfig(GPIO_ID);
	XGpio_CfgInitialize(&stepper_instance, cfg_ptr, cfg_ptr->BaseAddress);
	XGpio_SetDataDirection(&stepper_instance, STEPPER_CH, 0);

	XIntc_Initialize(&intc_instance, INTC_ID);
	XIntc_Connect(&intc_instance, TIMER_VEC_ID, (XInterruptHandler)TIMER_ISR, (void *)NULL);
	XIntc_Enable(&intc_instance, TIMER_VEC_ID);
	XIntc_Start(&intc_instance, XIN_REAL_MODE);

	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XIntc_InterruptHandler, (void *)&intc_instance);
	Xil_ExceptionEnable();

    while(1){

    }

    cleanup_platform();
    return 0;
}
