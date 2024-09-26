#ifndef NAV_H
#define NAV_H

#include <Ultrasonic.h>

enum Direction{
    FOWARD, BACK, LEFT, RIGHT, STOP,
    FOWARDRIGHT, FOWARDLEFT, BACKRIGHT, BACKLEFT
};

typedef struct MESSAGE{
    Direction direction;
    int speed;
    float rolling;
}Message;

class NAV{
private:
    Message input;
    Message output;
    UlSenzor *senz = nullptr;
public:
    NAV(UlSenzor*);
    void emergencyStop();
    void readInputMessage(Message);
    void autonomousControl();
    Message getOutput();
};

#endif