#include "FileReader.h"

int main() {
    Circuit myCircuit;
    CFileReader fileReader(myCircuit);
    fileReader.ProcessInput();

    return 0;
}
