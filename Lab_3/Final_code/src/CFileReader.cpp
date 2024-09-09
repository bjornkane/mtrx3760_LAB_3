#include "CFileReader.h"
#include <iostream>

// Constructor that initializes the FileReader with a reference to the Circuit
CFileReader::CFileReader(Circuit& circuit) : circuit(circuit) {}

// Processes input commands to build and simulate the circuit
void CFileReader::ProcessInput() {
    std::string Request; 

    // Loop to process each input command
    while (std::cin >> Request) {
        if (Request[0] == '#') {
            std::string DummyVar;
            std::getline(std::cin, DummyVar);  // Ignores comment lines
        } 
        else if (Request == "component") {
            std::string GateType, GateName;
            std::cin >> GateType >> GateName;
            circuit.AddGate(GateType, GateName);  // Adds a gate to the circuit
        } 
        else if (Request == "input") {
            std::string gateName;
            int inputIndex, level;
            std::cin >> gateName >> inputIndex >> level;
            circuit.DriveGate(gateName, inputIndex, static_cast<eLogicLevel>(level));  // Drive input to the gate

        } 
        else if (Request == "output") {
            std::string gateName;
            std::cin >> gateName;
            // Output the result of the specified gate
            std::cout << "Gate " << gateName << " output: " << static_cast<int>(circuit.GetGateOutput(gateName)) << std::endl;
        } 
        else if (Request == "comparator_output") {
            std::string gateName, outputType;
            std::cin >> gateName >> outputType;
            // Output the result of the comparator based on the comparison type
            eLogicLevel result = circuit.GetComparatorOutput(gateName, outputType);
            std::cout << gateName << " " << outputType << " output: " << static_cast<int>(result) << std::endl;
        } 
        else if (Request == "end") {
            break;  // Ends the simulator if end is received.
        }
    }
}
