#include "Converter/Converter.h"
#include <fstream>
#include <iostream>

int main(int argc, char **argv) {
    Converter converter(argv[argc - 1]);
    converter.FindID3();
    converter.Parsing();
}