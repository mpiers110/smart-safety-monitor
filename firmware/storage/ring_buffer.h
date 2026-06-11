#pragma once
#include <Arduino.h>

namespace storage {

template <typename T, size_t SIZE>
class RingBuffer {
public:
    bool push(const T& item);
    bool pop(T& item);

    bool isFull() const;
    bool isEmpty() const;
    size_t count() const;

private:
    T buffer[SIZE];
    size_t head = 0;
    size_t tail = 0;
    size_t size = 0;
};

}