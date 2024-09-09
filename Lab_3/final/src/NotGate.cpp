#include "NotGate.h"

CNotGate::CNotGate() {
    inputs.resize(1, eLogicLevel::LOGIC_UNDEFINED);
}

void CNotGate::DriveInput(int inputIndex, eLogicLevel level) {
    inputs[inputIndex] = level;
    ComputeOutput();
}

eLogicLevel CNotGate::GetOutputState() const {
    return outputValue;
}

void CNotGate::ComputeOutput() {
    outputValue = (inputs[0] == eLogicLevel::LOGIC_HIGH) ? eLogicLevel::LOGIC_LOW : eLogicLevel::LOGIC_HIGH;
}
