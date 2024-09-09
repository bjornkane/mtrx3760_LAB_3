#ifndef AND_GATE_H
#define AND_GATE_H

#include "LogicGates.h"

class CAndGates : public CLogicGates {
public:
    CAndGates();
    void DriveInput(int inputIndex, eLogicLevel level) override;
    eLogicLevel GetOutputState() const override;

protected:
    void ComputeOutput() override;
};

#endif  // AND_GATE_H
