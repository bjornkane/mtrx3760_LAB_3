#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// Constants and enums
const int InputsPerGate = 2;                 // Number of inputs per gate
const int MaxFanout = 4;                     // Maximum fanout: max gate inputs that one gate output can drive

// Enum defining the possible states of a logic line
enum eLogicLevel { LOGIC_UNDEFINED = -1, LOGIC_LOW, LOGIC_HIGH };

// Base class for all gates
class CLogicGates {
public:
    virtual ~CLogicGates() = default;
    
    virtual void DriveInput(int aInputIndex, eLogicLevel aNewLevel) = 0; // Drives input to the gate
    virtual eLogicLevel GetOutputState() const = 0;                      // Gets the output state of the gate
    virtual void ConnectOutput(class CWires* apOutputConnection) = 0;    // Connects the gate's output to a wire

protected:
    CLogicGates() : mOutputValue(LOGIC_UNDEFINED), mpOutputConnection(nullptr) {
        mInputs[0] = LOGIC_UNDEFINED;
        mInputs[1] = LOGIC_UNDEFINED;
    }

    virtual void ComputeOutput() = 0;   // Computes the output state of the gate

    eLogicLevel mInputs[InputsPerGate]; // Array to store the input states of the gate
    eLogicLevel mOutputValue;           // Stores the current output state of the gate
    CWires* mpOutputConnection;         // Pointer to the wire this gate's output is connected to
};

// CWires class
class CWires {
public:
    CWires() : mNumOutputConnections(0) {
        for (int i = 0; i < MaxFanout; ++i) {
            mpGatesToDrive[i] = nullptr;
            mGateInputIndices[i] = -1;
        }
    }

    void AddOutputConnection(CLogicGates* apGateToDrive, int aGateInputToDrive) {
        if (mNumOutputConnections < MaxFanout) {
            mpGatesToDrive[mNumOutputConnections] = apGateToDrive;
            mGateInputIndices[mNumOutputConnections] = aGateInputToDrive;
            ++mNumOutputConnections;
        }
    }

    void DriveLevel(eLogicLevel aNewLevel) {
        for (int i = 0; i < mNumOutputConnections; ++i) {
            if (mpGatesToDrive[i] != nullptr) {
                mpGatesToDrive[i]->DriveInput(mGateInputIndices[i], aNewLevel);
            }
        }
    }

private:
    int mNumOutputConnections;
    CLogicGates* mpGatesToDrive[MaxFanout];
    int mGateInputIndices[MaxFanout];
};

// Specific gate implementations (AND, OR, XOR, NOT)
class CAndGates : public CLogicGates {
public:
    CAndGates() { }
    void DriveInput(int aInputIndex, eLogicLevel aNewLevel) override {
        if (aInputIndex >= 0 && aInputIndex < InputsPerGate) {
            mInputs[aInputIndex] = aNewLevel;
            ComputeOutput();
        }
    }
    eLogicLevel GetOutputState() const override { return mOutputValue; }
    void ConnectOutput(CWires* apOutputConnection) override { mpOutputConnection = apOutputConnection; }

protected:
    void ComputeOutput() override {
        eLogicLevel NewVal = LOGIC_HIGH;
        if (mInputs[0] == LOGIC_LOW || mInputs[1] == LOGIC_LOW) {
            NewVal = LOGIC_LOW;
        }
        mOutputValue = NewVal;
        if (mpOutputConnection != nullptr) {
            mpOutputConnection->DriveLevel(mOutputValue);
        }
    }
};

class CORGates : public CLogicGates {
public:
    CORGates() { }
    void DriveInput(int aInputIndex, eLogicLevel aNewLevel) override {
        if (aInputIndex >= 0 && aInputIndex < InputsPerGate) {
            mInputs[aInputIndex] = aNewLevel;
            ComputeOutput();
        }
    }
    eLogicLevel GetOutputState() const override { return mOutputValue; }
    void ConnectOutput(CWires* apOutputConnection) override { mpOutputConnection = apOutputConnection; }

protected:
    void ComputeOutput() override {
        eLogicLevel NewVal = LOGIC_LOW;
        if (mInputs[0] == LOGIC_HIGH || mInputs[1] == LOGIC_HIGH) {
            NewVal = LOGIC_HIGH;
        }
        mOutputValue = NewVal;
        if (mpOutputConnection != nullptr) {
            mpOutputConnection->DriveLevel(mOutputValue);
        }
    }
};

