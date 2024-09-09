#ifndef OR_GATE_H
#define OR_GATE_H

#include "LogicGates.h"

class COrGates : public CLogicGates {
public:
    COrGates();
    void DriveInput(int inputIndex, eLogicLevel level) override;
    eLogicLevel GetOutputState() const override;

protected:
    void ComputeOutput() override;
};

#endif  // OR_GATE_H
