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

#include "int.h"
#include "functions.h"

#define TIMER_BASEADDR 	XPAR_MYIP_TIMER_COUNTER_0_S00_AXI_BASEADDR

#define TIMER_ID 		XPAR_MYIP_TIMER_COUNTER_0_DEVICE_ID
#define MOTOR_ID 		XPAR_STEPPING_MOTOR_DEVICE_ID
#define BTN_ID			XPAR_BUTTON_DEVICE_ID
#define PHOTO_ID		XPAR_PHOTO_INTERRUPTER_DEVICE_ID

XGpio gpio_instance0;	// motor
XGpio gpio_instance1;	// btn
XGpio gpio_instance2;	// photo

volatile unsigned int *tim_reg;		// timer register pointer

int main()
{
    init_platform();

    /* timer/counter register init */
	tim_reg = (volatile unsigned int *)TIMER_BASEADDR;
	tim_reg[0] = 0b01;	// 01 = timer mode, 10 = pwm mode
	tim_reg[1] = 999;	// prescalor
	tim_reg[2] = 199;	// max count value

	/* gpio init */
	XGpio_Config *cfg_ptr;

	cfg_ptr = XGpio_LookupConfig(MOTOR_ID);
	XGpio_CfgInitialize(&gpio_instance0, cfg_ptr, cfg_ptr->BaseAddress);
	cfg_ptr = XGpio_LookupConfig(BTN_ID);
	XGpio_CfgInitialize(&gpio_instance1, cfg_ptr, cfg_ptr->BaseAddress);
	cfg_ptr = XGpio_LookupConfig(PHOTO_ID);
	XGpio_CfgInitialize(&gpio_instance2, cfg_ptr, cfg_ptr->BaseAddress);

	XGpio_SetDataDirection(&gpio_instance0, MOTOR_CH, 0);		// motor output
	XGpio_SetDataDirection(&gpio_instance1, BTN_CH, 0b1111);	// button input
	XGpio_SetDataDirection(&gpio_instance2, PHOTO_CH, 0b111);	// photo interrupt input

	/* interrupt init */
	IntInit();

    while(1){
    	CheckFloor();
    }

    cleanup_platform();
    return 0;
}
