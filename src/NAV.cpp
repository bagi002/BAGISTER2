#include <NAV.h>
#include <Arduino.h>
#include <Ultrasonic.h>

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
    if(senz->alarmDistance()){
        output.direction = BACK;
        output.rolling = 0;
        if(output.speed < 100)output.speed = 150;
    }
}

void NAV::autonomousControl(){
    output = input;
    autoRegulationSpeed();
    emergencyStop();
}

void NAV::readInputMessage(Message in){
    input = in;
}

Message NAV::getOutput(){
    return output;
}

void NAV::selfDrivingI(){

}

void NAV::autoRegulationSpeed(){
    float frontDistance = senz->getDistance();

    if(output.speed > autoClacluateSpeed(frontDistance))output.speed = autoClacluateSpeed(frontDistance);

    if(frontDistance < 15 && (input.direction == FOWARD || input.direction == FOWARDLEFT || input.direction == FOWARDRIGHT)){
        output.speed = 0.0;
        output.direction = STOP;
    }
}

int NAV::autoClacluateSpeed(float dist){
    int x = 0;
    if(dist < 50 && dist > 15){
        x = (dist - 15.0) * (155 / 35) + 100;
    }else if(dist < 15){
        x = 100;
    }else{
        x = 255;
    }
    return x;
}