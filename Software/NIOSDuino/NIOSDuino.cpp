// Program to demonstrate the MD_Parola library
//
// Simplest program that does something useful - Hello World!
//
// MD_MAX72XX library can be found at https://github.com/MajicDesigns/MD_MAX72XX
//

#include <Arduino.h>
#include "lib/matrix/MD_Parola.h"
#include "lib/matrix/MD_MAX72xx.h"
#include <SPI.h>
#include "lib/SCD30/scd30.h"
#include <string.h>

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4


// Hardware SPI connection
//MD_Parola P = MD_Parola(HARDWARE_TYPE, A2, MAX_DEVICES);
// Arbitrary output pins
MD_Parola P = MD_Parola(HARDWARE_TYPE, A2, A0, A1, MAX_DEVICES);
SCD30 scd30;

void setup(void)
{
    Serial0.begin(9600);
    P.begin();   
    scd30.begin();
}

void loop(void)
{
    scd30.read();
    char co2[32];
    
    sprintf(co2, "%d", (int)scd30.co2_value());
    
    if (P.displayAnimate())
        P.displayText(co2, PA_CENTER, P.getSpeed(), P.getPause(), PA_NO_EFFECT, PA_NO_EFFECT);
    
    delay(100);
}