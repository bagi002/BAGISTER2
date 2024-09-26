#include <NAV.h>
#include <Arduino.h>

NAV::NAV(UlSenzor* senzor){
    senz = senzor;

    output.direction = STOP;
    output.rolling = 0.0;
    output.speed = 0;

    input.direction = STOP;
    input.rolling = 0.0;
    input.speed = 0;
}

void NAV::emergencyStop(){
    if(senz->alarmDistance() && (input.direction == FOWARD || input.direction == FOWARDLEFT || input.direction == FOWARDRIGHT)){
        output.direction = STOP;
        output.speed = 0.0;
    }
}

void NAV::autonomousControl(){
    output = input;
    emergencyStop();
}

void NAV::readInputMessage(Message in){
    input = in;
}

Message NAV::getOutput(){
    return output;
}