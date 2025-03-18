#ifndef DEBUG_H
#define DEBUG_H

#pragma once

#include "usart.h"
#include <cstring>
#include "stdio.h"

// Choose uart operation mode
// #define BLOCKING_MODE
#define NON_BLOCKING_MODE

class Debug
{
private:
    UART_HandleTypeDef* uartHandler;
    uint8_t txFlag;

public:
    Debug(UART_HandleTypeDef* uartHandler);
    ~Debug();

    void setTxFlag();

    // Overload for string literals
    Debug& operator<<(const char* msg)
    {

        #ifdef BLOCKING_MODE
        HAL_UART_Transmit(&UART_HANDLER, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
        #elif defined(NON_BLOCKING_MODE)
        txFlag = 0;
        HAL_UART_Transmit_DMA(uartHandler, (uint8_t*)msg, strlen(msg));
        #else
            #error "Please define either BLOCKING_MODE or NON_BLOCKING_MODE"
        #endif

        return *this;
    }

    // Overload for integer values
    Debug& operator<<(int num)
    {
        char buffer[12];  // Buffer for integer conversion
        snprintf(buffer, sizeof(buffer), "%d", num);
        return *this << buffer;  // Call the string overload
    }

    Debug& operator<<(uint32_t num)
    {
        char buffer[32];  // Buffer for integer conversion
        snprintf(buffer, sizeof(buffer), "%lu", num);
        return *this << buffer;  // Call the string overload
    }

    // Overload for float values
    Debug& operator<<(float num)
    {
        char buffer[20];  // Buffer for float conversion
        snprintf(buffer, sizeof(buffer), "%.2f", num); // Print with 2 decimal places
        return *this << buffer;
    }
};

#endif