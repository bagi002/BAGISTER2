#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

enum MotorStatus{
    STOP, FOWARD, BACK
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
    void fowardLeftSide(int);
    void fowardRightSide(int);
    void backLeftSide(int);
    void backRightSide(int);
    void stopLeftSide();
    void stopRightSide();
};

#endif