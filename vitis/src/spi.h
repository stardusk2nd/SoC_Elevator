#include "top.h"

/* SPI register & bit defines */
#define CONTROL 	spi_register[0]
#define DATA_IN		spi_register[1]
#define ENABLE		0
#define DATA		1

void SendCommand(uint8_t data);
void SendData(uint8_t data);

extern volatile uint32_t *spi_register;
