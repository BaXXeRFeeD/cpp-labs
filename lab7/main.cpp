#include "Converter/Converter.h"
#include <fstream>
#include <iostream>

int main(int argc, char **argv) {
    char* string = "";
    for (int i = 1; i < argc; i++) {
        if (argv[i][1] == 'i')
            string = argv[i + 1];
    }
    Converter converter(string);
    converter.FindID3();
    converter.Parsing();
    for(int i = 1; i < argc; i++){
        if (argv[i][1] == 'o') {
            converter.Output(argv[i + 1]);
        }
    }
}