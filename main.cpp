#include "Converter/Converter.h"
#include <fstream>
#include <iostream>

int main(int argc, char **argv) {
    Converter converter("tag.mp3");
    converter.FindID3();
    converter.Parsing();
    for(int i = 0; i < converter.frames_storage.storage[0].length; i++)
        std::cout << converter.frames_storage.storage[0].info[i];
}