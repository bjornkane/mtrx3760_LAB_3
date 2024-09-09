#ifndef XOR_GATE_H
#define XOR_GATE_H

#include "LogicGates.h"

class CXORGates : public CLogicGates {
public:
    CXORGates();
    void DriveInput(int inputIndex, eLogicLevel level) override;
    eLogicLevel GetOutputState() const override;

protected:
    void ComputeOutput() override;
};

#endif  // XOR_GATE_H
