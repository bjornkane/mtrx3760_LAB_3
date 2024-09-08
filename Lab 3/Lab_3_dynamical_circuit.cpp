#include <iostream>
#include <string>

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
    CLogicGates();                      // Constructor to initialize the gate
    virtual void ComputeOutput();       // Computes the output state of the gate

    eLogicLevel mInputs[InputsPerGate]; // Array to store the input states of the gate
    eLogicLevel mOutputValue;           // Stores the current output state of the gate
    CWires* mpOutputConnection;         // Pointer to the wire this gate's output is connected to
};

// CWires class
class CWires {
public:
    CWires();                                                                        // Constructor to initialize the wire
    void AddOutputConnection(CLogicGates* apGateToDrive, int aGateInputToDrive);     // Adds a gate to be driven by this wire
    void DriveLevel(eLogicLevel aNewLevel);                                          // Passes the logic level through to the connected gates

private:
    int mNumOutputConnections;              // Number of inputs, the output of this wire is connected to
    CLogicGates* mpGatesToDrive[MaxFanout]; // Array of pointers to gates this wire drives
    int mGateInputIndices[MaxFanout];       // Indices of the inputs in the gates that this wire is connected to
};

// CLogicGates class implementation
CLogicGates::CLogicGates() 
    : mOutputValue(LOGIC_UNDEFINED),
      mpOutputConnection(nullptr) 
{
    mInputs[0] = LOGIC_UNDEFINED;
    mInputs[1] = LOGIC_UNDEFINED;
}

void CLogicGates::ComputeOutput() {
    // Base implementation for derived classes to override
}

// CWires class implementation
CWires::CWires() 
    : mNumOutputConnections(0) 
{
    for (int i = 0; i < MaxFanout; ++i) 
    {
        mpGatesToDrive[i] = nullptr;   // Initialize gate pointers to nullptr
        mGateInputIndices[i] = -1;     // Initialize gate input indices to -1
    }
}

void CWires::AddOutputConnection(CLogicGates* apGateToDrive, int aGateInputToDrive) {
    if (mNumOutputConnections < MaxFanout) 
    {
        mpGatesToDrive[mNumOutputConnections] = apGateToDrive;          // Store the gate pointer
        mGateInputIndices[mNumOutputConnections] = aGateInputToDrive;   // Store the input index
        ++mNumOutputConnections;                                        // Increment the number of connections
    }
}

void CWires::DriveLevel(eLogicLevel aNewLevel) {
    for (int i = 0; i < mNumOutputConnections; ++i) 
    {
        if (mpGatesToDrive[i] != nullptr) 
        {
            mpGatesToDrive[i]->DriveInput(mGateInputIndices[i], aNewLevel); // Set the input level on each connected gate
        }
    }
}

// CAndGates class
class CAndGates : public CLogicGates {
public:
    CAndGates();                                         // Constructor to initialize the AND gate
    void DriveInput(int aInputIndex, eLogicLevel aNewLevel) override;  // Drives an input to the AND gate
    eLogicLevel GetOutputState() const override;        // Returns the current output state of the AND gate
    void ConnectOutput(CWires* apOutputConnection) override; // Connects the output of the AND gate to a wire

private:
    void ComputeOutput() override;    // Computes the output based on the inputs
};

// CORGates class
class CORGates : public CLogicGates {
public:
    CORGates();                                           // Constructor to initialize the OR gate
    void DriveInput(int aInputIndex, eLogicLevel aNewLevel) override;  // Drives an input to the OR gate
    eLogicLevel GetOutputState() const override;         // Returns the current output state of the OR gate
    void ConnectOutput(CWires* apOutputConnection) override; // Connects the output of the OR gate to a wire

private:
    void ComputeOutput() override;    // Computes the output based on the inputs
};

// CXORGates class
class CXORGates : public CLogicGates {
public:
    CXORGates();                                            // Constructor to initialize the XOR gate
    void DriveInput(int aInputIndex, eLogicLevel aNewLevel) override;  // Drives an input to the XOR gate
    eLogicLevel GetOutputState() const override;           // Returns the current output state of the XOR gate
    void ConnectOutput(CWires* apOutputConnection) override; // Connects the output of the XOR gate to a wire

private:
    void ComputeOutput() override;    // Computes the output based on the inputs
};

