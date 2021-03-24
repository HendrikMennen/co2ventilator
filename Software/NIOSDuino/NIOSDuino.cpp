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

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4


// Hardware SPI connection
//MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary output pins
MD_Parola P = MD_Parola(HARDWARE_TYPE, A2, A0, A1, MAX_DEVICES);

void setup(void)
{
    P.begin();
    Serial0.begin(9600);
}

void loop(void)
{
    Serial0.println("Test");
    
    //if (P.displayAnimate())
    //    P.displayText("Hello", PA_CENTER, P.getSpeed(), P.getPause(), PA_SCROLL_DOWN, PA_SCROLL_UP);
    
    delay(1000);
}