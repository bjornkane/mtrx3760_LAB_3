#ifndef CANDGATE_H
#define CANDGATE_H

#include "CLogicGates.h"

// CAndGate Class
class CAndGates : public CLogicGates {
public:
    CAndGates();
    void DriveInput(int inputIndex, eLogicLevel level) override;
    eLogicLevel GetOutputState() const override;

protected:
    void ComputeOutput() override;
};

#endif
