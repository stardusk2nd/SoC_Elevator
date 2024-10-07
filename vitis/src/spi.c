#include "spi.h"

bool TxDone = false;

void SendCommand(uint8_t data){
	DATA_IN = data;
	CONTROL &= ~(1 << DATA);
	CONTROL |= (1 << ENABLE);
	while(!TxDone);
	TxDone = false;
}

void SendData(uint8_t data){
	DATA_IN = data;
	CONTROL |= (1 << DATA);
	CONTROL |= (1 << ENABLE);
	while(!TxDone);
	TxDone = false;
}
