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

private:
    void ComputeOutput() {
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

private:
    void ComputeOutput() {
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

private:
    void ComputeOutput() {
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

private:
    void ComputeOutput() {
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

private:
    std::unordered_map<std::string, CLogicGates*> gates;
    std::unordered_map<std::string, std::vector<CWires*>> connections;
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
            } else if (command == "wire") {
                std::string fromGate, toGate;
                int inputIndex;
                file >> fromGate >> toGate >> inputIndex;
                circuit.AddWire(fromGate, toGate, inputIndex);
            } else if (command == "input") {
                std::string gateName;
                int inputIndex, level;
                file >> gateName >> inputIndex >> level;
                circuit.DriveGate(gateName, inputIndex, static_cast<eLogicLevel>(level));
            }
        }
        file.close();
    }

private:
    Circuit& circuit;
};

// Main function to handle parsing and execution
int main() {
    Circuit myCircuit;
    CircuitBuilder builder(myCircuit);

    // Build the circuit from a text file
    builder.BuildCircuitFromFile("circuit_tester.txt");

    // Test all gates by driving inputs and printing outputs dynamically
    std::cout << "Sum: " << static_cast<int>(myCircuit.GetGateOutput("Gate1")) << std::endl;
    std::cout << "Carry: " << static_cast<int>(myCircuit.GetGateOutput("Gate2")) << std::endl;

    return 0;
}
