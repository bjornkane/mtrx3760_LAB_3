#ifndef CLOGICGATES_H
#define CLOGICGATES_H

#include <vector>

enum class eLogicLevel { LOGIC_UNDEFINED = -1, LOGIC_LOW = 0, LOGIC_HIGH = 1 };

// Parent Class for all logic gates
class CLogicGates {
public:
    virtual ~CLogicGates() = default;
    virtual void DriveInput(int inputIndex, eLogicLevel level) = 0;
    virtual eLogicLevel GetOutputState() const = 0;

protected:
    virtual void ComputeOutput() = 0;
    std::vector<eLogicLevel> inputs;
    eLogicLevel outputValue = eLogicLevel::LOGIC_UNDEFINED;
};

#endif 
