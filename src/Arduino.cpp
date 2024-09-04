#include <Arduino.h>
#include <MotorDriver.h>

MotorDriver motori;

void setup() {
    delay(1000);
    motori.backLeftSide(70);
    motori.backRightSide(70);
    delay(1500);
    motori.fowardLeftSide(0);
    motori.fowardRightSide(0);
}

void loop() {
  
}
