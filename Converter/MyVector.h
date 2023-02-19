class Frame {
public:
    char frame_name[4];
    int length = 0;
    unsigned char flag1 = 0;
    unsigned char flag2 = 0;
    unsigned char code = 0;
    char* info = nullptr;
};

class MyVector {
public:
    Frame* storage = nullptr;
    int max_length = 0;
    int cur_length = 0;

    void add(Frame new_frame);
};