// CNotGates class
class CNotGates : public CLogicGates {
public:
    CNotGates();                                            // Constructor to initialize the NOT gate
    void DriveInput(int aInputIndex, eLogicLevel aNewLevel) override;  // Drives an input to the NOT gate
    eLogicLevel GetOutputState() const override;           // Returns the current output state of the NOT gate
    void ConnectOutput(CWires* apOutputConnection) override; // Connects the output of the NOT gate to a wire

private:
    void ComputeOutput() override;    // Computes the output based on the input
};

// CAndGates class implementation
CAndGates::CAndGates() 
{
    mInputs[0] = LOGIC_UNDEFINED;
    mInputs[1] = LOGIC_UNDEFINED;
}

void CAndGates::DriveInput(int aInputIndex, eLogicLevel aNewLevel) 
{
    if (aInputIndex >= 0 && aInputIndex < InputsPerGate) 
    {
        mInputs[aInputIndex] = aNewLevel;   // Set the input level
        ComputeOutput();                    // Recompute the output based on the new input
    }
}

eLogicLevel CAndGates::GetOutputState() const 
{
    return mOutputValue;  // Return the current output state
}

void CAndGates::ConnectOutput(CWires* apOutputConnection) 
{
    mpOutputConnection = apOutputConnection;  // Stores the wire pointer
}

void CAndGates::ComputeOutput() 
{
    eLogicLevel NewVal = LOGIC_HIGH;    // Default output state is HIGH (1)
    if (mInputs[0] == LOGIC_LOW || mInputs[1] == LOGIC_LOW) 
    {
        NewVal = LOGIC_LOW;             // If any input is LOW, output is LOW
    }
    mOutputValue = NewVal;              // Update the output state of the AND gate

    if (mpOutputConnection != nullptr) 
    {
        mpOutputConnection->DriveLevel(mOutputValue);  // Drive the wire connected to the AND gate's output
    }
}

// CORGates class implementation
CORGates::CORGates() 
{
    mInputs[0] = LOGIC_UNDEFINED;
    mInputs[1] = LOGIC_UNDEFINED;
}

void CORGates::DriveInput(int aInputIndex, eLogicLevel aNewLevel) 
{
    if (aInputIndex >= 0 && aInputIndex < InputsPerGate) 
    {
        mInputs[aInputIndex] = aNewLevel;   // Set the input level
        ComputeOutput();                    // Recompute the output based on the new input
    }
}

eLogicLevel CORGates::GetOutputState() const 
{
    return mOutputValue;  // Return the current output state
}

void CORGates::ConnectOutput(CWires* apOutputConnection) 
{
    mpOutputConnection = apOutputConnection;  // Store the wire pointer
}

void CORGates::ComputeOutput() 
{
    eLogicLevel NewVal = LOGIC_LOW;     // Default output state is LOW
    if (mInputs[0] == LOGIC_HIGH || mInputs[1] == LOGIC_HIGH) 
    {
        NewVal = LOGIC_HIGH;            // If any input is HIGH, output is HIGH
    }
    mOutputValue = NewVal;              // Update the output state of the OR gate

    if (mpOutputConnection != nullptr) 
    {
        mpOutputConnection->DriveLevel(mOutputValue);  // Drive the wire connected to the OR gate's output
    }
}

// CXORGates class implementation
CXORGates::CXORGates() 
{
    mInputs[0] = LOGIC_UNDEFINED;
    mInputs[1] = LOGIC_UNDEFINED;
}

void CXORGates::DriveInput(int aInputIndex, eLogicLevel aNewLevel) 
{
    if (aInputIndex >= 0 && aInputIndex < InputsPerGate) 
    {
        mInputs[aInputIndex] = aNewLevel;   // Set the input level
        ComputeOutput();                    // Recompute the output based on the new input
    }
}

eLogicLevel CXORGates::GetOutputState() const 
{
    return mOutputValue;  // Return the current output state
}

void CXORGates::ConnectOutput(CWires* apOutputConnection) 
{
    mpOutputConnection = apOutputConnection;  // Store the wire pointer
}

