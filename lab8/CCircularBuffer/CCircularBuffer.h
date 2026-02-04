#include "iostream"

template<typename T, typename Allocator = std::allocator<T>>
class CCircularBuffer {
private:
    Allocator allocator;
    size_t capacity_;
    size_t size_;
    int head_;
    int tail_;
    T *memory_;
public:

    CCircularBuffer();

    CCircularBuffer(size_t capacity);

    ~CCircularBuffer();

    CCircularBuffer(size_t capacity, T defaultValue);

    CCircularBuffer(const CCircularBuffer &rhs);

    CCircularBuffer &operator=(const CCircularBuffer &rhs);

    bool operator==(const CCircularBuffer &rhs);

    bool operator!=(const CCircularBuffer &rhs);

    void swap(CCircularBuffer &rhs);

    size_t size() const {
        return size_;
    }

    size_t max_size() const {
        return capacity_;
    }

    size_t capacity() const {
        return capacity_;
    }

    bool empty() const {
        return (size_ == 0);
    }

    T &at(int index) {
        if (index >= size_ || index < 0) {
            std::cout<< "Segmentation fault";
            exit(1);
        }
        return memory_[index];
    }

    void clear() {
        head_ = 0;
        tail_ = 0;
        for (int i = 0; i < size_; ++i) {
            memory_[i] = 0;
        }
        size_ = 0;
    }

    T &operator[](const int index) {
        int i = index % capacity_;
        return memory_[i];
    }

    T &back() {
        if (tail_ == 0) {
            return memory_[size_ - 1];
        }
        return memory_[tail_ - 1];
    }

    T &front() {
        return memory_[head_];
    }

    void push_back(const T &item);

    void push_front(const T &item);

    void pop_back();

    void pop_front();

    class Iterator {
    public:
        CCircularBuffer<T> &rhs_;
        int index_;

        Iterator(CCircularBuffer<T> &rhs);

        Iterator(CCircularBuffer<T> &rhs, int index);

        Iterator &operator++();

        Iterator &operator--();

        Iterator operator+(int);

        Iterator operator-(int);

        T &operator*();

        bool operator==(const Iterator &other);

        bool operator!=(const Iterator &other);

    };

    class ConstIterator {
    public:
        const CCircularBuffer<T> &rhs_;
        const int index_;

        ConstIterator(CCircularBuffer<T> &rhs) : rhs_(rhs), index_(0) {}

        ConstIterator(CCircularBuffer<T> &rhs, int index) : rhs_(rhs), index_(index) {}

        T &operator*();

        bool operator==(const ConstIterator &other);

        bool operator!=(const ConstIterator &other);
    };

    Iterator begin() {
        return Iterator(*this, head_);
    }

    Iterator end() {
        if (tail_ == 0) {
            return Iterator(*this, size_ - 1);
        }
        return Iterator(*this, tail_ - 1);
    }

    ConstIterator cbegin() {
        return ConstIterator(*this, head_);
    }

    ConstIterator cend() {
        return ConstIterator(*this, tail_ - 1);
    }
};