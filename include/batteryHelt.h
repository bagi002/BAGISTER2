#ifndef BATTERYHELT_H
#define BATTERYHELT_H

class Battery{
private:
    int VoltPin;
    int dataVoltage;
    int dataV[4];
public:
    Battery();
    void readVoltage();
    int getVoltageSource();
};

#endif