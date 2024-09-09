#include "Circuit.h"
#include "AndGate.h"
#include "OrGate.h"
#include "XORGate.h"
#include "NotGate.h"
#include "Comparator.h"

void Circuit::AddGate(const std::string& gateType, const std::string& gateName) {
    if (gateType == "AND") gates[gateName] = new CAndGates();
    else if (gateType == "XOR") gates[gateName] = new CXORGates();
    else if (gateType == "OR") gates[gateName] = new COrGates();
    else if (gateType == "NOT") gates[gateName] = new CNotGate();
    else if (gateType == "1BitComparator") gates[gateName] = new COneBitComparator();
}

void Circuit::DriveGate(const std::string& gateName, int inputIndex, eLogicLevel level) {
    if (gates.find(gateName) != gates.end()) {
        gates[gateName]->DriveInput(inputIndex, level);
    }
}

eLogicLevel Circuit::GetGateOutput(const std::string& gateName) const {
    if (gates.find(gateName) != gates.end()) {
        return gates.at(gateName)->GetOutputState();
    }
    return eLogicLevel::LOGIC_UNDEFINED;
}

eLogicLevel Circuit::GetComparatorOutput(const std::string& gateName, const std::string& outputType) const {
    if (gates.find(gateName) != gates.end()) {
        COneBitComparator* comparator = dynamic_cast<COneBitComparator*>(gates.at(gateName));
        if (comparator) {
            if (outputType == "greater") return comparator->GetGreaterOutput();
            else if (outputType == "equal") return comparator->GetEqualOutput();
            else if (outputType == "less") return comparator->GetLessOutput();
        }
    }
    return eLogicLevel::LOGIC_UNDEFINED;
}

void Circuit::AddOutputGate(const std::string& gateName) {
    outputGates.push_back(gateName);
}
