#ifndef COMPARATOR_H
#define COMPARATOR_H

#include "LogicGates.h"

class COneBitComparator : public CLogicGates {
public:
    COneBitComparator();
    void DriveInput(int inputIndex, eLogicLevel level) override;
    eLogicLevel GetOutputState() const override;

    eLogicLevel GetGreaterOutput() const;
    eLogicLevel GetEqualOutput() const;
    eLogicLevel GetLessOutput() const;

protected:
    void ComputeOutput() override;

private:
    eLogicLevel greater = eLogicLevel::LOGIC_UNDEFINED;
    eLogicLevel equal = eLogicLevel::LOGIC_UNDEFINED;
    eLogicLevel less = eLogicLevel::LOGIC_UNDEFINED;
};

#endif  // COMPARATOR_H