void CXORGates::ComputeOutput() 
{
    // XOR logic: output is HIGH if exactly one input is HIGH
    if ((mInputs[0] == LOGIC_HIGH && mInputs[1] == LOGIC_LOW) || 
        (mInputs[0] == LOGIC_LOW && mInputs[1] == LOGIC_HIGH)) 
    {
        mOutputValue = LOGIC_HIGH;
    } 
    else 
    {
        mOutputValue = LOGIC_LOW;
    }

    if (mpOutputConnection != nullptr) 
    {
        mpOutputConnection->DriveLevel(mOutputValue);  // Drive the wire connected to the XOR gate's output
    }
}

// CNotGates class implementation
CNotGates::CNotGates() 
{
    mInputs[0] = LOGIC_UNDEFINED; // NOT gate only has one input
}

void CNotGates::DriveInput(int aInputIndex, eLogicLevel aNewLevel) 
{
    if (aInputIndex == 0) 
    {
        mInputs[0] = aNewLevel;   // Set the input level
        ComputeOutput();          // Recompute the output based on the new input
    }
}

eLogicLevel CNotGates::GetOutputState() const 
{
    return mOutputValue;  // Return the current output state
}

void CNotGates::ConnectOutput(CWires* apOutputConnection) 
{
    mpOutputConnection = apOutputConnection;  // Store the wire pointer
}

void CNotGates::ComputeOutput() 
{
    // NOT logic: output is the opposite of the input
    mOutputValue = (mInputs[0] == LOGIC_HIGH) ? LOGIC_LOW : LOGIC_HIGH;

    if (mpOutputConnection != nullptr) 
    {
        mpOutputConnection->DriveLevel(mOutputValue);  // Drive the wire connected to the NOT gate's output
    }
}

// Circuit class
class Circuit {
public:
    Circuit();                                              // Constructor to initialize the circuit
    void TestInputStates(const std::string& gateType);      // Function to test input states for different gate types

private:
    static const int NumAndGates = 4;                   // Number of AND gates in the circuit
    static const int NumOrgates = 4;                    // Number of OR gates in the circuit
    static const int NumXorgates = 4;                   // Number of XOR gates in the circuit
    static const int NumNotGates = 4;                   // Number of NOT gates in the circuit
    static const int NumWires = 100;                    // Number of wires in the circuit

    CAndGates ANDGates[NumAndGates];                                // Array of AND gates
    CORGates ORGates[NumOrgates];                                   // Array of OR gates
    CXORGates XORGates[NumXorgates];                                // Array of XOR gates
    CNotGates NOTGates[NumNotGates];                                // Array of NOT gates
    CWires WireConnections[NumWires];                               // Array of wires used in the circuit
};

// Circuit class implementation
Circuit::Circuit() 
{
    for (int i = 0; i < NumAndGates; ++i) 
    {
        ANDGates[i] = CAndGates();          // Initializes the AND gate array
    }

    for (int i = 0; i < NumOrgates; ++i) 
    {
        ORGates[i] = CORGates();            // Initializes the OR gate array
    }

    for (int i = 0; i < NumXorgates; ++i) 
    {
        XORGates[i] = CXORGates();          // Initializes the XOR gate array
    }

    for (int i = 0; i < NumNotGates; ++i) 
    {
        NOTGates[i] = CNotGates();          // Initializes the NOT gate array
    }

    for (int i = 0; i < NumWires; ++i) 
    {
        WireConnections[i] = CWires();      // Initializes the wire array
    }

    // Example connections for AND Gate 0
    WireConnections[0].AddOutputConnection(&ANDGates[0], 0);        // Connect wire 0 to input 0 of AND gate 0
    WireConnections[1].AddOutputConnection(&ANDGates[0], 1);        // Connect wire 1 to input 1 of AND gate 0
    ANDGates[0].ConnectOutput(nullptr);                             // Connect the output of AND gate 0

    // Example connections for OR Gate 0
    WireConnections[0].AddOutputConnection(&ORGates[0], 0);         // Connect wire 0 to input 0 of OR gate 0
    WireConnections[1].AddOutputConnection(&ORGates[0], 1);         // Connect wire 1 to input 1 of OR gate 0
    ORGates[0].ConnectOutput(nullptr);                              // Connect the output of OR gate 0

    // Example connections for XOR Gate 0
    WireConnections[0].AddOutputConnection(&XORGates[0], 0);        // Connect wire 0 to input 0 of XOR gate 0
    WireConnections[1].AddOutputConnection(&XORGates[0], 1);        // Connect wire 1 to input 1 of XOR gate 0
    XORGates[0].ConnectOutput(nullptr);                             // Connect the output of XOR gate 0

    // Example connections for NOT Gate 0
    WireConnections[0].AddOutputConnection(&NOTGates[0], 0);        // Connect wire 0 to input of NOT gate 0
    NOTGates[0].ConnectOutput(nullptr);                             // Connect the output of NOT gate 0
}

