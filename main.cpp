#include <iostream>
#include <string>
#include <fstream>

void record(std::ofstream &fileout, long long memory) {
    long long mem = memory;
    while (mem > 0) {
        fileout << (char) (mem % 256);
        mem /= 256;
    }
    if (memory >= 65536 && memory < 16777215)
        fileout << (char) 0;
    else if (memory >= 256 && memory < 65535) {
        fileout << (char) 0 << (char) 0;
    } else if (memory < 256) {
        fileout << (char) 0 << (char) 0 << (char) 0;
    }
}

long long pow(int uint, int n) {
    long long result = uint;
    for (int i = 0; i < n; i++) {
        result *= 10;
    }
    return result;
}

long long upheaval(long long uint) {
    long long result = 0;
    int k = 0;
    while (uint > 0) {
        result += pow((uint % 10), k);
    }
    return result;
}

int main(int argc, char **argv) {
    uint16_t length = 0;
    uint16_t width = 0;
    std::string input;
    std::string output;
    int maxiter = -1;
    int freq = -1;
    for (int i = 1; i < argc; i += 2) {
        std::string argv1 = argv[i];
        std::string argv2 = argv[i + 1];
        if (argv1 == "-l" || argv1 == "--length")
            length = stoi(argv2);
        else if (argv1 == "-w" || argv1 == "--width")
            width = stoi(argv2);
        else if (argv1 == "-i" || argv1 == "--input")
            input = argv[i + 1];
        else if (argv1 == "-o" || argv1 == "-output")
            output = argv[i + 1];
        else if (argv1 == "-m" || argv1 == "--max-iter")
            maxiter = stoi(argv2);
        else if (argv1 == "-f" || argv1 == "--freq")
            freq = stoi(argv2);
    }
    std::fstream filein;
    std::ofstream fileout;
    filein.open(input);
    if (filein.fail()) {
        std::cout << input << " isn't present" << "\n";
    }
    if (length != 0 && width != 0 && maxiter != -1 && freq != -1) {
        long long array[width][length];
        std::string filename;
        int maxi = 1;
        int fre = 1;
        long long workarray[width][length];
        char digit;
        uint16_t workwidth = 0;
        uint16_t worklength = 0;
        std::string number = "";
        while (workwidth < width) {
            while (worklength < length) {
                digit = filein.get();
                while (digit != '\t' && digit != '\n') {
                    number += digit;
                    digit = filein.get();
                }
                array[workwidth][worklength] = stoi(number);
                workarray[workwidth][worklength] = array[workwidth][worklength];
                worklength++;
                number = "";
            }
            worklength = 0;
            workwidth++;
        }
        while (maxiter > 0) {
            filename = output;
            filename += "picture";
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < length; j++) {
                    if (array[i][j] > 3) {
                        workarray[i][j] -= 4;
                        if (i + 1 < width)
                            workarray[i + 1][j]++;
                        if (i - 1 >= 0)
                            workarray[i - 1][j]++;
                        if (j + 1 < length)
                            workarray[i][j + 1]++;
                        if (j - 1 >= 0)
                            workarray[i][j - 1]++;
                    }
                }
            }
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < length; j++) {
                    array[i][j] = workarray[i][j];
                }
            }
            if ((freq == 0 && maxiter == 1) || (freq != 0 && fre % freq == 0)) {
                long long memory = 54 + 4 * width * length;
                filename += std::to_string(maxi);
                maxi++;
                filename += ".bmp";
                fileout.open(filename);
                fileout << "BM";
                record(fileout, memory);
                fileout << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 54 << (char) 0 << (char) 0 << (char) 0
                        << (char) 40 << (char) 0 << (char) 0 << (char) 0;
                record(fileout, length);
                record(fileout, width);
                fileout << (char) 1 << (char) 0 << (char) 32 << (char) 0 << (char) 0 << (char) 0 << (char) 0
                        << (char) 0;
                record(fileout, length * width);
                fileout << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0;
                fileout << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0;
                for (int i = 0; i < width; i++) {
                    for (int j = 0; j < length; j++) {
                        if (array[i][j] == 0)
                            fileout << (char) 255 << (char) 255 << (char) 255 << (char) 0;
                        else if (array[i][j] == 1)
                            fileout << (char) 0 << (char) 128 << (char) 0 << (char) 0;
                        else if (array[i][j] == 2)
                            fileout << (char) 255 << (char) 0 << (char) 139 << (char) 0;
                        else if (array[i][j] == 3)
                            fileout << (char) 0 << (char) 255 << (char) 255 << (char) 0;
                        else if (array[i][j] > 3)
                            fileout << (char) 0 << (char) 0 << (char) 0 << (char) 0;
                    }
                }
                fileout.close();
            }
            maxiter--;
            fre++;
        }
    } else {
        std::cout << "Error parameters";
        exit(1);
    }
    return 0;
}