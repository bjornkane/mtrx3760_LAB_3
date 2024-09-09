#include "XORGate.h"

CXORGates::CXORGates() {
    inputs.resize(2, eLogicLevel::LOGIC_UNDEFINED);
}

void CXORGates::DriveInput(int inputIndex, eLogicLevel level) {
    inputs[inputIndex] = level;
    ComputeOutput();
}

eLogicLevel CXORGates::GetOutputState() const {
    return outputValue;
}

void CXORGates::ComputeOutput() {
    outputValue = (inputs[0] != inputs[1]) ? eLogicLevel::LOGIC_HIGH : eLogicLevel::LOGIC_LOW;
}
