#ifndef CFILE_READER_H
#define CFILE_READER_H

#include "Circuit.h"

// File reader Class that reads the text file from the terminal
class CFileReader {
public:
    CFileReader(Circuit& circuit);
    void ProcessInput();

private:
    Circuit& circuit;
};

#endif 
