#include <Arduino.h>
#include <Ultrasonic.h>

UlSenzor::UlSenzor(int trig, int echo){
    echoPin = echo;
    trigPin = trig;

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    distance = 0.0;
    for(int i = 0; i < 5; i++)lastDistance[i] = 0.0;
}

void UlSenzor::loadDistance(){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH,5500);
    float rawdistance = duration * 0.0343 / 2;
    float sum = 0;

    for(int i = 1; i < 5; i++){
        lastDistance[i] = lastDistance[i-1];
        sum += lastDistance[i];
    }
        lastDistance[0] = rawdistance;
        distance = (sum + rawdistance) / 5;

    //Serial.print("Distance: ");
    //Serial.print(distance);
    //Serial.println(" cm");
    if(distance == 0)distance = 60.0;
}

float UlSenzor::getDistance(){
    return distance;
}

bool UlSenzor::alarmDistance(){
    if(distance < 10) return true;
    return false;
}