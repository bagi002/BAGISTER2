#include <MotorDriver.h>
#include <Arduino.h>

MotorDriver::MotorDriver(){
    BRMF = 2;
    BRMB = 3;
    BRME = 5;
    BLME = 6;
    BLMF = 7;
    BLMB = 4;
    FRMF = 8;
    FRMB = 9;
    FRME = 10;
    FLMF = 12;
    FLMB = 13;
    FLME = 11;
    pinMode(BLMF, OUTPUT);
    pinMode(BLMB, OUTPUT);
    pinMode(BLME, OUTPUT);
    pinMode(BRMF, OUTPUT);
    pinMode(BRMB, OUTPUT);
    pinMode(BRME, OUTPUT);
    pinMode(FRMF, OUTPUT);
    pinMode(FRMB, OUTPUT);
    pinMode(FRME, OUTPUT);
    pinMode(FLMF, OUTPUT);
    pinMode(FLMB, OUTPUT);
    pinMode(FLME, OUTPUT);
    FLState = STOP;
    FRState = STOP;
    BLState = STOP;
    BRState = STOP;
}

// pri kretanju naprjed najmanja dozvoljena brzina je 80, ali se preporucuje kretanje brzinama od bar 100

void MotorDriver::fowardRightSide(int speed){
    digitalWrite(FRMF, HIGH);
    digitalWrite(FRMB, LOW);
    digitalWrite(BRMF, HIGH);
    digitalWrite(BRMB, LOW);

    analogWrite(FRME, 255);
    delay(3);
    analogWrite(BRME, 255);
    delay(2);
    analogWrite(FRME, speed);
    delay(3);
    analogWrite(BRME, speed);

    FRState = FOWARD;
    BRState = FOWARD;
}

void MotorDriver::fowardLeftSide(int speed){
    digitalWrite(FLMF, HIGH);
    digitalWrite(FLMB, LOW);
    digitalWrite(BLMF, HIGH);
    digitalWrite(BLMB, LOW);

    analogWrite(FLME, 255);
    delay(3);
    analogWrite(BLME, 255);
    delay(2);
    analogWrite(FLME, speed);
    delay(3);
    analogWrite(BLME, speed);

    FLState = FOWARD;
    BLState = FOWARD;
}

// pri kretanju nazad najmanja dozvoljena brzina je 70, preporucuje se rad sa brzinama vecim od 90.
void MotorDriver::backRightSide(int speed){
    digitalWrite(FRMF, LOW);
    digitalWrite(FRMB, HIGH);
    digitalWrite(BRMF, LOW);
    digitalWrite(BRMB, HIGH);

    analogWrite(FRME, 255);
    delay(3);
    analogWrite(BRME, 255);
    delay(2);
    analogWrite(FRME, speed);
    delay(3);
    analogWrite(BRME, speed);

    FRState = BACK;
    BRState = BACK;
}

void MotorDriver::backLeftSide(int speed){
    digitalWrite(FLMF, LOW);
    digitalWrite(FLMB, HIGH);
    digitalWrite(BLMF, LOW);
    digitalWrite(BLMB, HIGH);

    analogWrite(FLME, 255);
    delay(3);
    analogWrite(BLME, 255);
    delay(2);
    analogWrite(FLME, speed);
    delay(3);
    analogWrite(BLME, speed);

    FLState = BACK;
    BLState = BACK;
}

void MotorDriver::stopLeftSide(){
    digitalWrite(FLMF, LOW);
    digitalWrite(FLMB, LOW);
    analogWrite(FLME, 0);

    digitalWrite(BLMF, LOW);
    digitalWrite(BLMB, LOW);
    analogWrite(BLME, 0);

    FLState = STOP;
    BLState = STOP;
}

void MotorDriver::stopRightSide(){
    digitalWrite(FRMF, LOW);
    digitalWrite(FRMB, LOW);
    analogWrite(FRME, 0);

    digitalWrite(BRMF, LOW);
    digitalWrite(BRMB, LOW);
    analogWrite(BRME, 0);

    FLState = STOP;
    BLState = STOP;
}

