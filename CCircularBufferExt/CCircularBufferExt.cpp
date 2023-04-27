#include "CCircularBufferExt.h"
#include <iostream>

template<typename T, typename Allocator>
CCircularBufferExt<T, Allocator>::Iterator::Iterator(CCircularBufferExt<T> &rhs) : rhs_(rhs), index_(0) {}

template<typename T, typename Allocator>
CCircularBufferExt<T, Allocator>::Iterator::Iterator(CCircularBufferExt<T> &rhs, int index) : rhs_(rhs), index_(index) {
    rhs_ = rhs;
    if (index > 0)
        index_ = index % rhs.capacity_;
    else{
        int a = index_;
        int b = rhs.capacity_;
        if (a % b == 0)
            index_ = a % b - b + rhs_.size_;
        else
            index_ = a % b + rhs_.size_;
    }
}

template<typename T, typename Allocator>
typename CCircularBufferExt<T, Allocator>::Iterator &CCircularBufferExt<T, Allocator>::Iterator::operator++() {
    index_++;
    if(index_ == rhs_.size_)
        index_ = 0;
    return *this;
}

template<typename T, typename Allocator>
typename CCircularBufferExt<T, Allocator>::Iterator &CCircularBufferExt<T, Allocator>::Iterator::operator--() {
    index_--;
    if(index_ == -1)
        index_ = rhs_.size_ - 1;
    return *this;
}

template<typename T, typename Allocator>
typename CCircularBufferExt<T, Allocator>::Iterator CCircularBufferExt<T, Allocator>::Iterator::operator+(int num_) {
    if (num_ < 0)
        return operator-(-num_);
    Iterator tmp = *this;
    tmp.index_ += num_;
    tmp.index_ %= rhs_.size_;
    return tmp;
}

template<typename T, typename Allocator>
typename CCircularBufferExt<T, Allocator>::Iterator CCircularBufferExt<T, Allocator>::Iterator::operator-(int num_) {
    if (num_ < 0)
        return operator+(-num_);
    Iterator tmp = *this;
    tmp.index_ -= num_;
    int a = tmp.index_;
    int b = rhs_.size_;
    if (a % b == 0)
        tmp.index_ = a % b - b + rhs_.size_;
    else
        tmp.index_ = a % b + rhs_.size_;
    return tmp;
}

template<typename T, typename Allocator>
T &CCircularBufferExt<T, Allocator>::Iterator::operator*() {
    if (index_ > rhs_.size() || index_ < 0) {
        std::cout << "Segmentation fault";
        exit(1);
    }
    return rhs_.memory_[index_];
}

template<typename T, typename Allocator>
bool CCircularBufferExt<T, Allocator>::Iterator::operator==(const Iterator &other) {
    return other.index_ == index_;
}

template<typename T, typename Allocator>
bool CCircularBufferExt<T, Allocator>::Iterator::operator!=(const Iterator &other) {
    return index_ != other.index_;
}

template<typename T, typename Allocator>
T &CCircularBufferExt<T, Allocator>::ConstIterator::operator*() {
    if (index_ > rhs_.size_ || index_ < 0) {
        std::cout << "Segmentation fault";
        exit(1);
    }
    return rhs_[index_];
}

template<typename T, typename Allocator>
bool CCircularBufferExt<T, Allocator>::ConstIterator::operator==(const ConstIterator &other) {
    return other.index_ == index_;
}

template<typename T, typename Allocator>
bool CCircularBufferExt<T, Allocator>::ConstIterator::operator!=(const ConstIterator &other) {
    return index_ != other.index_;
}

template<typename T, typename Allocator>
CCircularBufferExt<T, Allocator>::CCircularBufferExt() {
    size_ = 0;
    capacity_ = 0;
    head_ = tail_ = 0;
    memory_ = allocator.allocate(capacity_);
}

template<typename T, typename Allocator>
CCircularBufferExt<T, Allocator>::CCircularBufferExt(size_t capacity) {
    size_ = 0;
    capacity_ = capacity;
    head_ = tail_ = 0;
    memory_ = allocator.allocate(capacity_);
    for (int i = 0; i < capacity_; ++i) {
        memory_[i] = 0;
    }
}

template<typename T, typename Allocator>
CCircularBufferExt<T, Allocator>::CCircularBufferExt(size_t capacity, T defaultValue) {
    size_ = capacity;
    capacity_ = capacity;
    head_ = 0;
    tail_ = capacity_;
    memory_ = allocator.allocate(capacity_);
    for (int i = 0; i < capacity_; ++i) {
        memory_[i] = defaultValue;
    }
}

