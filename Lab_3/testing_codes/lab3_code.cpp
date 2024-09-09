#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

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

// AND Gate
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

// OR Gate
class COrGates : public CLogicGates {
public:
    COrGates() { inputs.resize(2, eLogicLevel::LOGIC_UNDEFINED); }

    void DriveInput(int inputIndex, eLogicLevel level) override {
        inputs[inputIndex] = level;
        ComputeOutput();
    }

    eLogicLevel GetOutputState() const override { return outputValue; }

protected:
    void ComputeOutput() override {
        outputValue = (inputs[0] == eLogicLevel::LOGIC_HIGH || inputs[1] == eLogicLevel::LOGIC_HIGH) 
                      ? eLogicLevel::LOGIC_HIGH : eLogicLevel::LOGIC_LOW;
    }
};

// XOR Gate
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

// NOT Gate
class CNotGate : public CLogicGates {
public:
    CNotGate() { inputs.resize(1, eLogicLevel::LOGIC_UNDEFINED); } // NOT gate has one input
    
    void DriveInput(int inputIndex, eLogicLevel level) override {
        inputs[inputIndex] = level;
        ComputeOutput();
    }
    
    eLogicLevel GetOutputState() const override { return outputValue; }

protected:
    void ComputeOutput() override {
        outputValue = (inputs[0] == eLogicLevel::LOGIC_HIGH) ? eLogicLevel::LOGIC_LOW : eLogicLevel::LOGIC_HIGH;
    }
};

// One-Bit Comparator class
class COneBitComparator : public CLogicGates {
public:
    COneBitComparator() { inputs.resize(2, eLogicLevel::LOGIC_UNDEFINED); }
    
    void DriveInput(int inputIndex, eLogicLevel level) override {
        inputs[inputIndex] = level;
        ComputeOutput();
    }
    
    eLogicLevel GetOutputState() const override { return outputValue; }

    // Additional methods to get outputs for A > B, A = B, A < B
    eLogicLevel GetGreaterOutput() const { return greater; }
    eLogicLevel GetEqualOutput() const { return equal; }
    eLogicLevel GetLessOutput() const { return less; }

protected:
    void ComputeOutput() override {
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

private:
    eLogicLevel greater = eLogicLevel::LOGIC_UNDEFINED;
    eLogicLevel equal = eLogicLevel::LOGIC_UNDEFINED;
    eLogicLevel less = eLogicLevel::LOGIC_UNDEFINED;
};

// Circuit class to manage gates and connections
class Circuit {
public:
    void AddGate(const std::string& gateType, const std::string& gateName) {
        if (gateType == "AND") gates[gateName] = new CAndGates();
        else if (gateType == "XOR") gates[gateName] = new CXORGates();
        else if (gateType == "OR") gates[gateName] = new COrGates();
        else if (gateType == "NOT") gates[gateName] = new CNotGate();
        else if (gateType == "1BitComparator") gates[gateName] = new COneBitComparator();
    }

    void DriveGate(const std::string& gateName, int inputIndex, eLogicLevel level) {
        if (gates.find(gateName) != gates.end()) {
            std::cout << "Driving input " << inputIndex << " of gate " << gateName << " with level " << static_cast<int>(level) << std::endl;
            gates[gateName]->DriveInput(inputIndex, level);
        } else {
            std::cerr << "Error: Gate " << gateName << " not found." << std::endl;
        }
    }

    eLogicLevel GetGateOutput(const std::string& gateName) const {
        if (gates.find(gateName) != gates.end()) {
            return gates.at(gateName)->GetOutputState();
        } else {
            std::cerr << "Error: Gate " << gateName << " not found." << std::endl;
            return eLogicLevel::LOGIC_UNDEFINED;
        }
    }

    eLogicLevel GetComparatorOutput(const std::string& gateName, const std::string& outputType) const {
        if (gates.find(gateName) != gates.end()) {
            COneBitComparator* comparator = dynamic_cast<COneBitComparator*>(gates.at(gateName));
            if (comparator) {
                if (outputType == "greater") return comparator->GetGreaterOutput();
                else if (outputType == "equal") return comparator->GetEqualOutput();
                else if (outputType == "less") return comparator->GetLessOutput();
            }
        }
        std::cerr << "Error: Gate " << gateName << " not found or not a comparator." << std::endl;
        return eLogicLevel::LOGIC_UNDEFINED;
    }

    void AddOutputGate(const std::string& gateName) {
        outputGates.push_back(gateName);
    }

private:
    std::unordered_map<std::string, CLogicGates*> gates;
    std::vector<std::string> outputGates;  // Holds the gates marked for output
};

// New class to handle file reading
class CFileReader {
public:
    CFileReader(Circuit& circuit) : circuit(circuit) {}

    void ProcessInput() {
        std::string Request;
        int A = -1, B = -1;  // Store the values of A and B

        while (std::cin >> Request) {
            if (Request[0] == '#') {
                std::string DummyVar;
                std::getline(std::cin, DummyVar);  // Ignore comment lines
            } else if (Request == "component") {
                std::string GateType, GateName;
                std::cin >> GateType >> GateName;
                std::cout << "Adding gate of type " << GateType << " named " << GateName << std::endl;
                circuit.AddGate(GateType, GateName);
            } else if (Request == "input") {
                std::string gateName;
                int inputIndex, level;
                std::cin >> gateName >> inputIndex >> level;
                circuit.DriveGate(gateName, inputIndex, static_cast<eLogicLevel>(level));

                // Capture A and B values when XOR gate is set
                if (gateName == "xor1") {
                    if (inputIndex == 0) A = level;  // Capture A value
                    else if (inputIndex == 1) B = level;  // Capture B value
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
                break;  // End of input
            } else {
                std::cout << "Unrecognised command " << Request << std::endl;
            }
        }
    }

private:
    Circuit& circuit;
};

// Main function
int main() {
    Circuit myCircuit;
    CFileReader fileReader(myCircuit);
    fileReader.ProcessInput();

    return 0;
}
