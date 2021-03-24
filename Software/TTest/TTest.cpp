
#include <Arduino.h>

void setup() {
    Serial0.begin(9600); //Set Baudrate with "New Processor"
}

void loop() {
    Serial0.println("Hello World");
    delay(1000);
}
