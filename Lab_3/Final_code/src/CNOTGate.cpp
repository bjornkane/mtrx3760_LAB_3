#include "CNOTGate.h"

// Constructor to initialize NOT gate with one undefined input
CNotGate::CNotGate() {
    inputs.resize(1, eLogicLevel::LOGIC_UNDEFINED);
}

// Sets the logic level of the input and recomputes the output
void CNotGate::DriveInput(int inputIndex, eLogicLevel level) {
    inputs[inputIndex] = level;
    ComputeOutput();
}

// Returns the current output state of the NOT gate
eLogicLevel CNotGate::GetOutputState() const {
    return outputValue;
}

// Computes the NOT gate's output based on its single input (inverts the input)
void CNotGate::ComputeOutput() {
    outputValue = (inputs[0] == eLogicLevel::LOGIC_HIGH) ? eLogicLevel::LOGIC_LOW : eLogicLevel::LOGIC_HIGH;
}
