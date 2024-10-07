#include "top.h"
#include "spi.h"

//#define DATASTEPS 1700;

void SpiLcdInit();

void PrintArrow(bool Direction, bool Start);
void UpArrow();
void DownArrow();
void Stop();

void PrintCurFloor(uint8_t CurFloor);
void Iteration(bool OnOff);
void SetA(bool OnOff);
void SetB(bool OnOff);
void SetC(bool OnOff);
void SetD(bool OnOff);
void SetE(bool OnOff);
void SetF(bool OnOff);
void SetG(bool OnOff);
