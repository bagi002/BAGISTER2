#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

enum MotorStatus{
    STOP, FOWARD, BACK
};

enum Command{
    FOWARDC, BACKC, ROTATELEFTC, ROTATERIGHTC, STOPC,
    FOWARDRIGHTC, FOWARDLEFTC, BACKRIGHTC, BACKLEFTC
};

class MotorDriver{
private:
// Front left motor, foward, back, enabled
    int FLMF;
    int FLMB;
    int FLME;
    MotorStatus FLState;
// Front right
    int FRMF;
    int FRMB;
    int FRME;
    MotorStatus FRState;
// Back left
    int BLMF;
    int BLMB;
    int BLME;
    MotorStatus BLState;
// Back Right
    int BRMF;
    int BRMB;
    int BRME;
    MotorStatus BRState;
public:
    MotorDriver();
    //Set osnovnih upravljackih komandi
    void fowardLeftSide(int);
    void fowardRightSide(int);
    void backLeftSide(int);
    void backRightSide(int);
    void stopLeftSide();
    void stopRightSide();
    void changeSpeedLeft(int, bool);
    void changeSpeedRight(int, bool);
    //Set naprednijih upravljackih komandi
    void fowardCar(int);
    void backCar(int);
    void rotateInPlace(int, bool);
    void stopCar();
    void rollCarFoward(int, float, bool); 
    void rollCarBack(int, float, bool);
    //upravljacki set
    void controlCar(Command, int, float);
};

#endif