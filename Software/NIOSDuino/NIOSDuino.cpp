#include <Arduino.h>
#include "lib/matrix/MD_Parola.h"
#include "lib/matrix/MD_MAX72xx.h"
#include <SPI.h>
#include "lib/SCD30/scd30.h"
#include <string.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

MD_Parola P = MD_Parola(HARDWARE_TYPE, A2, A0, A1, MAX_DEVICES);

SCD30 scd30;

void setup(void)
{
    Serial0.begin(9600);
    P.begin();
    P.setIntensity(1);
    scd30.begin();
    
    pinMode(A3, OUTPUT);
}

int timer = 0;

void loop(void)
{
    scd30.read();
    char co2[32];
    
    sprintf(co2, "%d", (int)scd30.co2_value());
    if (P.displayAnimate())
        P.displayText(co2, PA_CENTER, P.getSpeed(), P.getPause(), PA_NO_EFFECT, PA_NO_EFFECT);
    
    if(scd30.co2_value() > 2000)
        digitalWrite(A3, HIGH); //Turn on ventilator
    else
        digitalWrite(A3, LOW);//Turn off ventilator
    
    delay(500);
}