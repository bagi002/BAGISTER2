#include <Arduino.h>
#include <MotorDriver.h>

MotorDriver motori;

void setup() {
    Serial.begin(115200);
}

void loop() {
    String data = "";
    Command command;

    int speed;
    bool controling = false;

    if(Serial.available() > 0){
        data = Serial.readStringUntil('\n');
    }

    if(data.length() > 0){
        String tmp = "";

        for(int i = 0; i < data.length(); i++){
            if(data[i] == ';'){
                command = (Command)tmp.toInt();
                i++;
                tmp = "";
            }
                tmp += data[i];
        }

        speed = tmp.toInt();
        controling = true;


    }

    if(controling){
        controling = false;
        motori.controlCar(command, speed);
    }

    delay(20);
}
