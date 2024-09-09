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

void MotorDriver::changeSpeedLeft(int speed,bool lowSpeed = false){
    if(!lowSpeed){
        analogWrite(FLME, speed);
        analogWrite(BLME, speed);
    }else{

    }
}

void MotorDriver::changeSpeedRight(int speed, bool lowSpeed = false){
    if(!lowSpeed){
        analogWrite(FRME, speed);
        analogWrite(BRME, speed);
    }else{

    }
}

// naprednije komande za laksu kontrolu

void MotorDriver::fowardCar(int speed){
    if(speed < 80) speed = 80;

    if(BLState != FOWARD && FLState != FOWARD && BRState != FOWARD && FRState != FOWARD){
        fowardRightSide(100);
        delay(10);
        fowardLeftSide(200);
        delay(9);
        fowardLeftSide(speed);
        fowardRightSide(speed);
    }else{
        if(BLState == FOWARD && FLState == FOWARD){
            changeSpeedLeft(speed);
        }else{
            stopLeftSide();
            delay(2);
            fowardLeftSide(speed);
        }

        if(BRState == FOWARD && FRState == FOWARD){
            changeSpeedRight(speed);
        }else{
            stopRightSide();
            delay(2);
            fowardRightSide(speed);
        }
    }
}

void MotorDriver::backCar(int speed){
    if(speed < 70) speed = 70;

    if(BLState != BACK && FLState != BACK && BRState != BACK && FRState != BACK){
        backRightSide(100);
        delay(10);
        backLeftSide(200);
        delay(9);
        backLeftSide(speed);
        backRightSide(speed);
    }else{
        if(BLState == BACK && FLState == BACK){
            changeSpeedLeft(speed);
        }else{
            stopLeftSide();
            delay(2);
            backLeftSide(speed);
        }

        if(BRState == BACK && FRState == BACK){
            changeSpeedRight(speed);
        }else{
            stopRightSide();
            delay(2);
            backRightSide(speed);
        }
    }
}

void MotorDriver::rotateInPlace(int speed, bool right){
    if(speed < 140) speed = 160;
    stopLeftSide();
    stopRightSide();
    delay(3);
    if(right){
        fowardLeftSide(speed+30);
        delay(8);
        backRightSide(speed);
    }else{
        backLeftSide(speed+30);
        delay(8);
        fowardRightSide(speed);
    }
}

void MotorDriver::stopCar(){
    stopLeftSide();
    stopRightSide();
}

void MotorDriver::rollCarFoward(int speed, float rolling, bool right){
    if(BLState != FOWARD || FLState != FOWARD || BRState != FOWARD || FRState != FOWARD){
        fowardCar(200);
        delay(15);
    }

    if(speed < 120) speed = 140;

    rolling = 1.0 - rolling;

    if(right){
        fowardLeftSide(speed);
        fowardRightSide((80 + (speed - 80) * rolling));
    }else{
        fowardRightSide(speed);
        fowardLeftSide((80 + (speed - 80) * rolling));
    }
}

void MotorDriver::rollCarBack(int speed, float rolling, bool right){
    if(BLState != BACK || FLState != BACK || BRState != BACK || FRState != BACK){
        backCar(200);
        delay(15);
    }

    if(speed < 120) speed = 140;

    rolling = 1.0 - rolling;

    if(right){
        backLeftSide(speed);
        backRightSide((80 + (speed - 80) * rolling));
    }else{
        backRightSide(speed);
        backLeftSide((80 + (speed - 80) * rolling));
    }
}

void MotorDriver::controlCar(Command command, int speed, float rolling = 1){
    switch(command){
        case STOPC:
            stopCar();
            break;
        case FOWARDC:
            fowardCar(speed);
            break;
        case BACKC:
            backCar(speed);
            break;
        case ROTATERIGHTC:
            rotateInPlace(speed, true);
            break;
        case ROTATELEFTC:
            rotateInPlace(speed, false);
            break;
        case FOWARDRIGHTC:
            rollCarFoward(speed, rolling, true);
            break;
        case FOWARDLEFTC:
            rollCarFoward(speed, rolling, false);
            break;
        case BACKRIGHTC:
            rollCarBack(speed, rolling, true);
            break;
        case BACKLEFTC:
            rollCarBack(speed, rolling, false);
            break;
    }
}