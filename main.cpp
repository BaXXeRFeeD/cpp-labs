#include <iostream>
#include <vector>
#include <fstream>
#include <bitset>
#include <filesystem>
#include <cmath>

struct Archive {
    int word = 0;
    int bytes;

    bool create = false;
    std::vector<std::string> files;
    std::string archive_name;
    bool list = false;
    bool extract = false;
    std::string append = "";
    std::string del = "";
    bool concatenate = false;
};

void ControlBytes(int word, Archive &app) {
    int i = 2;
    while (true) {
        if (i >= log2(i + word + 1)) {
            app.bytes = i;
            break;
        }
        i++;
    }
}

void Parsing(int argc, char **argv, Archive &app) {
    std::string arg;
    for (int i = 1; i < argc; i++) {
        arg = argv[i];
        if (arg == "-c" || arg == "--create") {
            app.create = true;
        } else if (arg == "-l" || arg == "--list") {
            app.list = true;
        } else if (arg == "-x" || arg == "--extract") {
            app.extract = true;
        } else if (arg == "-a" || arg == "--append") {
            app.append = argv[i + 1];
            i++;
        } else if (arg == "-d" || arg == "--delete") {
            app.del = argv[i + 1];
            i++;
        } else if (arg == "-A" || arg == "--concatenate") {
            app.concatenate = true;
        } else if (arg == "-f") {
            app.archive_name = argv[i + 1];
            app.archive_name += ".hrc";
            i++;
        } else if (arg == "-w" || arg == "--word") {
            arg = argv[i + 1];
            app.word = stoi(arg);
            i++;
            ControlBytes(app.word, app);
        } else if (arg.substr(0, 7) == "--file=")
            app.archive_name = arg.substr(7, arg.length() - 6) + ".hrc";
        else
            app.files.push_back(arg);
    }
}

void List(Archive &app) {
    std::fstream archive(app.archive_name);
    std::string str;
    std::getline(archive, str);
}

void Addfile(std::vector<std::string> &files) {

    std::bitset<32> bts1;
}

int Filesize(std::string filename) {
    int count = 0;
    return count;
}

namespace fs = std::filesystem;

std::string Filenaming(std::string filename) {
    fs::path file{filename};
    std::string a{file.string()};
    return a;
}

void Headerwrite(Archive app, std::ofstream &archive) {
    std::string str = std::to_string(app.word) + " " + std::to_string(app.files.size());
    for (int i = 0; i < app.files.size(); i++) {
        std::ifstream file(app.files[i], std::ios::ate);
        str += " " + Filenaming(app.files[i]) + " " + std::to_string(file.tellg());
    }
    str = std::to_string(3 * str.size()) + " " + str;
    std::bitset<8> bts;
    std::vector<bool> header(str.size() * 8 * 3);
    int z = 0;
    for (int i = 0; i < str.size(); i++) {
        unsigned char c = str[i];
        for (int j = 7; j >= 0; j--) {
            if (c % 2 == 0)
                bts[j] = 0;
            else
                bts[j] = 1;
            c /= 2;
        }
        for (int j = 0; j < 8; j++) {
            if (bts[j]) {
                header[z] = 0;
                z++;
                header[z] = 0;
                z++;
                header[z] = 1;
                z++;
            } else {
                header[z] = 1;
                z++;
                header[z] = 1;
                z++;
                header[z] = 0;
                z++;
            }
        }
    }
    for (int i = 0; i < header.size(); i += 8) {
        unsigned char c = 0;
        for (int j = i; j < i + 8; j++) {
            c *= 2;
            c += header[j];
        }
        archive << c;
    }
    for (int i = 0; i < 24; i += 8) {
        unsigned char c = 0;
        for (int j = i; j < i + 8; j++) {
            c *= 2;
            c += header[j];
        }
    }
}

