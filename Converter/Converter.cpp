#include "Converter.h"
#include <iostream>

Converter::Converter(char *file_name) {
    this->file_name = file_name;
    file.open(this->file_name, std::ifstream::binary);
}

Converter::Converter() {}

void Converter::FindID3() {

    bool while_flag = true;

    unsigned char length_bits[4];

    unsigned char header[5];
    for (int i = 0; i < 5; i++)
        header[i] = file.get();

    while (while_flag) {
        while_flag = false;
        if (header[0] == 'I' && header[1] == 'D' && header[2] == '3' && (header[3] == 4 ||
            header[3] == 3) && header[4] == 0) {
            flag = file.get();
            for (int i = 0; i < 4; i++) {
                length_bits[i] = file.get();
                if (length_bits[i] >= 128)
                    while_flag = true;
            }
        } else {
            while_flag = true;
            for (int i = 1; i < 5; i++)
                header[i - 1] = header[i];
            header[4] = file.get();
        }
    }
    find = (int) file.tellg() - 10;
    GetLength(length_bits);
//    if((flag / 64) % 2 == 1){
//
//    }
}

void Converter::GetLength(unsigned char length_bits[4]) {
    length = length_bits[3] + length_bits[2] * pow(2, 7) +
             length_bits[1] * pow(2, 14) + length_bits[0] * pow(2, 21);
}

void Converter::Parsing() {
    while (file.tellg() <= find + length) {
        Frame frame;
        frame.frame_name = "";
        for (int i = 0; i < 4; i++)
            frame.frame_name += file.get();
        if ((frame.frame_name[0] < 'A' || frame.frame_name[0] > 'Z') &&
            (frame.frame_name[0] < '0' || frame.frame_name[0] > '9'))
            break;
        for (int i = 3; i >= 0; i--)
            frame.length += file.get() * pow(2, 7 * i);
        frame.flag1 = file.get();
        frame.flag2 = file.get();
        frame.info = new unsigned char[frame.length];

        for (int i = 0; i < frame.length; i++) {
            frame.info[i] = file.get();
        }

        std::string taboo[11] = {"AENC", "APIC", "ASPI", "GEOB", "MCDI", "MLLT",
                                 "RVRB", "SIGN", "SYTC", "TFLT", "TMED"};

        bool flag = true;
        for (int i = 0; i < 11; i++) {
            if (frame.frame_name == taboo[i])
                flag = false;
        }
        if (flag) {
            if((frame.flag2 / 2) % 2 == 1){
                unsigned char* information = new unsigned char[frame.length];
                for(int i = 0; i < frame.length; i++)
                    information[i] = frame.info[i];
                int n = 0;
                for(int i = 0; i < frame.length; i++){
                    if(frame.info[i] == 255){
                        n++;
                    }
                }
                frame.info = new unsigned char [frame.length - n];
                int j = 0;
                for(int i = 0; i < frame.length; i++){
                    if(information[i] != 255){
                        frame.info[j] = information[i];
                        j++;
                    }
                    else{
                        frame.info[j] = information[i];
                        j++;
                        i++;
                    }
                }
                if(information[frame.length - n - 2] != 255)
                    frame.info[j] = information[frame.length - 1];
                frame.length -= n;
            }
            frames_storage.add(frame);
        }
    }
}

void Converter::Output(char *filename) {
    std::ofstream output(filename, std::ofstream::binary | std::ofstream::trunc);
    for (int i = 0; i < frames_storage.cur_length; i++) {
        if (frames_storage.storage[i].frame_name == "COMM") {
            output << "COMM | Language: " << frames_storage.storage[i].info[1] << frames_storage.storage[i].info[2]
                   << frames_storage.storage[i].info[3] << " | content description: ";
            int n = 0;
            if(frames_storage.storage[i].flag2 % 2 == 1)
                n = 4;
            int x;
            for (int j = 4 + n; j < frames_storage.storage[i].length; j++) {
                x = j;
                if (frames_storage.storage[i].info[j] == '\0')
                    break;
                output << frames_storage.storage[i].info[j];
            }
            output << " | actual text: ";
            if (frames_storage.storage[i].info[0] == 1 || frames_storage.storage[i].info[0] == 2) {
                for (int j = x + 2;
                     j < frames_storage.storage[i].length && frames_storage.storage[i].info[j] != 0; j++) {
                    output << frames_storage.storage[i].info[j];
                }
            } else {
                for (int j = x + 1;
                     j < frames_storage.storage[i].length && frames_storage.storage[i].info[j] != 0; j++) {
                    output << frames_storage.storage[i].info[j];
                }
            }
            output << "\n";
        } else if ((frames_storage.storage[i].frame_name == "TXXX") ||
                   (frames_storage.storage[i].frame_name == "WXXX")) {
            output << frames_storage.storage[i].frame_name << " | description: ";
            int n = 0;
            if(frames_storage.storage[i].flag2 % 2 == 1)
                n = 4;
            int x;
            for (int j = 1 + n; j < frames_storage.storage[i].length; j++) {
                x = j;
                if (frames_storage.storage[i].info[j] == '\0')
                    break;
                output << frames_storage.storage[i].info[j];
            }
            output << " | value: ";

            if (frames_storage.storage[i].info[0] == 1 || frames_storage.storage[i].info[0] == 2) {
                for (int j = x + 2;
                     j < frames_storage.storage[i].length && frames_storage.storage[i].info[j] != 0; j++) {
                    output << frames_storage.storage[i].info[j];
                }
            } else {
                for (int j = x + 1;
                     j < frames_storage.storage[i].length && frames_storage.storage[i].info[j] != 0; j++) {
                    output << frames_storage.storage[i].info[j];
                }
            }
            output << "\n";
        } else {
            if (frames_storage.storage[i].frame_name[0] == 'T') {
                output << frames_storage.storage[i].frame_name;
                output << " | info: ";
                int n = 0;
                if(frames_storage.storage[i].flag2 % 2 == 1)
                    n = 4;

                int x = 1;
                for (int j = x + n; j < frames_storage.storage[i].length; j++) {
                    x = j + 1;
                    if(frames_storage.storage[i].info[j] == 0)
                        break;
                    output << frames_storage.storage[i].info[j];
                }
                if(x < frames_storage.storage[i].length)
                    output << " | ";
                while (x < frames_storage.storage[i].length && frames_storage.storage[i].info[x] != 0) {
                    for (int j = x; j < frames_storage.storage[i].length; j++) {
                        x = j + 1;
                        if(frames_storage.storage[i].info[j] == 0)
                            break;
                        output << frames_storage.storage[i].info[j];
                    }
                    output << " | ";
                }

                output << "\n";
            } else {
                for (int j = 0; j < 4; j++)
                    output << frames_storage.storage[i].frame_name[j];
                output << " | ";
                int n = 0;
                if(frames_storage.storage[i].flag2 % 2 == 1)
                    n = 4;
                for (int j = 0 + n; j < frames_storage.storage[i].length && frames_storage.storage[i].info[j] != 0; j++)
                    output << frames_storage.storage[i].info[j];
                output << "\n";
            }
        }
    }
}