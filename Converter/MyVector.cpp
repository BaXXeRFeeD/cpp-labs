#include "MyVector.h"

void MyVector::add(Frame new_frame) {
    if (storage == nullptr) {
        storage = new Frame[1];
        storage[0] = new_frame;
        max_length = 1;
        cur_length = 1;
    } else {
        if (cur_length == max_length) {
            max_length *= 2;
            Frame* new_storage = new Frame[max_length];
            for (int i = 0; i < cur_length; i++) {
                new_storage[i] = storage[i];
            }
            new_storage[cur_length] = new_frame;
            cur_length++;

            delete[] storage;
            storage = new Frame[max_length];
            for (int i = 0; i < cur_length; i++) {
                storage[i] = new_storage[i];
            }
            delete[] new_storage;
        } else {
            storage[cur_length] = new_frame;
            cur_length++;
        }
    }
}
