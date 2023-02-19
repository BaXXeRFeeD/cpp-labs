#include "Converter.h"
#include <iostream>
Converter::Converter(char* file_name) {
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
    while(frames_storage.cur_length < 1){
        Frame frame;
        for(int i = 0; i < 4; i++)
            frame.frame_name[i] = file.get();
        for(int i = 3; i >= 0; i--)
            frame.length += file.get() * pow(2, 7 * i);
        frame.flag1 = file.get();
        frame.flag2 = file.get();
        frame.info = new char[frame.length];

        for(int i = 0; i < frame.length; i++){
            frame.info[i] = file.get();
        }
        std::cout << frame.length << " ";

        frames_storage.add(frame);
    }
}
