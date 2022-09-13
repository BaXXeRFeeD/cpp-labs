#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int lines(std::ifstream &file) {
    return std::count(std::istreambuf_iterator<char>(file),
                      std::istreambuf_iterator<char>(), '\n') + 1;
}

int bytes(std::ifstream &file) {
    return file.tellg();
}

int words(std::string file_name, int count_lines) {
    std::string str;
    std::string File = "";
    std::ifstream file(file_name);
    for (int i = 0; i < count_lines; i++) {
        std::getline(file, str);
        File = File + " " + str;
    }
    int count_words = 0;
    for (int i = 0; i < File.size() - 1; i++) {
        if (File[i] == ' ' && File[i + 1] != ' ')
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
            str[str.size() - 3] == 't')
            files.push_back(argv[i]);
        else
            options.push_back(argv[i]);
    } //search files and options
    if (options.empty()) {
        lwc[0] = 1;
        lwc[1] = 1;
        lwc[2] = 1;
    }
    for (int i = 0; i < options.size(); i++) {
        if (options[i][0] == options[i][1] && options[i][0] == '-') {
            if (options[i] == "--lines")
                lwc[0] = 1;
            else {
                if (options[i] == "--words")
                    lwc[1] = 1;
                else {
                    if (options[i] == "--bytes")
                        lwc[2] = 1;
                    else {
                        std::cout << "Error option";
                        exit(1);
                    }
                }
            }
        }
        for (int j = 1; j < options[i].size(); j++) {
            if (options[i][j] == 'l')
                lwc[0] = 1;
            else {
                if (options[i][j] == 'w')
                    lwc[1] = 1;
                else {
                    if (options[i][j] == 'c')
                        lwc[2] = 1;
                    else {
                        std::cout << "Error option";
                        exit(1);
                    }
                }
            }
        }
    }
    for (int i = 0; i < files.size(); i++) {
        std::ifstream file(files[i]);
        if (file.fail())
            std::cout << files[i] << " isn't present";
        else {
            int count_lines = -1;
            if (lwc[0] == 1 || lwc[1] == 1)
                count_lines = lines(file);
            if (lwc[0] == 1)
                std::cout << count_lines << " ";
            if (lwc[2] == 1)
                std::cout << bytes(file) << " ";
            if (lwc[1] == 1)
                std::cout << words(files[i], count_lines) << " ";
            std::cout << files[i] << "\n";
        }
    }
    system("pause");
}