class CXORGates : public CLogicGates {
public:
    CXORGates() { }
    void DriveInput(int aInputIndex, eLogicLevel aNewLevel) override {
        if (aInputIndex >= 0 && aInputIndex < InputsPerGate) {
            mInputs[aInputIndex] = aNewLevel;
            ComputeOutput();
        }
    }
    eLogicLevel GetOutputState() const override { return mOutputValue; }
    void ConnectOutput(CWires* apOutputConnection) override { mpOutputConnection = apOutputConnection; }

protected:
    void ComputeOutput() override {
        if ((mInputs[0] == LOGIC_HIGH && mInputs[1] == LOGIC_LOW) || (mInputs[0] == LOGIC_LOW && mInputs[1] == LOGIC_HIGH)) {
            mOutputValue = LOGIC_HIGH;
        } else {
            mOutputValue = LOGIC_LOW;
        }
        if (mpOutputConnection != nullptr) {
            mpOutputConnection->DriveLevel(mOutputValue);
        }
    }
};

// New class for NOT gate
class CNotGates : public CLogicGates {
public:
    CNotGates() {
        mInputs[1] = LOGIC_UNDEFINED; // NOT gate only has 1 input
    }
    void DriveInput(int aInputIndex, eLogicLevel aNewLevel) override {
        if (aInputIndex == 0) {
            mInputs[0] = aNewLevel;
            ComputeOutput();
        }
    }
    eLogicLevel GetOutputState() const override { return mOutputValue; }
    void ConnectOutput(CWires* apOutputConnection) override { mpOutputConnection = apOutputConnection; }

protected:
    void ComputeOutput() override {
        mOutputValue = (mInputs[0] == LOGIC_HIGH) ? LOGIC_LOW : LOGIC_HIGH;
        if (mpOutputConnection != nullptr) {
            mpOutputConnection->DriveLevel(mOutputValue);
        }
    }
};

// Circuit class
class Circuit {
public:
    Circuit() { WireConnections.reserve(100); }
    void LoadCircuitFromFile(const std::string& filename);
    void TestInputStates(const std::string& gateType);
    void TestCircuit();

private:
    std::unordered_map<int, CLogicGates*> Gates; // Maps gate IDs to gate objects
    std::unordered_map<std::string, int> GateTypeIDMap; // Maps gate types to their corresponding IDs
    std::vector<CWires*> WireConnections;   // Dynamic list of wires

    CLogicGates* CreateGate(const std::string& gateType);
};

CLogicGates* Circuit::CreateGate(const std::string& gateType) {
    if (gateType == "AND") return new CAndGates();
    if (gateType == "OR") return new CORGates();
    if (gateType == "XOR") return new CXORGates();
    if (gateType == "NOT") return new CNotGates(); // Add NOT gate creation
    return nullptr;
}

void Circuit::LoadCircuitFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file '" << filename << "'. Please check the file path and permissions." << std::endl;
    return;
}
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "GATE") {
            std::string gateType;
            int gateId, input1, input2, output;
            iss >> gateType >> gateId >> input1 >> input2 >> output;
            CLogicGates* gate = CreateGate(gateType);
            Gates[gateId] = gate;
            GateTypeIDMap[gateType] = gateId;

            if (output >= WireConnections.size()) {
                WireConnections.resize(output + 1, nullptr);
            }

            if (!WireConnections[output]) {
                WireConnections[output] = new CWires();
            }

            WireConnections[output]->AddOutputConnection(gate, 0);
        }
        else if (command == "WIRE") {
            int outputWire, inputWire;
            iss >> outputWire >> inputWire;

            if (outputWire >= WireConnections.size()) {
                WireConnections.resize(outputWire + 1, nullptr);
            }

            if (!WireConnections[outputWire]) {
                WireConnections[outputWire] = new CWires();
            }

            WireConnections[outputWire]->AddOutputConnection(Gates[inputWire], 0);
        }
    }

    file.close();
}

