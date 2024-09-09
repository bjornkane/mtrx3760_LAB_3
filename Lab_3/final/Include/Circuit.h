#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <unordered_map>
#include <string>
#include "LogicGates.h"

class Circuit {
public:
    void AddGate(const std::string& gateType, const std::string& gateName);
    void DriveGate(const std::string& gateName, int inputIndex, eLogicLevel level);
    eLogicLevel GetGateOutput(const std::string& gateName) const;
    eLogicLevel GetComparatorOutput(const std::string& gateName, const std::string& outputType) const;
    void AddOutputGate(const std::string& gateName);

private:
    std::unordered_map<std::string, CLogicGates*> gates;
    std::vector<std::string> outputGates;
};

#endif  // CIRCUIT_H
