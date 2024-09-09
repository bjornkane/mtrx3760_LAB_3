#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <unordered_map>
#include <string>
#include <vector>
#include "CLogicGates.h"
#include "CAndGate.h"
#include "COrGate.h"
#include "CXORGate.h"
#include "CNOTGate.h"
#include "COneBitComparator.h"

// Circuit class to manage gates and connections
class Circuit {
public:
    void AddGate(const std::string& gateType, const std::string& gateName);
    void DriveGate(const std::string& gateName, int inputIndex, eLogicLevel level);
    eLogicLevel GetGateOutput(const std::string& gateName) const;
    eLogicLevel GetComparatorOutput(const std::string& gateName, const std::string& outputType) const;
    void AddOutputGate(const std::string& gateName);

private:
    std::unordered_map<std::string, CLogicGates*> gates;
    std::vector<std::string> outputGates;  // Holds the gates marked for output
};

#endif