//void Fileheader(std::string filename, std::ofstream &archive, Archive app) {
//    std::ifstream file(filename, std::ios::ate);
//    std::string str = Filenaming(filename);
//    str = std::to_string(file.tellg()) + " ";
//    std::vector<bool> output;
//    std::vector<bool> container;
//    std::vector<bool> cur_ch;
//    for (int i = 0; i < str.size(); i++) {
//        char c = str[i];
//        for (int j = 7; j >= 0; j--) {
//            cur_ch.push_back(c % 2);
//            c /= 2;
//        }
//        while (!cur_ch.empty()) {
//            while (container.size() != app.word && !cur_ch.empty()) {
//                container.push_back(cur_ch.back());
//                cur_ch.pop_back();
//            }
//            if (container.size() == app.word) {
//                for (int z = 1; z <= pow(2, app.bytes - 1); z *= 2) {
//                    container.insert(container.begin() + z - 1, 0);
//                }
//                for (int z = 1; z <= pow(2, app.bytes - 1); z *= 2) {
//                    int count = 0;
//                    for (int j = z - 1; j < app.word + app.bytes; j += 2 * z) {
//                        for (int t = j; t < j + z && t < app.word + app.bytes; t++)
//                            count += container[t];
//                    }
//                    if (count % 2 != 0)
//                        container[z - 1] = 1;
//                }
//                for (int i = 0; i < container.size(); i++) {
//                    output.push_back(container.front());
//                    container.erase(container.begin());
//                }
//                if (!cur_ch.empty()) {
//                    while (container.size() != app.word && !cur_ch.empty()) {
//                        container.push_back(cur_ch.back());
//                        cur_ch.pop_back();
//                    }
//                }
//            }
//        }
//        while (output.size() >= 8) {
//            unsigned char ch = 0;
//            for (int j = 0; j < 8; j++) {
//                ch *= 2;
//                ch += output.front();
//                output.erase(output.begin());
//            }
//            archive << ch;
//        }
//    }
//    if (!output.empty()) {
//        while (output.size() != 8)
//            output.push_back(0);
//        unsigned char ch = 0;
//        for (int j = 0; j < 8; j++) {
//            ch *= 2;
//            ch += output.front();
//            output.erase(output.begin());
//        }
//        archive << ch;
//    }
//    file.close();
//}

void Filewrite(std::string filename, std::ofstream &archive, Archive app) {
    std::ifstream file(filename);
    std::vector<bool> output;
    std::vector<bool> container;
    std::vector<bool> cur_ch;
    char c = file.get();
    while (!file.eof()) {
        for (int j = 7; j >= 0; j--) {
            cur_ch.push_back(c % 2);
            c /= 2;
        }
        while (!cur_ch.empty()) {
            while (container.size() != app.word && !cur_ch.empty()) {
                container.push_back(cur_ch.back());
                cur_ch.pop_back();
            }
            if (container.size() == app.word) {
                for (int z = 1; z <= pow(2, app.bytes - 1); z *= 2) {
                    container.insert(container.begin() + z - 1, 0);
                }
                for (int z = 1; z <= pow(2, app.bytes - 1); z *= 2) {
                    int count = 0;
                    for (int j = z - 1; j < app.word + app.bytes; j += 2 * z) {
                        for (int t = j; t < j + z && t < app.word + app.bytes; t++)
                            count += container[t];
                    }
                    if (count % 2 != 0)
                        container[z - 1] = 1;
                }
                for (int i = 0; i < app.word + app.bytes; i++) {
                    output.push_back(container.front());
                    container.erase(container.begin());
                }
//                if (!cur_ch.empty()) {
//                    while (container.size() != app.word && !cur_ch.empty()) {
//                        container.push_back(cur_ch.back());
//                        cur_ch.pop_back();
//                    }
//                }
            }
        }
        while (output.size() >= 8) {
            unsigned char ch = 0;
            for (int j = 0; j < 8; j++) {
                ch *= 2;
                ch += output.front();
                output.erase(output.begin());
            }
            archive << ch;
        }
        c = file.get();
    }
    if (!output.empty()) {
        while (output.size() != 8)
            output.push_back(0);
        unsigned char ch = 0;
        for (int j = 0; j < 8; j++) {
            ch *= 2;
            ch += output.front();
            output.erase(output.begin());
        }
        archive << ch;
    }
    file.close();
}

void create_archive(Archive app) {
    std::ofstream archive;
    archive.open(app.archive_name);
    Headerwrite(app, archive);
    for (int i = 0; i < app.files.size(); i++) {
        Filewrite(app.files[i], archive, app);
    }
}