template<typename T, typename Allocator>
CCircularBufferExt<T, Allocator>::~CCircularBufferExt() {
    allocator.deallocate(memory_, capacity_);
}

template<typename T, typename Allocator>
CCircularBufferExt<T, Allocator>::CCircularBufferExt(const CCircularBufferExt &rhs) {
    capacity_ = rhs.capacity_;
    size_ = rhs.size_;
    head_ = rhs.head_;
    tail_ = rhs.tail_;
    memory_ = allocator.allocate(capacity_);
    for (int i = 0; i < capacity_; ++i) {
        memory_[i] = rhs.memory_[i];
    }
}

template<typename T, typename Allocator>
CCircularBufferExt<T, Allocator> &CCircularBufferExt<T, Allocator>::operator=(const CCircularBufferExt<T, Allocator> &rhs) {
    if (this == &rhs) {
        return *this;
    }
    allocator.deallocate(memory_, capacity_);

    capacity_ = rhs.capacity_;
    size_ = rhs.size_;
    head_ = rhs.head_;
    tail_ = rhs.tail_;
    memory_ = allocator.allocate(capacity_);
    for (int i = 0; i < capacity_; ++i) {
        memory_[i] = rhs.memory_[i];
    }
    return *this;
}

template<typename T, typename Allocator>
bool CCircularBufferExt<T, Allocator>::operator==(const CCircularBufferExt &rhs) {
    if (capacity_ != rhs.capacity_) {
        return false;
    }
    if (size_ != rhs.size_) {
        return false;
    }
    for (int i = 0; i < size_; ++i) {
        if (memory_[i] != rhs.memory_[i]) {
            return false;
        }
    }
    return true;
}

template<typename T, typename Allocator>
bool CCircularBufferExt<T, Allocator>::operator!=(const CCircularBufferExt &rhs) {
    if (capacity_ == rhs.capacity_) {
        return false;
    }
    if (size_ == rhs.size_) {
        return false;
    }
    for (int i = 0; i < size_; ++i) {
        if (memory_[i] == rhs.memory_[i]) {
            return false;
        }
    }
    return true;
}

template<typename T, typename Allocator>
void CCircularBufferExt<T, Allocator>::swap(CCircularBufferExt &rhs) {
    CCircularBufferExt tmp = *this;
    *this = rhs;
    rhs = tmp;
}

template<typename T>
void swap(CCircularBufferExt<T> &lhs, CCircularBufferExt<T> &rhs) {
    CCircularBufferExt<T> tmp = lhs;
    lhs = rhs;
    rhs = tmp;
}

template<typename T, typename Allocator>
void CCircularBufferExt<T, Allocator>::push_back(const T &item) {
    if (size_ == capacity_) {
        int newcapacity_ = (capacity_ == 0) ? 1 : capacity_ * 2;
        reserve(newcapacity_);
    }
    memory_[tail_] = item;
    tail_ = (tail_ + 1) % capacity_;
    size_++;
}

template<typename T, typename Allocator>
void CCircularBufferExt<T, Allocator>::push_front(const T &item) {
    if (size_ == capacity_) {
        int newcapacity_ = (capacity_ == 0) ? 1 : capacity_ * 2;
        reserve(newcapacity_);
    }
    head_ = (head_ - 1 + capacity_) % capacity_;
    memory_[head_] = item;
    size_++;
}

template<typename T, typename Allocator>
void CCircularBufferExt<T, Allocator>::pop_back() {
    if (size_ > 0) {
        tail_ = (tail_ - 1 + capacity_) % capacity_;
        size_--;
    }
}

template<typename T, typename Allocator>
void CCircularBufferExt<T, Allocator>::pop_front() {
    if (size_ > 0) {
        head_ = (head_ + 1) % capacity_;
        size_--;
    }
}

template<typename T, typename Allocator>
void CCircularBufferExt<T, Allocator>::reserve(const int newCapacity) {
    T *newMemory = allocator.allocate(newCapacity);
    for (int i = 0; i < size_; i++) {
        newMemory[i] = (*this)[i];
    }
    allocator.deallocate(memory_, capacity_);
    memory_ = newMemory;
    head_ = 0;
    tail_ = size_;
    capacity_ = newCapacity;
}