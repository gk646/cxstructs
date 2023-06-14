#ifndef CXSTRUCTURES_STACK_H
#define CXSTRUCTURES_STACK_H

#include <cstdint>
#include <algorithm>

namespace cxstructs {
    template<typename T>
    class Stack {
        T *arr_;
        uint_fast32_t size_;
        uint_fast32_t arrlen_;
        uint_fast32_t minlen;

        void grow() {
            arrlen_ *= 2;
            auto newarr = new T[arrlen_];
            std::copy_n(arr_, size_, newarr);
            delete[] arr_;
            arr_ = newarr;
        }

        void shrink() {
            arrlen_ /= 2;
            auto newarr = new T[arrlen_];
            std::copy_n(arr_, size_, newarr);
            delete[] arr_;
            arr_ = newarr;
            minlen = size_ / 2 > 16 ? size_ / 2 : 0;
        }

    public:
        explicit Stack(uint_fast32_t initialCapacity = 16) : arrlen_(initialCapacity), size_(0), arr_(new T[arrlen_]),
                                                             minlen(0) {}

        explicit Stack(T fillVal, uint_fast32_t initialCapacity = 16) : arrlen_(initialCapacity), size_(0),
                                                                        arr_(new T[arrlen_]), minlen(0) {
            std::fill_n(arr_, fillVal);
            size_ = arrlen_;
        }

        Stack(const Stack<T> &o) : arrlen_(o.arrlen_), size_(o.size_), arr_(new T[arrlen_]), minlen(o.minlen) {
            std::copy_n(o.arr_, arrlen_, arr_);
        }

        ~Stack() {
            delete[] arr_;
        }

        Stack &operator=(const Stack<T> &o) {
            if (this == &o)return *this;
            minlen = o.minlen;
            arrlen_ = o.arrlen_;
            size_ = o.size_;
            delete[] arr_;
            arr_ = new T[o.arrlen_];
            std::copy_n(o.arr_, arrlen_, arr_);

            return *this;
        }

        /**
         *
         * @return the current size of the stack
         */
        [[nodiscard]] uint_fast32_t size() {
            return size_;
        }

        void add(T val) {
            if (size_ == arrlen_) {
                grow();
            }
            arr_[size_++] = val;
        }

        T &pop() {
            if (size_ == 0) {
                throw std::logic_error("cant pop from empty stack!");
            }
            if (size_ - 1 < minlen) {
                shrink();
            }
            size_--;
            return arr_[size_];

        }

        T &top() {
            return arr_[size_ - 1];
        }

        static void TEST() {

        }
    };
}

#endif //CXSTRUCTURES_STACK_H
