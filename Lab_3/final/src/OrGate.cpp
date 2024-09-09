#include "OrGate.h"

COrGates::COrGates() {
    inputs.resize(2, eLogicLevel::LOGIC_UNDEFINED);
}

void COrGates::DriveInput(int inputIndex, eLogicLevel level) {
    inputs[inputIndex] = level;
    ComputeOutput();
}

eLogicLevel COrGates::GetOutputState() const {
    return outputValue;
}

void COrGates::ComputeOutput() {
    outputValue = (inputs[0] == eLogicLevel::LOGIC_HIGH || inputs[1] == eLogicLevel::LOGIC_HIGH) ? eLogicLevel::LOGIC_HIGH : eLogicLevel::LOGIC_LOW;
}
