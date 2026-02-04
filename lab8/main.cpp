#include <iostream>
#include "CCircularBuffer/CCircularBuffer.cpp"
#include "CCircularBufferExt/CCircularBufferExt.cpp"


int main() {
    CCircularBufferExt<int> arr(3);
    arr.push_back(0);
    arr.push_back(0);
    arr.push_back(0);
    arr.push_back(0);
    CCircularBufferExt<int> arr2(4, 0);
    std::cout << arr.capacity();
    return 0;
}