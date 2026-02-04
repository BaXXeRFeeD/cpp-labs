#include <iostream>

template<typename T, typename Allocator = std::allocator<T>>
class CCircularBufferExt {
private:
    Allocator allocator;
    size_t capacity_;
    size_t size_;
    int head_;
    int tail_;
    T *memory_;

    void reserve(const int newCapacity);
public:

    CCircularBufferExt();

    CCircularBufferExt(size_t capacity);

    CCircularBufferExt(size_t capacity, T defaultValue);

    ~CCircularBufferExt();

    CCircularBufferExt(const CCircularBufferExt &rhs);

    CCircularBufferExt &operator=(const CCircularBufferExt &rhs);

    bool operator==(const CCircularBufferExt &rhs);

    bool operator!=(const CCircularBufferExt &rhs);

    void swap(CCircularBufferExt &rhs);

    [[nodiscard]] size_t size() const {
        return size_;
    }

    [[nodiscard]] size_t max_size() const {
        return capacity_;
    }

    [[nodiscard]] size_t capacity() const {
        return capacity_;
    }

    [[nodiscard]] bool empty() const {
        return (size_ == 0);
    }

    T &at(int index) {
        if (index >= size_ || index < 0) {
            std::cout << "Segmentation fault";
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

    T &operator[](int index) {
        if (index > size_ || index < 0) {
            std::cout << "Segmentation fault";
            exit(1);
        }
        return memory_[(head_ + index) % capacity_];
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

    void print() {
        for (int i = 0; i < capacity_; ++i) {
            std::cout << memory_[i] << " ";
        }
        std::cout << '\n';
    }

    void push_back(const T &item);

    void push_front(const T &item);

    void pop_back();

    void pop_front();

    class Iterator {
    public:
        CCircularBufferExt<T> &rhs_;
        int index_;

        Iterator(CCircularBufferExt<T> &rhs);

        Iterator(CCircularBufferExt<T> &rhs, int index);

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
        const CCircularBufferExt<T> &rhs_;
        const int index_;

        ConstIterator(CCircularBufferExt<T> &rhs) : rhs_(rhs), index_(0) {}

        ConstIterator(CCircularBufferExt<T> &rhs, int index) : rhs_(rhs), index_(index) {}

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