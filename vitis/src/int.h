#include "top.h"
#include "motor.h"
#include "spi_lcd.h"

/* interrupt ID */
#define INTC_ID 		XPAR_INTC_0_DEVICE_ID

#define SPI_VEC_ID		XPAR_INTC_0_SPI_TX_0_VEC_ID
#define TIMER_VEC_ID	XPAR_INTC_0_TIMER_COUNTER_1_VEC_ID
#define BTN_VEC_ID		XPAR_INTC_0_GPIO_0_VEC_ID
#define PHOTO_VEC_ID	XPAR_INTC_0_GPIO_1_VEC_ID

void TIMER_ISR(void *CallBackRef);
void ISR_SPI(void *CallBackRef);
void BTN_ISR(void *CallBackRef);
void PHOTO_ISR(void *CallBackRef);
void IntInit();

extern XGpio gpio_instance1;	// btn
extern XGpio gpio_instance2;	// photo

extern uint8_t CurrentFloor;
extern uint8_t TargetFloor;
extern bool start;
extern bool TxDone;
extern bool ArrowFlag;
extern bool PrintFlag;
extern bool PwmFlag;
