#include "Arduino.h"
#include "serial.h"
#include <stdarg.h>

#define LEN_BUF 128

void printfSerial(const char *fmt, ... )
{
    char buf[LEN_BUF];
    va_list args;
    va_start (args, fmt );
    vsnprintf(buf, LEN_BUF, fmt, args);
    va_end (args);
    Serial.print(buf);
}
