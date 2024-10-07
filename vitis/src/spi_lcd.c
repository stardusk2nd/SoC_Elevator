#include "spi_lcd.h"

void SpiLcdInit(){
    // set prescalor as 9(10-1)
    CONTROL = 0b0000100100;

    SendCommand(0x11);
    SendCommand(0x29);
    SendCommand(0x3a);
    SendData(0x55);

	unsigned int i = 0;
	SendCommand(0x2c);
	for(i = 0; i < 153600; i++){
		SendData(0x00);
	}
}

void PrintArrow(bool Direction, bool Start){
	if(!Start)
		Stop();
	else{
		if(Direction)
			UpArrow();
		else
			DownArrow();
	}
}
void UpArrow(){
	for(uint8_t j=0; j<50; j+=10){
		SendCommand(0x2a);
		SendData(0x00);
		SendData(140 - j);
		SendData(0x00);
		SendData(149 - j);

		SendCommand(0x2b);
		SendData(0x00);
		SendData(40 + j);
		SendData(0x00);
		SendData(129 - j);

		SendCommand(0x2c);
		for(uint16_t i=0; i<1800-40*j; i++){
			SendData(0xff);
		}
	}

}
void DownArrow(){
	for(uint8_t j=0; j<50; j+=10){
		SendCommand(0x2a);
		SendData(0x00);
		SendData(100 + j);
		SendData(0x00);
		SendData(109 + j);

		SendCommand(0x2b);
		SendData(0x00);
		SendData(40 + j);
		SendData(0x00);
		SendData(129 - j);

		SendCommand(0x2c);
		for(uint16_t i=0; i<1800-40*j; i++){
			SendData(0xff);
		}
	}
}
void Stop(){
	for(uint8_t j=0; j<50; j+=10){
		SendCommand(0x2a);
		SendData(0x00);
		SendData(100 + j);
		SendData(0x00);
		SendData(109 + j);

		SendCommand(0x2b);
		SendData(0x00);
		SendData(40);
		SendData(0x00);
		SendData(129);

		SendCommand(0x2c);
		for(uint16_t i=0; i<1800; i++){
			SendData(0x00);
		}
	}
}

void PrintCurFloor(uint8_t CurFloor){
	switch(CurFloor){
		case 1:
			SetA(0);
			SetB(1);
			SetC(1);
			SetD(0);
			SetE(0);
			SetF(0);
			SetG(0);
			break;
		case 2:
			SetA(1);
			SetB(1);
			SetC(0);
			SetD(1);
			SetE(1);
			SetF(0);
			SetG(1);
			break;
		case 3:
			SetA(1);
			SetB(1);
			SetC(1);
			SetD(1);
			SetE(0);
			SetF(0);
			SetG(1);
			break;
	}
}

void Iteration(bool OnOff){
    uint16_t i = 0;
    SendCommand(0x2c);
    for(i=0; i<1700; i++){
    	if(OnOff)
    		SendData(0xff);
    	else
    		SendData(0x00);
    }
}

void SetA(bool OnOff){
    SendCommand(0x2a);
    SendData(0x00);
    SendData(20);
    SendData(0x00);
    SendData(29);

    SendCommand(0x2b);
    SendData(0x00);
    SendData(190);
    SendData(0x01);
    SendData(0x12);

    Iteration(OnOff);
}
void SetB(bool OnOff){
    SendCommand(0x2a);
    SendData(0x00);
    SendData(30);
    SendData(0x00);
    SendData(114);

    SendCommand(0x2b);
    SendData(0x01);
    SendData(0x13);
    SendData(0x01);
    SendData(0x1c);

    Iteration(OnOff);
}
void SetC(bool OnOff){
    SendCommand(0x2a);
    SendData(0x00);
    SendData(125);
    SendData(0x00);
    SendData(209);

    SendCommand(0x2b);
    SendData(0x01);
    SendData(0x13);
    SendData(0x01);
    SendData(0x1c);

    Iteration(OnOff);
}
void SetD(bool OnOff){
    SendCommand(0x2a);
    SendData(0x00);
    SendData(210);
    SendData(0x00);
    SendData(219);

    SendCommand(0x2b);
    SendData(0x00);
    SendData(190);
    SendData(0x01);
    SendData(0x12);

    Iteration(OnOff);
}
void SetE(bool OnOff){
    SendCommand(0x2a);
    SendData(0x00);
    SendData(125);
    SendData(0x00);
    SendData(209);

    SendCommand(0x2b);
    SendData(0x00);
    SendData(180);
    SendData(0x00);
    SendData(189);

    Iteration(OnOff);
}
void SetF(bool OnOff){
    SendCommand(0x2a);
    SendData(0x00);
    SendData(30);
    SendData(0x00);
    SendData(114);

    SendCommand(0x2b);
    SendData(0x00);
    SendData(180);
    SendData(0x00);
    SendData(189);

    Iteration(OnOff);
}
void SetG(bool OnOff){
    SendCommand(0x2a);
    SendData(0x00);
    SendData(115);
    SendData(0x00);
    SendData(124);

    SendCommand(0x2b);
    SendData(0x00);
    SendData(190);
    SendData(0x01);
    SendData(0x12);

    Iteration(OnOff);
}
