#include <Arduino.h>
#include <MotorDriver.h>

MotorDriver motori;

void setup() {
    Serial.begin(115200);

    Serial.println("Krece");
}

void loop() {
    String data = "";
    Command command;

    int speed;
    bool controling = false;
    float rolling;

    if(Serial.available() > 0){
        data = Serial.readStringUntil('\n');
    }

    if(data.length() > 0){
        String tmp = "";
        int dat = 0;

        for(int i = 0; i < data.length(); i++){
            if(data[i] == ';'){
                if(dat == 0){
                    command = (Command)tmp.toInt();
                }
                if(dat == 1){
                    rolling = (float)tmp.toFloat();
                }
                dat++;
                i++;
                tmp = "";
            }
                tmp += data[i];
        }

        speed = tmp.toInt();
        controling = true;

        Serial.println("primljeno");
    }

    if(controling){
        controling = false;
        motori.controlCar(command, speed, rolling);
    }

    delay(20);
}
