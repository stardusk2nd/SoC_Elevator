#include "spi_lcd.h"

void SpiLcdInit(){
    /* 9-2nd bit: set prescalor as 9(10-1)
	   1st bit: set dc as low(command mode)
       0th bit: set cs as high(spi module off) */
	CONTROL = 0b0000100100;

	// soft reset
	SendCommand(0x01);
	// sleep out
    SendCommand(0x11);
    // display on
    SendCommand(0x29);
    // RGB format setting (as RGB: 5/6/5 bit)
    SendCommand(0x3a);
    SendData(0x55);

	unsigned int i = 0;
	// memory write
	SendCommand(0x2c);
	// fill entire display(320x240) with black color(RGB: 0/0/0)
	// black: background / white: letter
	for(i = 0; i < 153600; i++){
		SendData(0x00);
	}
}

/* functions for printing arrow */
void PrintArrow(bool Direction, bool Start){
	// print out only when the elevator is moving
	if(!Start)
		Stop();
	else{
		if(Direction == UP)
			UpArrow();
		else
			DownArrow();
	}
}
void UpArrow(){
	for(uint8_t j=0; j<50; j+=10){
		// column addr. set
		SendCommand(0x2a);
		// start addr.
		SendData(0x00);
		SendData(140 - j);
		// end addr.
		SendData(0x00);
		SendData(149 - j);

		// page addr. set
		SendCommand(0x2b);
		SendData(0x00);
		SendData(40 + j);
		SendData(0x00);
		SendData(129 - j);

		SendCommand(0x2c);
		for(uint16_t i=0; i<900-20*j; i++){
			SendData(0x00);
			SendData(0b00011111);
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
		for(uint16_t i=0; i<900-20*j; i++){
			SendData(0x00);
			SendData(0b00011111);
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

/* functions for printing seven segment */
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
    if(OnOff){
		for(i=0; i<850; i++){
			SendData(0x00);
			SendData(0b00011111);
		}
    }
	else{
		for(i=0; i<1700; i++){
			SendData(0x00);
		}
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
