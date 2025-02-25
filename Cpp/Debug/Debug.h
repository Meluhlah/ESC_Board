#ifndef DEBUG_H
#define DEBUG_H

#pragma once

#include "usart.h"
#include <cstring>
#include "stdio.h"

class Debug
{
public:
    Debug();
    ~Debug();

    // Overload for string literals
    Debug& operator<<(const char* msg)
    {
        HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
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