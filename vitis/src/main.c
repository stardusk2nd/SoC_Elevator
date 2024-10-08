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
#include "motor.h"
#include "spi.h"
#include "spi_lcd.h"

#define TIMER_BASEADDR 	XPAR_MYIP_TIMER_COUNTER_0_S00_AXI_BASEADDR
#define PWM_BASEADDR	XPAR_LED_PWM_S00_AXI_BASEADDR

#define TIMER_ID 		XPAR_MYIP_TIMER_COUNTER_0_DEVICE_ID
#define MOTOR_ID 		XPAR_STEPPING_MOTOR_DEVICE_ID
#define BTN_ID			XPAR_BUTTON_DEVICE_ID
#define PHOTO_ID		XPAR_PHOTO_INTERRUPTER_DEVICE_ID

#define SPI_BASEADDR 	XPAR_SPI_TX_0_S00_AXI_BASEADDR

XGpio gpio_instance0;	// motor
XGpio gpio_instance1;	// btn
XGpio gpio_instance2;	// photo

/* custom ip's register address */
volatile uint32_t *spi_register = (volatile uint32_t *) SPI_BASEADDR;
volatile uint32_t *tim_reg = (volatile uint32_t *) TIMER_BASEADDR;
volatile uint32_t *pwm_reg = (volatile uint32_t *) PWM_BASEADDR;

bool PrintFlag = false;
bool ArrowFlag = false;
bool PwmFlag = false;
uint8_t LedState = 0;

extern bool direction;
extern bool start;

int main()
{
    init_platform();

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

	/* spi lcd module init */
	SpiLcdInit();

    /* timer/counter register init */
	tim_reg[0] = 0b01;	// 01 = timer mode, 10 = pwm mode
	tim_reg[1] = 999;	// prescalor
	tim_reg[2] = 199;	// max count value

	/* timer/counter - pwm mode register init */
	// target frequency = 1kHz
	pwm_reg[0] = 0b10;
	pwm_reg[1] = 100 - 1;
	pwm_reg[2] = 1000 - 1;
	pwm_reg[3] = 1000;

	// set the 'PrintFlag' to print out current floor, when system's rebooted
	PrintFlag = true;

    while(1){
    	// print current floor
    	if(PrintFlag){
    		PrintCurFloor(CurrentFloor);
    		PrintFlag = false;
    	}
    	// print arrow
    	if(ArrowFlag){
    		PrintArrow(direction, start);
    		ArrowFlag = false;
    	}
    	if(PwmFlag){
    		if(LedState < 3){
    			LedState++;
    		}
    		else{
    			LedState = 0;
    		}
			switch(LedState){
			case 0: pwm_reg[3] = 1000; break;
			case 1: pwm_reg[3] = 667; break;
			case 2: pwm_reg[3] = 333; break;
			case 3: pwm_reg[3] = 0; break;
			}
    		PwmFlag = false;
    	}
    }

    cleanup_platform();
    return 0;
}