void Extract(Archive &app) {
    std::ifstream archive(app.archive_name);
    std::string size = "";
    std::vector<bool> header_archive;
    std::vector<int> file_size;

    std::vector<bool> ch;
    while (true) {
        for (int i = 0; i < 3; i++) {
            unsigned char c = archive.get();
            for (int j = 7; j >= 0; j--) {
                ch.push_back(c % 2);
                c /= 2;
            }
            for (int j = 0; j < 8; j++) {
                header_archive.push_back(ch.back());
                ch.pop_back();
            }
        }
        unsigned char c = 0;
        int z = 2;
        for (int j = 0; j < 8; j++) {
            c *= 2;
            c += header_archive[z];
            z += 3;
        }
        header_archive.clear();
        if (c != ' ')
            size += c;
        else
            break;
    }

    int iter = 0;
    std::string str_search = "";
    for (int i = 0; i < stoi(size) / 3; i++) {
        for (int t = 0; t < 3; t++) {
            unsigned char c = archive.get();
            for (int j = 7; j >= 0; j--) {
                ch.push_back(c % 2);
                c /= 2;
            }
            for (int j = 0; j < 8; j++) {
                header_archive.push_back(ch.back());
                ch.pop_back();
            }
        }
        unsigned char c = 0;
        int z = 2;
        for (int j = 0; j < 8; j++) {
            c *= 2;
            c += header_archive[z];
            z += 3;
        }
        header_archive.clear();
        if (c != ' ')
            str_search += c;
        else {
            iter = i + 1;
            break;
        }
    }
    app.word = std::stoi(str_search);

    str_search = "";
    for (int i = iter; i < stoi(size) / 3; i++) {
        for (int t = 0; t < 3; t++) {
            unsigned char c = archive.get();
            for (int j = 7; j >= 0; j--) {
                ch.push_back(c % 2);
                c /= 2;
            }
            for (int j = 0; j < 8; j++) {
                header_archive.push_back(ch.back());
                ch.pop_back();
            }
        }
        unsigned char c = 0;
        int z = 2;
        for (int j = 0; j < 8; j++) {
            c *= 2;
            c += header_archive[z];
            z += 3;
        }
        header_archive.clear();
        if (c != ' ')
            str_search += c;
        else {
            iter = i + 1;
            break;
        }
    }
    int files = stoi(str_search);
    str_search = "";
    files *= 2;
    while (files > 0) {
        for (int i = iter; i < stoi(size) / 3; i++) {
            for (int t = 0; t < 3; t++) {
                unsigned char c = archive.get();
                for (int j = 7; j >= 0; j--) {
                    ch.push_back(c % 2);
                    c /= 2;
                }
                for (int j = 0; j < 8; j++) {
                    header_archive.push_back(ch.back());
                    ch.pop_back();
                }
            }
            unsigned char c = 0;
            int z = 2;
            for (int j = 0; j < 8; j++) {
                c *= 2;
                c += header_archive[z];
                z += 3;
            }
            header_archive.clear();
            if (c != ' ')
                str_search += c;
            else {
                iter = i + 1;
                if (files % 2 == 0)
                    app.files.push_back(str_search);
                else
                    file_size.push_back(stoi(str_search));
                str_search = "";
                break;
            }
        }
        files--;
    }
    ControlBytes(app.word, app);
    file_size.push_back(stoi(str_search));

    for (int i = 0; i < app.files.size(); i++) {
        std::ofstream file(app.files[i]);
        std::vector<bool> container;
        std::vector<bool> output;
        ch.clear();
        int x = 0;
        while (x != file_size[i]) {
            unsigned char c = archive.get();
            for (int z = 7; z >= 0; z--) {
                ch.push_back(c % 2);
                c /= 2;
            }
            while (!ch.empty()) {
                while (container.size() != app.word + app.bytes && !ch.empty()) {
                    container.push_back(ch.back());
                    ch.pop_back();
                }
                if (container.size() == app.word + app.bytes) {
                    for (int z = 1; z <= pow(2, app.bytes - 1); z *= 2) {
                        container.erase(container.begin() + z - 1);
                    }
                    for (int z = 0; z < app.word; z++) {
                        output.push_back(container.front());
                        container.erase(container.begin());
                    }
                }
            }
            while (output.size() >= 8) {
                unsigned char cha = 0;
                for (int j = 0; j < 8; j++) {
                    cha *= 2;
                    cha += output.front();
                    output.erase(output.begin());
                }
                x++;
                file << cha;
            }
        }
    }
}

void hamarc(Archive app) {
    if (app.create) {
        create_archive(app);
    }
    if (app.extract) {
        Extract(app);
    }
}

int main(int argc, char **argv) {
    Archive app;
    Parsing(argc, argv, app);
    hamarc(app);
}