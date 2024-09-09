#ifndef CORGATE_H
#define CORGATE_H

#include "CLogicGates.h"

// OrGate Class
class COrGates : public CLogicGates {
public:
    COrGates();
    void DriveInput(int inputIndex, eLogicLevel level) override;
    eLogicLevel GetOutputState() const override;

protected:
    void ComputeOutput() override;
};

#endif 
