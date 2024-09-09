#ifndef FILE_READER_H
#define FILE_READER_H

#include "Circuit.h"

class CFileReader {
public:
    CFileReader(Circuit& circuit);
    void ProcessInput();

private:
    Circuit& circuit;
};

#endif  // FILE_READER_H
