#include <Arduino.h>

void setup(){
    Serial2.begin(115200, SERIAL_8N1, 16, 17); // RX2 na GPIO16 i TX2 na GPIO17
    Serial2.print("0;255\n");
}

void loop(){
    Serial2.print("0;0\n");
    delay(1000);
}