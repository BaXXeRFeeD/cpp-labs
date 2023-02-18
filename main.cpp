#include <iostream>
#include <fstream>
int main(){
    std::ifstream file;
    file.open("tag.mp3");
    char c = file.get();
    c = file.get();
    c = file.get();
    c = file.get();
    c = file.get();
    std::cout << (int) c;
}