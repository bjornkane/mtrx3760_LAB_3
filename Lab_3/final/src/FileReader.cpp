#include "FileReader.h"
#include <iostream>

CFileReader::CFileReader(Circuit& circuit) : circuit(circuit) {}

void CFileReader::ProcessInput() {
    std::string Request;
    int A = -1, B = -1;

    while (std::cin >> Request) {
        if (Request[0] == '#') {
            std::string DummyVar;
            std::getline(std::cin, DummyVar);
        } else if (Request == "component") {
            std::string GateType, GateName;
            std::cin >> GateType >> GateName;
            circuit.AddGate(GateType, GateName);
        } else if (Request == "input") {
            std::string gateName;
            int inputIndex, level;
            std::cin >> gateName >> inputIndex >> level;
            circuit.DriveGate(gateName, inputIndex, static_cast<eLogicLevel>(level));

            if (gateName == "xor1") {
                if (inputIndex == 0) A = level;
                else if (inputIndex == 1) B = level;
            }
        } else if (Request == "output") {
            std::string gateName;
            std::cin >> gateName;
            std::cout << "Gate " << gateName << " output: " << static_cast<int>(circuit.GetGateOutput(gateName)) << std::endl;
        } else if (Request == "comparator_output") {
            std::string gateName, outputType;
            std::cin >> gateName >> outputType;
            eLogicLevel result = circuit.GetComparatorOutput(gateName, outputType);
            std::cout << gateName << " " << outputType << " output: " << static_cast<int>(result) << std::endl;
        } else if (Request == "end") {
            break;
        }
    }
}
