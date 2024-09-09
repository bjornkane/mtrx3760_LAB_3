#ifndef NOT_GATE_H
#define NOT_GATE_H

#include "LogicGates.h"

class CNotGate : public CLogicGates {
public:
    CNotGate();
    void DriveInput(int inputIndex, eLogicLevel level) override;
    eLogicLevel GetOutputState() const override;

protected:
    void ComputeOutput() override;
};

#endif  // NOT_GATE_H