void Circuit::TestInputStates(const std::string& gateType) 
{
    std::cout << gateType << std::endl;
    int gateId = GateTypeIDMap[gateType.substr(0, gateType.find(" "))];

    if (gateType == "AND GATE") 
    {
        eLogicLevel inputs[4][2] = {
            {LOGIC_LOW, LOGIC_LOW},  // A = 0, B = 0
            {LOGIC_LOW, LOGIC_HIGH}, // A = 0, B = 1
            {LOGIC_HIGH, LOGIC_LOW}, // A = 1, B = 0
            {LOGIC_HIGH, LOGIC_HIGH} // A = 1, B = 1
        };

        for (int i = 0; i < 4; ++i) 
        {
            WireConnections[0]->DriveLevel(inputs[i][0]);
            WireConnections[1]->DriveLevel(inputs[i][1]);
            std::cout << "Input: A = " << (inputs[i][0] == LOGIC_HIGH ? 1 : 0)
                      << ", B = " << (inputs[i][1] == LOGIC_HIGH ? 1 : 0)
                      << "; Output: " << (Gates[gateId]->GetOutputState() == LOGIC_HIGH ? 1 : 0) << std::endl;
        }
        std::cout << std::endl;
    }

        if (gateType == "OR GATE") 
    {
        eLogicLevel inputs[4][2] = {
            {LOGIC_LOW, LOGIC_LOW},  // A = 0, B = 0
            {LOGIC_LOW, LOGIC_HIGH}, // A = 0, B = 1
            {LOGIC_HIGH, LOGIC_LOW}, // A = 1, B = 0
            {LOGIC_HIGH, LOGIC_HIGH} // A = 1, B = 1
        };

        for (int i = 0; i < 4; ++i) 
        {
            WireConnections[0]->DriveLevel(inputs[i][0]);
            WireConnections[1]->DriveLevel(inputs[i][1]);
            std::cout << "Input: A = " << (inputs[i][0] == LOGIC_HIGH ? 1 : 0)
                      << ", B = " << (inputs[i][1] == LOGIC_HIGH ? 1 : 0)
                      << "; Output: " << (Gates[gateId]->GetOutputState() == LOGIC_HIGH ? 1 : 0) << std::endl;
        }
        std::cout << std::endl;
    }

        if (gateType == "XOR GATE") 
    {
        eLogicLevel inputs[4][2] = {
            {LOGIC_LOW, LOGIC_LOW},  // A = 0, B = 0
            {LOGIC_LOW, LOGIC_HIGH}, // A = 0, B = 1
            {LOGIC_HIGH, LOGIC_LOW}, // A = 1, B = 0
            {LOGIC_HIGH, LOGIC_HIGH} // A = 1, B = 1
        };

        for (int i = 0; i < 4; ++i) 
        {
            WireConnections[0]->DriveLevel(inputs[i][0]);
            WireConnections[1]->DriveLevel(inputs[i][1]);
            std::cout << "Input: A = " << (inputs[i][0] == LOGIC_HIGH ? 1 : 0)
                      << ", B = " << (inputs[i][1] == LOGIC_HIGH ? 1 : 0)
                      << "; Output: " << (Gates[gateId]->GetOutputState() == LOGIC_HIGH ? 1 : 0) << std::endl;
        }
        std::cout << std::endl;
    }

    if (gateType == "NOT GATE")
    {
        eLogicLevel inputs[2] = {
            LOGIC_LOW,  // A = 0
            LOGIC_HIGH  // A = 1
        };

        for (int i = 0; i < 2; ++i)
        {
            WireConnections[0]->DriveLevel(inputs[i]);
            std::cout << "Input: A = " << (inputs[i] == LOGIC_HIGH ? 1 : 0)
                      << "; Output: " << (Gates[gateId]->GetOutputState() == LOGIC_HIGH ? 1 : 0) << std::endl;
        }
        std::cout << std::endl;
    }
}

void Circuit::TestCircuit() {
    TestInputStates("AND GATE");
    TestInputStates("OR GATE");
    TestInputStates("XOR GATE");
    TestInputStates("NOT GATE"); // Test the NOT gate
}

int main() {
    Circuit myCircuit;

    // Load the circuit from a description file
    myCircuit.LoadCircuitFromFile("C:\\Users\\varun\\Desktop\\USYD\\2024 SEM 2 (sem 5)\\MTRX3760\\Lab 3\\MTRX3760\\Lab 3\\circuit_tester.txt");

    // Test the circuit after it's dynamically loaded
    myCircuit.TestCircuit();

    return 0;
}
