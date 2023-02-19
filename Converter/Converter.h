#include "MyVector.h"
#include <fstream>
#include <string>

class Converter {
    char* file_name;
    int length;
    unsigned char flag;
    std::ifstream file;

    int find;

public:
    MyVector frames_storage;

public:
    Converter(char *filename);

    void FindID3();

    void GetLength(unsigned char length_bits[4]);

    void Parsing();

    void Output(char *filename);
};
