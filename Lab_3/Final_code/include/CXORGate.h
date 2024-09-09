#ifndef CXORGATE_H
#define CXORGATE_H

#include "CLogicGates.h"

// XORGate Class
class CXORGates : public CLogicGates {
public:
    CXORGates();
    void DriveInput(int inputIndex, eLogicLevel level) override;
    eLogicLevel GetOutputState() const override;

protected:
    void ComputeOutput() override;
};

#endif 
