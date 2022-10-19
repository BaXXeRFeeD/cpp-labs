#include <iostream>
#include <fstream>
#include <string>

void WriteNumberInFile(std::ofstream& fileout, uint64_t kMemory) {
    uint64_t mem = kMemory;
    while (mem > 0) {
        fileout << (char) (mem % 256);
        mem /= 256;
    }
    if (kMemory >= 65536 && kMemory < 16777215)
        fileout << (char) 0;
    else if (kMemory >= 256 && kMemory < 65535)
        fileout << (char) 0 << (char) 0;
    else if (kMemory < 256)
        fileout << (char) 0 << (char) 0 << (char) 0;
}

void WriteFileHead(std::ofstream& fileout, uint64_t kMemory) {
    fileout << "BM";
    WriteNumberInFile(fileout, kMemory);
    fileout << (char) 0 << (char) 0 << (char) 0 << (char) 0
            << (char) 54 << (char) 4 << (char) 0 << (char) 0;
}

void WriteFileInfo(std::ofstream& fileout, uint16_t kLength, uint16_t kWidth) {
    fileout << (char) 40 << (char) 0 << (char) 0 << (char) 0;
    WriteNumberInFile(fileout, kLength);
    WriteNumberInFile(fileout, kWidth);
    fileout << (char) 1 << (char) 0 << (char) 8 << (char) 0 << (char) 0 << (char) 0 << (char) 0
            << (char) 0;
    WriteNumberInFile(fileout, (uint64_t) kLength * (uint64_t) kWidth);
    fileout << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0
            << (char) 0;
    fileout << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0 << (char) 0
            << (char) 0;
}

void WriteColorTable(std::ofstream& fileout) {
    fileout << (char) 255 << (char) 255 << (char) 255 << (char) 0;
    fileout << (char) 0 << (char) 128 << (char) 0 << (char) 0;
    fileout << (char) 255 << (char) 0 << (char) 139 << (char) 0;
    fileout << (char) 0 << (char) 255 << (char) 255 << (char) 0;
    for (int i = 0; i < 252; i++) {
        fileout << (char) 0 << (char) 0 << (char) 0 << (char) 0;
    }
}

uint64_t pow(uint64_t a, uint64_t b) {
    uint64_t result = 1;
    for (int i = 0; i < b; i++)
        result *= (uint64_t) a;
    return result;
}

uint64_t toNumber(std::string str) {
    uint64_t result = 0;
    for (int i = str.size() - 1; i >= 0; i--) {
        result += (uint64_t) (str[i] - '0') * pow(10, str.size() - i - 1);
    }
    return result;
}

int main(int argc, char **argv) {
    uint16_t kLength = 0;
    uint16_t kWidth = 0;
    std::string kInput;
    std::string kOutput;
    int maxiter = 0;
    int freq = 0;
    for (int i = 1; i < argc; i += 2) {
        std::string argv1 = argv[i];
        std::string argv2 = argv[i + 1];
        if (argv1 == "-l" || argv1 == "--kLength")
            kLength = stoi(argv2);
        else if (argv1 == "-w" || argv1 == "--kWidth")
            kWidth = stoi(argv2);
        else if (argv1 == "-i" || argv1 == "--kInput")
            kInput = argv[i + 1];
        else if (argv1 == "-o" || argv1 == "-kOutput")
            kOutput = argv[i + 1];
        else if (argv1 == "-m" || argv1 == "--max-iter")
            maxiter = stoi(argv2);
        else if (argv1 == "-f" || argv1 == "--freq")
            freq = stoi(argv2);
    }
    uint64_t kRowsize = kLength / 4;
    if (kLength % 4 != 0)
        kRowsize++;
    kRowsize *= 4;
    std::ifstream kFilein(kInput, std::fstream::in);
    std::ofstream fileout;
    if (kFilein.fail()) {
        std::cout << kInput << " isn't present" << "\n";
    } else if (kOutput.empty())
        std::cout << "kOutput storage isn't present" << "\n";
    else {
        if (kLength != 0 && kWidth != 0) {
            uint64_t **array = new uint64_t *[kWidth];
            for (int i = 0; i < kWidth; i++) {
                array[i] = new uint64_t[kLength];
            }
            for (int i = 0; i < kWidth; i++) {
                for (int j = 0; j < kLength; j++)
                    array[i][j] = 0;
            }
            uint64_t **workarray = new uint64_t *[kWidth];
            for (int i = 0; i < kWidth; i++) {
                workarray[i] = new uint64_t[kLength];
            }
            for (int i = 0; i < kWidth; i++) {
                for (int j = 0; j < kLength; j++)
                    workarray[i][j] = 0;
            }
            std::string filename;
            int filenumber = 1;
            int freqnumber = 1;
            uint16_t x;
            uint16_t y;
            char digit;
            std::string number = "";
            while (!kFilein.eof()) {
                digit = kFilein.get();
                while (digit != '\t' && digit != '\n') {
                    number += digit;
                    digit = kFilein.get();
                }
                x = toNumber(number);
                number = "";
                digit = kFilein.get();
                while (digit != '\t' && digit != '\n') {
                    number += digit;
                    digit = kFilein.get();
                }
                y = toNumber(number);
                number = "";
                digit = kFilein.get();
                while (digit != '\t' && digit != '\n' && !kFilein.eof()) {
                    number += digit;
                    digit = kFilein.get();
                }
                array[y - 1][x - 1] = toNumber(number);
                workarray[y - 1][x - 1] = array[y - 1][x - 1];
                number = "";

            }
            bool reset = false;
            bool iterzero = false;
            if (maxiter == 0) {
                reset = true;
                iterzero = true;
            }
            while (maxiter > 0 || reset) {
                filename = kOutput;
                filename += "\\picture";
                filename += std::to_string(filenumber);
                filename += ".bmp";
                reset = false;
                for (int i = 0; i < kWidth; i++) {
                    for (int j = 0; j < kLength; j++) {
                        if (array[i][j] > 3) {
                            workarray[i][j] -= 4;
                            reset = true;
                            if (i + 1 < kWidth)
                                workarray[i + 1][j]++;
                            if (i - 1 >= 0)
                                workarray[i - 1][j]++;
                            if (j + 1 < kLength)
                                workarray[i][j + 1]++;
                            if (j - 1 >= 0)
                                workarray[i][j - 1]++;
                        }
                    }
                }
                for (int i = 0; i < kWidth; i++) {
                    for (int j = 0; j < kLength; j++) {
                        array[i][j] = workarray[i][j];
                    }
                }
                if ((((freq == 0 && maxiter == 1) || (freq != 0 && freqnumber % freq == 0)) && !iterzero) ||
                    (iterzero && !reset)) {
                    uint64_t memory = 54 + 256 * 4 + (uint64_t) kWidth * (uint64_t) kLength;
                    fileout.open(filename);
                    WriteFileHead(fileout, memory);
                    WriteFileInfo(fileout, kLength, kWidth);
                    WriteColorTable(fileout);
                    for (int i = 0; i < kWidth; i++) {
                        for (int j = 0; j < kLength; j++) {
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
                        for (int j = kLength; j < kRowsize; j++)
                            fileout << (char) 0;
                    }
                    fileout.close();
                    filenumber++;
                }
                maxiter--;
                freqnumber++;
            }
        } else {
            std::cout << "Error parameters";
            exit(1);
        }
    }
    return 0;
}