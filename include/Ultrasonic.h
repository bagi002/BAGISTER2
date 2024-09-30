#ifndef ULTRASONIC_H
#define ULTRASONIC_H

class UlSenzor{
private:
    int trigPin;
    int echoPin;
    float distance;
    float lastDistance[5];
public:
    UlSenzor(int, int);
    void loadDistance();
    float getDistance();
    bool alarmDistance();
};

#endif