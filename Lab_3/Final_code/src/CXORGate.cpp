#include "CXORGate.h"

// Constructor to initialize XOR gate with two undefined inputs
CXORGates::CXORGates() {
    inputs.resize(2, eLogicLevel::LOGIC_UNDEFINED);
}

// Sets the logic level of a specific input and recomputes the output
void CXORGates::DriveInput(int inputIndex, eLogicLevel level) {
    inputs[inputIndex] = level;
    ComputeOutput();
}

// Returns the current output state of the XOR gate
eLogicLevel CXORGates::GetOutputState() const {
    return outputValue;
}

// Computes the XOR gate's output based on its two inputs
void CXORGates::ComputeOutput() {
    outputValue = (inputs[0] != inputs[1]) ? eLogicLevel::LOGIC_HIGH : eLogicLevel::LOGIC_LOW;
}
