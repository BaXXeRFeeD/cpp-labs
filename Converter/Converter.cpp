#include "Converter.h"
#include <iostream>

Converter::Converter(char *file_name) {
    this->file_name = file_name;
    file.open(this->file_name, std::ifstream::binary);
}

void Converter::FindID3() {

    bool while_flag = true;

    unsigned char length_bits[4];

    while (while_flag) {
        while_flag = false;
        if (file.get() == 'I' && file.get() == 'D' && file.get() == '3' &&
            file.get() == 4 && file.get() == 0) {
            flag = file.get();
            for (int i = 0; i < 4; i++) {
                length_bits[i] = file.get();
                if (length_bits[i] >= 128)
                    while_flag = true;
            }
        } else {
            while_flag = true;
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
        for (int i = 0; i < 4; i++)
            frame.frame_name[i] = file.get();
        for (int i = 3; i >= 0; i--)
            frame.length += file.get() * pow(2, 7 * i);
        frame.flag1 = file.get();
        frame.flag2 = file.get();
        frame.info = new char[frame.length];

        for (int i = 0; i < frame.length; i++) {
            frame.info[i] = file.get();
        }
        std::cout << frame.length << " ";

        frames_storage.add(frame);
    }
}

void Converter::Output(char *filename) {
    std::ofstream output(filename, std::ofstream::binary | std::ofstream::trunc);
    for (int i = 0; i < frames_storage.cur_length; i++) {
        if (frames_storage.storage[i].frame_name[0] == 'T') {
            for (int j = 1; j < frames_storage.cur_length; j++)
                output << frames_storage.storage->info[j];
        } else if (frames_storage.storage[i].frame_name[0] == 'W') {
            if (frames_storage.storage[i].frame_name[1] == 'X' && frames_storage.storage[i].frame_name[2] == 'X' &&
                frames_storage.storage[i].frame_name[3] == 'X') {
                for (int j = 1; j < frames_storage.cur_length; j++)
                    output << frames_storage.storage->info[j];
            } else {
                for (int j = 0; j < frames_storage.cur_length; j++)
                    output << frames_storage.storage->info[j];
            }
        } else
            for (int j = 0; j < frames_storage.cur_length; j++)
                output << frames_storage.storage->info[j];
    }
}


