#ifndef CNOTGATE_H
#define CNOTGATE_H

#include "CLogicGates.h"

// NotGate Class
class CNotGate : public CLogicGates {
public:
    CNotGate();
    void DriveInput(int inputIndex, eLogicLevel level) override;
    eLogicLevel GetOutputState() const override;

protected:
    void ComputeOutput() override;
};

#endif 
