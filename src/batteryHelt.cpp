#include <batteryHelt.h>
#include <Arduino.h>

Battery::Battery(){
    VoltPin = 34;
    pinMode(34, INPUT);
    dataVoltage = 0;
    dataVoltage = analogRead(VoltPin);
    dataV[0] = dataVoltage;
    dataV[1] = dataVoltage;
    dataV[2] = dataVoltage;
    dataV[3] = dataVoltage;
}

void Battery::readVoltage(){
    dataV[3] = dataV[2];
    dataV[2] = dataV[1];
    dataV[1] = dataV[0];
    dataV[0] = analogRead(VoltPin);
    dataVoltage = (dataV[3] + dataV[2] + dataV[1] + dataV[0])/4;
}

int Battery::getVoltageSource(){
    return dataVoltage;
}