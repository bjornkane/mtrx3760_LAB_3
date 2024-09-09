#include "Comparator.h"

COneBitComparator::COneBitComparator() {
    inputs.resize(2, eLogicLevel::LOGIC_UNDEFINED);
}

void COneBitComparator::DriveInput(int inputIndex, eLogicLevel level) {
    inputs[inputIndex] = level;
    ComputeOutput();
}

eLogicLevel COneBitComparator::GetOutputState() const {
    return outputValue;
}

eLogicLevel COneBitComparator::GetGreaterOutput() const {
    return greater;
}

eLogicLevel COneBitComparator::GetEqualOutput() const {
    return equal;
}

eLogicLevel COneBitComparator::GetLessOutput() const {
    return less;
}

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
