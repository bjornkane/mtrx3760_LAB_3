#include "COrGate.h"

// Constructor to initialize OR gate with two undefined inputs
COrGates::COrGates() {
    inputs.resize(2, eLogicLevel::LOGIC_UNDEFINED);
}

// Sets the logic level of a specific input and recomputes the output
void COrGates::DriveInput(int inputIndex, eLogicLevel level) {
    inputs[inputIndex] = level;
    ComputeOutput();
}

// Returns the current output state of the OR gate
eLogicLevel COrGates::GetOutputState() const {
    return outputValue;
}

// Computes the OR gate's output based on its two inputs
void COrGates::ComputeOutput() {
    outputValue = (inputs[0] == eLogicLevel::LOGIC_HIGH || inputs[1] == eLogicLevel::LOGIC_HIGH) 
                    ? eLogicLevel::LOGIC_HIGH: eLogicLevel::LOGIC_LOW;
}
