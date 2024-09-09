#include "Circuit.h"
#include <iostream>

// Adds a gate to the circuit based on the gate type and assigns it a name
void Circuit::AddGate(const std::string& gateType, const std::string& gateName) {
    if (gateType == "AND") {
        gates[gateName] = new CAndGates();          // Add AND gate
    } else if (gateType == "XOR") {
        gates[gateName] = new CXORGates();          // Add XOR gate
    } else if (gateType == "OR") {
        gates[gateName] = new COrGates();           // Add OR gate
    } else if (gateType == "NOT") {
        gates[gateName] = new CNotGate();           // Add NOT gate
    } else if (gateType == "1BitComparator") {
        gates[gateName] = new COneBitComparator();  // Add 1-bit comparator
    } else {
        std::cerr << "Error: Unknown gate type " 
        << gateType << std::endl;                   // Error for unknown gate type
    }
}

// Drives the input of a specified gate with a given logic level
void Circuit::DriveGate(const std::string& gateName, int inputIndex, eLogicLevel level) {
    if (gates.find(gateName) != gates.end()) {
        std::cout << "Input Index " << inputIndex 
        << " of " << gateName << " gate runs with logic " 
                  << static_cast<int>(level) << std::endl;  // Output driven input
        gates[gateName]->DriveInput(inputIndex, level);     // Drive the input of the gate
    } else {
        std::cerr << "Error: Gate " 
        << gateName << " not found." << std::endl;          // Error if gate not found
    }
}

// Returns the output state of a specified gate
eLogicLevel Circuit::GetGateOutput(const std::string& gateName) const {
    if (gates.find(gateName) != gates.end()) {
        return gates.at(gateName)->GetOutputState();  // Return output state of the gate
    } else {
        std::cerr << "Error: Gate " 
        << gateName << " not found." << std::endl;      // Error if gate not found
        return eLogicLevel::LOGIC_UNDEFINED;            // Return undefined logic level if gate not found
    }
}

// Returns the output of a one-bit comparator based on the output type (greater, equal, less)
eLogicLevel Circuit::GetComparatorOutput(const std::string& gateName, const std::string& outputType) const {
    if (gates.find(gateName) != gates.end()) {
        COneBitComparator* comparator = dynamic_cast<COneBitComparator*>(gates.at(gateName));  // Cast to comparator
        if (comparator) {
            if (outputType == "greater") {
                return comparator->GetGreaterOutput();  // Return output for greater
            } else if (outputType == "equal") {
                return comparator->GetEqualOutput();    // Return output for equal
            } else if (outputType == "less") {
                return comparator->GetLessOutput();     // Return output for less
            }
        }
    }
    std::cerr << "Error: Gate " << gateName 
    << " not found or not a comparator." << std::endl;  // Error for invalid gate or comparator
    return eLogicLevel::LOGIC_UNDEFINED;                // Return undefined logic level if gate or comparator not found
}

// Adds a gate to the list of output gates
void Circuit::AddOutputGate(const std::string& gateName) {
    outputGates.push_back(gateName);                    // Add gate name to outputGates vector
}
