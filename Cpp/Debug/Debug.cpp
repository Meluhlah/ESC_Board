#include "Debug.h"

Debug::Debug(UART_HandleTypeDef *uartHandler)
:   uartHandler(uartHandler), txFlag(0)
{
}

Debug::~Debug()
{

}

void Debug::setTxFlag()
{
    txFlag = 1;
}
