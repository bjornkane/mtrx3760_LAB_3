#include "CAndGate.h"

// Constructor to initialize AND gate with two undefined inputs
CAndGates::CAndGates() {
    inputs.resize(2, eLogicLevel::LOGIC_UNDEFINED);
}

// Sets the logic level of a specific input and recomputes the output
void CAndGates::DriveInput(int inputIndex, eLogicLevel level) {
    inputs[inputIndex] = level;
    ComputeOutput();
}

// Returns the current output state of the AND gate
eLogicLevel CAndGates::GetOutputState() const {
    return outputValue;
}

// Computes the AND gate's output based on its two inputs
void CAndGates::ComputeOutput() {
    outputValue = (inputs[0] == eLogicLevel::LOGIC_HIGH && inputs[1] == eLogicLevel::LOGIC_HIGH) 
                    ? eLogicLevel::LOGIC_HIGH : eLogicLevel::LOGIC_LOW;
}
