#include <Arduino.h>
#include "lib/matrix/MD_MAX72xx.h"
#include <SPI.h>
#include "lib/SCD30/scd30.h"
#include <string.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CHAR_SPACING  1 // pixels between characters

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, 16, 14, 15, MAX_DEVICES);

SCD30 scd30;
bool running = true;
int timer = 0;
int mode = 0;
const int ventSwitch = 22;
const bool miniBar = false;
const bool autoMode = false;

void printText(uint8_t modStart, uint8_t modEnd, char *pMsg)
{
    uint8_t   state = 0;
    uint8_t   curLen;
    uint16_t  showLen;
    uint8_t   cBuf[8];
    int16_t   col = ((modEnd + 1) * COL_SIZE) - 1;

    mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

    do     // finite state machine to print the characters in the space available
    {
        switch(state)
        {
            case 0: // Load the next character from the font table
                // if we reached end of message, reset the message pointer
                if (*pMsg == '\0')
                {
                    showLen = col - (modEnd * COL_SIZE);  // padding characters
                    state = 2;
                    break;
                }

                // retrieve the next character form the font file
                showLen = mx.getChar(*pMsg++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);
                curLen = 0;
                state++;
                // !! deliberately fall through to next state to start displaying

            case 1: // display the next part of the character
                mx.setColumn(col--, cBuf[curLen++]);

                // done with font character, now display the space between chars
                if (curLen == showLen)
                {
                    showLen = CHAR_SPACING;
                    state = 2;
                }
                break;

            case 2: // initialize state for displaying empty columns
                curLen = 0;
                state++;
                // fall through

            case 3:    // display inter-character spacing or end of message padding (blank columns)
                mx.setColumn(col--, 0);
                curLen++;
                if (curLen == showLen)
                    state = 0;
                break;

            default:
                col = -1;   // this definitely ends the do loop
        }
    } while (col >= (modStart * COL_SIZE));

    mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}

void printScale(uint8_t modStart, uint8_t modEnd, int value, int column)
{
    mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
    
    for (int i = 0; i < 8; i++) {
        if(i < value) mx.setPoint(i, column, true);
        else mx.setPoint(i, column, false);
    }
    
    mx.control(modStart, modEnd, MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}

void readState()
{
    //State
    int buttonState = digitalRead(32);
    
    if(buttonState == HIGH)
    {
        running = true;
    }
    else
    {
        running = false;
    }
    
    //Mode
    if(!autoMode)
    {        
        int mode1 = digitalRead(33);
        int mode2 = digitalRead(34);
        
        if(mode1 == LOW && mode2 == LOW) //Mode 0
        {
            mode = 0;
        }
        else if(mode1 == HIGH && mode2 == LOW) //Mode 1
        {
            mode = 1;
        }
        else if(mode1 == HIGH && mode2 == HIGH) //Mode 2
        {
            mode = 2;
        }
    }
    else
    {
        timer++;
        if(timer > 10)
        {
            mode++;
            if(mode > 2) mode = 0;
            timer = 0;
        }
    }
    
}

void setup(void)
{
    Serial0.begin(9600);
    mx.begin();
    mx.control(MD_MAX72XX::INTENSITY, 1);
    scd30.begin();
    
    pinMode(A3, OUTPUT);
}

void loop(void)
{
    readState();
    
    if(running)
    {
        scd30.read();
        
        char message[32];
        float correctTemp = roundf(scd30.temp_value() * 10) / 10.0 - 9; //Rouned to 1 fractional and reduces by 12 device heat
        
        if(mode == 0)
        {
            sprintf(message, "%.1fC", correctTemp);
        }
        else if(mode == 1)
        {
            sprintf(message, "%.fP", scd30.co2_value());
        }
        else if(mode == 2)
        {
            sprintf(message, "%.1f%%", scd30.hum_value());
        }
        
        printText(0, MAX_DEVICES-1, message);
        
        if(miniBar)
        {
            int co2Scale = scd30.co2_value() / 2000 * 8;
            int humScale = scd30.hum_value() / 100 * 8;
            int tempScale = scd30.temp_value() / ventSwitch * 8;
            
            printScale(0, MAX_DEVICES-1, co2Scale, 0);
            printScale(0, MAX_DEVICES-1, humScale, 1);
            printScale(0, MAX_DEVICES-1, tempScale, 2);
        }
        
        if(correctTemp > ventSwitch)
            digitalWrite(A3, HIGH); //Turn on ventilator
        else
            digitalWrite(A3, LOW);//Turn off ventilator
        
        Serial0.println(message);
    }
    else
    {
        digitalWrite(A3, LOW); //Turn off ventilator
        mx.clear();
    }
    
    delay(100);
}

