#include "AndGate.h"

CAndGates::CAndGates() {
    inputs.resize(2, eLogicLevel::LOGIC_UNDEFINED);
}

void CAndGates::DriveInput(int inputIndex, eLogicLevel level) {
    inputs[inputIndex] = level;
    ComputeOutput();
}

eLogicLevel CAndGates::GetOutputState() const {
    return outputValue;
}

void CAndGates::ComputeOutput() {
    outputValue = (inputs[0] == eLogicLevel::LOGIC_HIGH && inputs[1] == eLogicLevel::LOGIC_HIGH) ? eLogicLevel::LOGIC_HIGH : eLogicLevel::LOGIC_LOW;
}
