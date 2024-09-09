 // SID: 520534445
 // Lab 3: Refactoring and Design 

#include "CFileReader.h"

int main() {
    Circuit myCircuit;                      // Create an instance of Circuit to manage gates
    CFileReader fileReader(myCircuit);      // Initialize file reader with the circuit instance
    fileReader.ProcessInput();              // Process input commands for building and simulating the circuit

    return 0;
}
