#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xintc.h"
#include "xil_exception.h"
#include "sleep.h"
#include "stdbool.h"

/* custom ip register address */
#define SPI_BASEADDR XPAR_SPI_TX_0_S00_AXI_BASEADDR

/* interrupt ID */
#define SPI_VEC_ID	XPAR_INTC_0_SPI_TX_0_VEC_ID
#define INTC_ID		XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID

/* register init */
volatile unsigned int *spi_register = (volatile unsigned int *) SPI_BASEADDR;

/* SPI register & bit defines */
#define CONTROL spi_register[0]
#define DATA_IN	spi_register[1]
#define ENABLE	0
#define DATA	1

/* functions declaration */
void ISR_SPI(void *CallBackRef);
void SPI_SendCmd(uint8_t data);
void SPI_SendData(uint8_t data);

XIntc intc_instance;

uint8_t InitSteps;
bool TxDone = false;

int main()
{
    init_platform();

    /* interrupt init */
    XIntc_Initialize(&intc_instance, INTC_ID);
    XIntc_Connect(&intc_instance, SPI_VEC_ID, (XInterruptHandler)ISR_SPI, (void *)NULL);
    XIntc_Enable(&intc_instance, SPI_VEC_ID);
    XIntc_Start(&intc_instance, XIN_REAL_MODE);
    Xil_ExceptionInit();
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XIntc_InterruptHandler, &intc_instance);
    Xil_ExceptionEnable();

    // set prescalor as 9(10-1)
    CONTROL = 0b0000100100;

    SPI_SendCmd(0x11);
    SPI_SendCmd(0x29);
    SPI_SendCmd(0x3a);
    SPI_SendCmd(0x55);

    SPI_SendCmd(0x2a);
    SPI_SendData(0x00);
    SPI_SendData(0x00);
    SPI_SendData(0x00);
    SPI_SendData(0x77);

    SPI_SendCmd(0x2b);
    SPI_SendData(0x00);
    SPI_SendData(0x00);
    SPI_SendData(0x00);
    SPI_SendData(0x9f);

    SPI_SendCmd(0x2c);

    while(1){
    	SPI_SendData(0xff);
    }

    cleanup_platform();
    return 0;
}

// tx complete interrupt
void ISR_SPI(void *CallBackRef){
	CONTROL &= ~(1 << ENABLE);
	TxDone = true;
}

void SPI_SendCmd(uint8_t data){
	DATA_IN = data;
	CONTROL &= ~(1 << DATA);
	CONTROL |= (1 << ENABLE);
	while(!TxDone);
	TxDone = false;
}
void SPI_SendData(uint8_t data){
	DATA_IN = data;
	CONTROL |= (1 << DATA);
	CONTROL |= (1 << ENABLE);
	while(!TxDone);
	TxDone = false;
}
