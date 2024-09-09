#include "COneBitComparator.h"

// Constructor to initialize the comparator with two undefined inputs
COneBitComparator::COneBitComparator() {
    inputs.resize(2, eLogicLevel::LOGIC_UNDEFINED);
}

// Sets the logic level of for a input index and recomputes the output
void COneBitComparator::DriveInput(int inputIndex, eLogicLevel level) {
    inputs[inputIndex] = level;
    ComputeOutput();
}

// Returns the general output state of the comparator
eLogicLevel COneBitComparator::GetOutputState() const {
    return outputValue;
}

// Computes the outputs for greater, equal, and less based on the two inputs
void COneBitComparator::ComputeOutput() {
    if (inputs[0] == eLogicLevel::LOGIC_HIGH && inputs[1] == eLogicLevel::LOGIC_LOW) {
        greater = eLogicLevel::LOGIC_HIGH;
        equal = eLogicLevel::LOGIC_LOW;
        less = eLogicLevel::LOGIC_LOW;
    } else if (inputs[0] == eLogicLevel::LOGIC_LOW && inputs[1] == eLogicLevel::LOGIC_HIGH) {
        greater = eLogicLevel::LOGIC_LOW;
        equal = eLogicLevel::LOGIC_LOW;
        less = eLogicLevel::LOGIC_HIGH;
    } else if (inputs[0] == inputs[1]) {
        greater = eLogicLevel::LOGIC_LOW;
        equal = eLogicLevel::LOGIC_HIGH;
        less = eLogicLevel::LOGIC_LOW;
    } else {
        greater = equal = less = eLogicLevel::LOGIC_UNDEFINED;
    }
}

// Returns the output state for "greater than" comparison
eLogicLevel COneBitComparator::GetGreaterOutput() const {
    return greater;
}

// Returns the output state for "equal to" comparison
eLogicLevel COneBitComparator::GetEqualOutput() const {
    return equal;
}

// Returns the output state for "less than" comparison
eLogicLevel COneBitComparator::GetLessOutput() const {
    return less;
}