void Circuit::TestInputStates(const std::string& gateType) 
{
    std::cout << gateType << std::endl;

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
            WireConnections[0].DriveLevel(inputs[i][0]);
            WireConnections[1].DriveLevel(inputs[i][1]);
            std::cout << "Input: A = " << (inputs[i][0] == LOGIC_HIGH ? 1 : 0)
                      << ", B = " << (inputs[i][1] == LOGIC_HIGH ? 1 : 0)
                      << "; Output: " << (ANDGates[0].GetOutputState() == LOGIC_HIGH ? 1 : 0) << std::endl;
        }
        std::cout << std::endl;
    } 
    
    else if (gateType == "OR GATE") 
    {
        eLogicLevel inputs[4][2] = {
            {LOGIC_LOW, LOGIC_LOW},  // A = 0, B = 0
            {LOGIC_LOW, LOGIC_HIGH}, // A = 0, B = 1
            {LOGIC_HIGH, LOGIC_LOW}, // A = 1, B = 0
            {LOGIC_HIGH, LOGIC_HIGH} // A = 1, B = 1
        };

        for (int i = 0; i < 4; ++i) 
        {
            WireConnections[0].DriveLevel(inputs[i][0]);
            WireConnections[1].DriveLevel(inputs[i][1]);
            std::cout << "Input: A = " << (inputs[i][0] == LOGIC_HIGH ? 1 : 0)
                      << ", B = " << (inputs[i][1] == LOGIC_HIGH ? 1 : 0)
                      << "; Output: " << (ORGates[0].GetOutputState() == LOGIC_HIGH ? 1 : 0) << std::endl;
        }
        std::cout << std::endl;
    } 

    else if (gateType == "XOR GATE") 
    {
        eLogicLevel inputs[4][2] = {
            {LOGIC_LOW, LOGIC_LOW},  // A = 0, B = 0
            {LOGIC_LOW, LOGIC_HIGH}, // A = 0, B = 1
            {LOGIC_HIGH, LOGIC_LOW}, // A = 1, B = 0
            {LOGIC_HIGH, LOGIC_HIGH} // A = 1, B = 1
        };

        for (int i = 0; i < 4; ++i) 
        {
            WireConnections[0].DriveLevel(inputs[i][0]);
            WireConnections[1].DriveLevel(inputs[i][1]);
            std::cout << "Input: A = " << (inputs[i][0] == LOGIC_HIGH ? 1 : 0)
                      << ", B = " << (inputs[i][1] == LOGIC_HIGH ? 1 : 0)
                      << "; Output: " << (XORGates[0].GetOutputState() == LOGIC_HIGH ? 1 : 0) << std::endl;
        }
        std::cout << std::endl;
    } 

    else if (gateType == "NOT GATE") 
    {
        eLogicLevel inputs[2] = {
            LOGIC_LOW,  // A = 0
            LOGIC_HIGH  // A = 1
        };

        for (int i = 0; i < 2; ++i) 
        {
            WireConnections[0].DriveLevel(inputs[i]);
            std::cout << "Input: A = " << (inputs[i] == LOGIC_HIGH ? 1 : 0)
                      << "; Output: " << (NOTGates[0].GetOutputState() == LOGIC_HIGH ? 1 : 0) << std::endl;
        }
        std::cout << std::endl;
    }
}

int main() 
{
    Circuit myCircuits;                                      // Create an instance of the Circuit class
    myCircuits.TestInputStates("AND GATE");                  // Test the AND gate
    myCircuits.TestInputStates("OR GATE");                   // Test the OR gate
    myCircuits.TestInputStates("XOR GATE");                  // Test the XOR gate
    myCircuits.TestInputStates("NOT GATE");                  // Test the NOT gate
    
    return 0;
}
