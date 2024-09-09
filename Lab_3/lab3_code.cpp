#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

// Enum defining the possible states of a logic line
enum class eLogicLevel { LOGIC_UNDEFINED = -1, LOGIC_LOW = 0, LOGIC_HIGH = 1 };

// Base class for all gates
class CLogicGates {
public:
    virtual ~CLogicGates() = default;
    virtual void DriveInput(int inputIndex, eLogicLevel level) = 0;
    virtual eLogicLevel GetOutputState() const = 0;

protected:
    virtual void ComputeOutput() = 0;
    std::vector<eLogicLevel> inputs;
    eLogicLevel outputValue = eLogicLevel::LOGIC_UNDEFINED;
};

// CWires class for managing connections
class CWires {
public:
    void AddOutputConnection(CLogicGates* gate, int inputIndex) {
        connections.push_back({gate, inputIndex});
    }

    void DriveLevel(eLogicLevel level) {
        for (auto& conn : connections) {
            conn.first->DriveInput(conn.second, level);
        }
    }

private:
    std::vector<std::pair<CLogicGates*, int>> connections;
};

// Derived classes for each logic gate
class CAndGates : public CLogicGates {
public:
    CAndGates() { inputs.resize(2, eLogicLevel::LOGIC_UNDEFINED); }
    void DriveInput(int inputIndex, eLogicLevel level) override {
        inputs[inputIndex] = level;
        ComputeOutput();
    }
    eLogicLevel GetOutputState() const override { return outputValue; }

protected:
    void ComputeOutput() override {
        outputValue = (inputs[0] == eLogicLevel::LOGIC_HIGH && inputs[1] == eLogicLevel::LOGIC_HIGH) ? eLogicLevel::LOGIC_HIGH : eLogicLevel::LOGIC_LOW;
    }
};

class CORGates : public CLogicGates {
public:
    CORGates() { inputs.resize(2, eLogicLevel::LOGIC_UNDEFINED); }
    void DriveInput(int inputIndex, eLogicLevel level) override {
        inputs[inputIndex] = level;
        ComputeOutput();
    }
    eLogicLevel GetOutputState() const override { return outputValue; }

protected:
    void ComputeOutput() override {
        outputValue = (inputs[0] == eLogicLevel::LOGIC_HIGH || inputs[1] == eLogicLevel::LOGIC_HIGH) ? eLogicLevel::LOGIC_HIGH : eLogicLevel::LOGIC_LOW;
    }
};

class CXORGates : public CLogicGates {
public:
    CXORGates() { inputs.resize(2, eLogicLevel::LOGIC_UNDEFINED); }
    void DriveInput(int inputIndex, eLogicLevel level) override {
        inputs[inputIndex] = level;
        ComputeOutput();
    }
    eLogicLevel GetOutputState() const override { return outputValue; }

protected:
    void ComputeOutput() override {
        outputValue = (inputs[0] != inputs[1]) ? eLogicLevel::LOGIC_HIGH : eLogicLevel::LOGIC_LOW;
    }
};

class CNotGates : public CLogicGates {
public:
    CNotGates() { inputs.resize(1, eLogicLevel::LOGIC_UNDEFINED); }
    void DriveInput(int inputIndex, eLogicLevel level) override {
        inputs[0] = level;
        ComputeOutput();
    }
    eLogicLevel GetOutputState() const override { return outputValue; }

protected:
    void ComputeOutput() override {
        outputValue = (inputs[0] == eLogicLevel::LOGIC_HIGH) ? eLogicLevel::LOGIC_LOW : eLogicLevel::LOGIC_HIGH;
    }
};

// Circuit class to manage gates and connections
class Circuit {
public:
    void AddGate(const std::string& gateType, const std::string& gateName) {
        if (gateType == "AND") gates[gateName] = new CAndGates();
        else if (gateType == "OR") gates[gateName] = new CORGates();
        else if (gateType == "XOR") gates[gateName] = new CXORGates();
        else if (gateType == "NOT") gates[gateName] = new CNotGates();
    }

    void AddWire(const std::string& fromGate, const std::string& toGate, int inputIndex) {
        CWires* wire = new CWires();
        wire->AddOutputConnection(gates[toGate], inputIndex);
        connections[fromGate].push_back(wire);
    }

    void DriveGate(const std::string& gateName, int inputIndex, eLogicLevel level) {
        gates[gateName]->DriveInput(inputIndex, level);
        for (auto& wire : connections[gateName]) {
            wire->DriveLevel(gates[gateName]->GetOutputState());
        }
    }

    eLogicLevel GetGateOutput(const std::string& gateName) const {
        return gates.at(gateName)->GetOutputState();
    }

    void AddOutputGate(const std::string& gateName) {
        outputGates.push_back(gateName);
    }

    void PrintOutputs() const {
        if (outputGates.empty()) {
            std::cout << "No output gates defined!" << std::endl;
            return;
        }
        for (const auto& gateName : outputGates) {
            std::cout << gateName << " output: " << static_cast<int>(gates.at(gateName)->GetOutputState()) << std::endl;
        }
    }

private:
    std::unordered_map<std::string, CLogicGates*> gates;
    std::unordered_map<std::string, std::vector<CWires*>> connections;
    std::vector<std::string> outputGates;  // Holds the gates marked for output
};

// CircuitBuilder class for reading and building the circuit from a file
class CircuitBuilder {
public:
    CircuitBuilder(Circuit& circuit) : circuit(circuit) {}

    void BuildCircuitFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return;
        }

        std::string command;
        while (file >> command) {
            if (command == "component") {
                std::string gateType, gateName;
                file >> gateType >> gateName;
                circuit.AddGate(gateType, gateName);
                std::cout << "Added component: " << gateType << " " << gateName << std::endl;  // Debug info
            } else if (command == "wire") {
                std::string fromGate, toGate;
                int inputIndex;
                file >> fromGate >> toGate >> inputIndex;
                circuit.AddWire(fromGate, toGate, inputIndex);
                std::cout << "Connected wire from: " << fromGate << " to " << toGate << std::endl;  // Debug info
            } else if (command == "input") {
                std::string gateName;
                int inputIndex, level;
                file >> gateName >> inputIndex >> level;
                circuit.DriveGate(gateName, inputIndex, static_cast<eLogicLevel>(level));
                std::cout << "Set input for gate: " << gateName << " InputIndex: " << inputIndex << " Level: " << level << std::endl;  // Debug info
            } else if (command == "output") {
                std::string gateName;
                file >> gateName;
                circuit.AddOutputGate(gateName);
                std::cout << "Marked gate for output: " << gateName << std::endl;  // Debug info
            }
        }
        file.close();
    }

private:
    Circuit& circuit;
};

int main() {
    Circuit myCircuit;
    CircuitBuilder builder(myCircuit);

    // Build the circuit from a text file
    builder.BuildCircuitFromFile("circuit_tester.txt");

    // Print only the outputs for the gates marked with "output"
    myCircuit.PrintOutputs();

    return 0;  // Terminate the main function
}
