#include <iostream>
#include <fstream>
#include <string>

void WriteInFile(std::ofstream& fileout, uint64_t memory) {
    uint64_t mem = memory;
    while (mem > 0) {
        fileout << (char) (mem % 256);
        mem /= 256;
    }
    if (memory >= 65536 && memory < 16777215)
        fileout << (char) 0;
    else if (memory >= 256 && memory < 65535)
        fileout << (char) 0 << (char) 0;
    else if (memory < 256)
        fileout << (char) 0 << (char) 0 << (char) 0;
}

uint64_t pow(uint64_t a, uint64_t b) {
    uint64_t result = 1;
    for (int i = 0; i < b; i++)
        result *= (uint64_t) a;
    return result;
}

uint64_t number(std::string str) {
    uint64_t result = 0;
    for (int i = str.size() - 1; i >= 0; i--) {
        result += (uint64_t) (str[i] - '0') * pow(10, str.size() - i - 1);
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
    std::ifstream filein(input, std::fstream::in);
    std::ofstream fileout;
    if (filein.fail()) {
        std::cout << input << " isn't present" << "\n";
    } else if (output.empty())
        std::cout << "output storage isn't present" << "\n";
    else {
        if (length != 0 && width != 0 && maxiter != -1 && freq != -1) {
            uint64_t **array = new uint64_t *[width];
            for (int i = 0; i < width; i++) {
                array[i] = new uint64_t[length];
            }
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < length; j++)
                    array[i][j] = 0;
            }
            uint64_t **workarray = new uint64_t *[width];
            for (int i = 0; i < width; i++) {
                workarray[i] = new uint64_t[length];
            }
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < length; j++)
                    workarray[i][j] = 0;
            }
            std::string filename;
            int maxi = 1;
            int fre = 1;
            int x;
            int y;
            char digit;
            std::string ss = "";
            while (!filein.eof()) {//model.exe -l 2 -w 2 -m 2 -f 1 -i book3.tsv -o ./
                digit = filein.get();
                while (digit != '\t' && digit != '\n') {
                    ss += digit;
                    digit = filein.get();
                }
                x = number(ss);
                ss = "";
                digit = filein.get();
                while (digit != '\t' && digit != '\n') {
                    ss += digit;
                    digit = filein.get();
                }
                y = number(ss);
                ss = "";
                digit = filein.get();
                while (digit != '\t' && digit != '\n' && !filein.eof()) {
                    ss += digit;
                    digit = filein.get();
                }
                array[y - 1][x - 1] = number(ss);
                workarray[y - 1][x - 1] = array[y - 1][x - 1];
                ss = "";
            }
            while (maxiter > 0) {
                filename = output;
                filename += "\\picture";
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
                    uint64_t memory = 54 + 256 * 4 + (uint64_t) width * (uint64_t) length;
                    filename += std::to_string(maxi);
                    maxi++;
                    filename += ".bmp";
                    fileout.open(filename);
                    fileout << "BM";
                    WriteInFile(fileout, memory);
                    fileout << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 54 << (char) 4
                            << (char) 0 << (char) 0
                            << (char) 40 << (char) 0 << (char) 0 << (char) 0;
                    WriteInFile(fileout, length);
                    WriteInFile(fileout, width);
                    fileout << (char) 1 << (char) 0 << (char) 8 << (char) 0 << (char) 0 << (char) 0 << (char) 0
                            << (char) 0;
                    WriteInFile(fileout, (uint64_t) length * (uint64_t) width);
                    fileout << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0
                            << (char) 0;
                    fileout << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0
                            << (char) 0;
                    fileout << (char) 255 << (char) 255 << (char) 255 << (char) 0;
                    fileout << (char) 0 << (char) 128 << (char) 0 << (char) 0;
                    fileout << (char) 255 << (char) 0 << (char) 139 << (char) 0;
                    fileout << (char) 0 << (char) 255 << (char) 255 << (char) 0;
                    for(int i = 0; i < 252; i++) {
                        fileout << (char) 0 << (char) 0 << (char) 0 << (char) 0;
                    }
                    uint64_t rowsize = length / 4;
                    if(length % 4 != 0)
                        rowsize++;
                    rowsize*=4;
                    for (int i = 0; i < width; i++) {
                        for (int j = 0; j < length; j++) {
                            if (array[i][j] == 0)
                                fileout << (char) 0;
                            else if (array[i][j] == 1)
                                fileout << (char) 1;
                            else if (array[i][j] == 2)
                                fileout << (char) 2;
                            else if (array[i][j] == 3)
                                fileout << (char) 3;
                            else if (array[i][j] > 3)
                                fileout << (char) 4;
                        }
                        for(int j = length; j < rowsize; j++)
                            fileout << (char) 0;
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
    }
    return 0;
}