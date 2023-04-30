#include <iostream>
#include <utility>

namespace new_std {

    template<typename InputIt, typename UnaryPredicate>
    bool all_of(InputIt first, InputIt last, UnaryPredicate p) {
        if(first == last && !p(*first))
            return false;
        for (auto it = first; it != last; ++it) {
            if (!p(*it)) {
                return false;
            }
        }
        return true;
    }

    template<typename InputIt, typename UnaryPredicate>
    bool any_of(InputIt first, InputIt last, UnaryPredicate p) {
        if(first == last && p(*first))
            return true;
        for (auto it = first; it != last; ++it) {
            if (p(*it)) {
                return true;
            }
        }
        return false;
    }

    template<typename InputIt, typename UnaryPredicate>
    bool none_of(InputIt first, InputIt last, UnaryPredicate p) {
        if(first == last && p(*first))
            return false;
        for (auto it = first; it != last; ++it) {
            if (p(*it)) {
                return false;
            }
        }
        return true;
    }

    template <typename InputIterator, typename UnaryPredicate>
    bool one_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
        bool found = false;
        if(first == last && pred(*first))
            return true;
        for (auto it = first; it != last; ++it) {
            if (pred(*it)) {
                if (found) {
                    return false;
                }
                found = true;
            }
        }
        return found;
    }

    template <typename ForwardIterator, typename Compare>
    bool is_sorted(ForwardIterator first, ForwardIterator last, Compare comp) {
        if (first == last) {
            return true;
        }
        ForwardIterator next = first;
        for (++next; next != last; ++first, ++next) {
            if (comp(*next, *first)) {
                return false;
            }
        }
        return true;
    }

    template <typename InputIterator, typename UnaryPredicate>
    bool is_partitioned(InputIterator first, InputIterator last, UnaryPredicate pred) {
        bool found_true = false;
        bool found_false = false;
        for (; first != last; ++first) {
            if (pred(*first)) {
                found_true = true;
            } else {
                found_false = true;
            }
            if (found_true && found_false) {
                return true;
            }
        }
        return false;
    }

    template <typename InputIterator, typename T>
    InputIterator find_not(InputIterator first, InputIterator last, const T& value) {
        for (auto it = first; it != last; ++it) {
            if (*it != value) {
                return it;
            }
        }
        return last;
    }

    template <typename BidirectionalIterator, typename T>
    BidirectionalIterator find_backward(BidirectionalIterator first, BidirectionalIterator last, const T& value) {
        for (auto it = last; it != first; --it) {
            if (*it != value) {
                return it;
            }
        }
        return first;
    }

    template <typename ForwardIterator, typename UnaryPredicate>
    bool is_palindrome(ForwardIterator first, ForwardIterator last, UnaryPredicate pred) {
        while (last != first && last != --first) {
            if (!pred(*first, *last)) {
                return false;
            }
        }
        return true;
    }

    template<typename Iterator1, typename Iterator2>
    class ZipIterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = std::pair<typename std::iterator_traits<Iterator1>::value_type,
                typename std::iterator_traits<Iterator2>::value_type>;
        using difference_type = std::ptrdiff_t;
        using reference = value_type;


        explicit ZipIterator(Iterator1 it1, Iterator2 it2)
                : iterator1_(it1), iterator2_(it2) {}

        bool operator!=(const ZipIterator &other) const {
            return iterator1_ != other.iterator1_ || iterator2_ != other.iterator2_;
        }

        reference operator*() const {
            return std::make_pair(*iterator1_, *iterator2_);
        }

        ZipIterator &operator++() {
            ++iterator1_;
            ++iterator2_;
            return *this;
        }

    private:
        Iterator1 iterator1_;
        Iterator2 iterator2_;
    };

    template<typename Container1, typename Container2>
    class zip {
    public:
        zip(const Container1 &c1, const Container2 &c2)
                : container1_(c1), container2_(c2) {}

        auto begin() const {
            return ZipIterator(
                    container1_.begin(), container2_.begin());
        }

        auto end() const {
            return ZipIterator(
                    container1_.end(), container2_.end());
        }

    private:
        const Container1 &container1_;
        const Container2 &container2_;
    };

    template<typename T>
    class xrange {
    public:

        std::size_t size() const {
            if(step_ > 0)
                return (end_ - start_ + step_ - 1) / step_;
            else if(step_ < 0)
                return (start_ - end_ - step_ - 1) / (-step_);
            return 0;
        }

        class iterator {
        public:
            using iterator_category = std::input_iterator_tag;
            using value_type = T;
            using difference_type = T;
            using pointer = const T*;
            using reference = const T&;

            iterator(T value, T step) : value_(value), step_(step) {}

            reference operator*() const { return value_; }
            pointer operator->() const { return &value_; }

            iterator& operator++() {
                value_ += step_;
                return *this;
            }

            iterator operator++(int) {
                return (*this)++;
            }

            bool operator!=(const iterator& other) const {
                return (step_ >= 0 && value_ < other.value_) || (step_ < 0 && value_ > other.value_);
            }

        private:
            T value_;
            T step_;
        };

        xrange(T start, T end, T step) : start_(start), end_(end), step_(step) {}
        xrange(T end) : xrange(T(), end, T(1)) {}
        xrange(T start, T end) : xrange(start, end, T(1)) {}

        iterator begin() const { return iterator(start_, step_); }
        iterator end() const { return iterator(end_, step_); }

    private:
        T start_;
        T end_;
        T step_;
    };

}

