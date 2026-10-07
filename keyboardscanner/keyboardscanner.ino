/*
Kurzweil PC2 76-Key MIDI Controller

PC2-specific adaptation of Daniel Moura's keyboardscanner project.
Original scanner copyright (C) 2017 Daniel Moura <oxesoft@gmail.com>
GPLv3 or later.
*/

#include "globals.h"

#define EXTENSION(name) void name##Setup(); void name##Loop();
#include "extensions.h"
#undef EXTENSION

void setup()
{
    // Successful prototype path:
    // Mega UART0/USB serial COM port -> Bome serial MIDI at 31250 baud, 8N1.
    Serial.begin(31250);

    scannerSetup();

    #define EXTENSION(name) name##Setup();
    #include "extensions.h"
    #undef EXTENSION
}

void loop()
{
    scannerLoop();
    statesLoop();

    #define EXTENSION(name) name##Loop();
    #include "extensions.h"
    #undef EXTENSION
}
