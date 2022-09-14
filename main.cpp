#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

long long lines(std::ifstream &file) {
    return std::count(std::istreambuf_iterator<char>(file),
                      std::istreambuf_iterator<char>(), '\n') + 1;
}

size_t bytes(std::ifstream &file) {
    return file.tellg();
}

int words(std::ifstream &file, long long countlines) {
    std::string str;
    std::string fileinstring = " ";
    file.seekg(0);
    for (int i = 0; i < countlines; i++) {
        std::getline(file, str);
        fileinstring += (" " + str);
    }
    int count_words = 0;
    for (int i = 0; i < fileinstring.size() - 1; i++) {
        if (fileinstring[i] == ' ' && fileinstring[i + 1] != ' ')
            count_words++;
    }
    return count_words;
}

int main(int argc, char **argv) {
    std::vector<std::string> files;
    std::vector<std::string> options;
    std::vector<int> lwc(3); // checker options: lines, bytes, words
    for (int i = 1; i < argc; i++) {
        std::string str = argv[i];
        if (str[str.size() - 1] == 't' && str[str.size() - 2] == 'x' &&
            str[str.size() - 3] == 't') //check file with .txt or not
            files.push_back(argv[i]);
        else
            options.push_back(argv[i]);
    } //search files and options
    if (options.empty()) {
        lwc[0] = 1;
        lwc[1] = 1;
        lwc[2] = 1;
    }
    for (int i = 0; i < options.size(); i++) { //options
        if (options[i][0] == options[i][1] && options[i][0] == '-') {
            if (options[i] == "--lines")
                lwc[0] = 1;
            else if (options[i] == "--words")
                lwc[1] = 1;
            else if (options[i] == "--bytes")
                lwc[2] = 1;
            else {
                std::cout << "Error option";
                exit(1);
            }
        } else {
            for (int j = 1; j < options[i].size(); j++) {
                if (options[i][j] == 'l')
                    lwc[0] = 1;
                else if (options[i][j] == 'w')
                    lwc[1] = 1;
                else if (options[i][j] == 'c')
                    lwc[2] = 1;
                else {
                    std::cout << "Error option";
                    exit(1);
                }
            }
        }
    }
    for (int i = 0; i < files.size(); i++) { // files
        std::ifstream file(files[i]);
        if (file.fail())
            std::cout << files[i] << " isn't present";
        else {
            long long countlines = lines(file);
            size_t byte = bytes(file);
            if (lwc[0] == 1) // lwc[0] - lines
                std::cout << countlines << " ";
            if (lwc[1] == 1) // lwc[1] - words
                std::cout << words(file, countlines) << " ";
            if (lwc[2] == 1) // lwc[2] - bytes
                std::cout << byte << " ";
            //if lines/words/bytes need, lwc[i] = 1
            std::cout << files[i] << "\n";
        }
    }
}