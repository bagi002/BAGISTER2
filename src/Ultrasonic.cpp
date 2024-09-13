#include <Arduino.h>
#include <Ultrasonic.h>

UlSenzor::UlSenzor(int trig, int echo){
    echoPin = echo;
    trigPin = trig;

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    distance = 0.0;
}

void UlSenzor::loadDistance(){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 3500);

    distance = duration * 0.0343 / 2;
}

float UlSenzor::getDistance(){
    return distance;
}

bool UlSenzor::alarmDistance(){
    if(distance < 15) return true;
    return false;
}