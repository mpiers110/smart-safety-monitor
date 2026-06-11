#include "ring_buffer.h"

namespace storage {

template <typename T, size_t SIZE>
bool RingBuffer<T, SIZE>::push(const T& item) {
    if (isFull()) {
        // overwrite oldest (important for telemetry systems)
        tail = (tail + 1) % SIZE;
        size--;
    }

    buffer[head] = item;
    head = (head + 1) % SIZE;
    size++;

    return true;
}

template <typename T, size_t SIZE>
bool RingBuffer<T, SIZE>::pop(T& item) {
    if (isEmpty()) return false;

    item = buffer[tail];
    tail = (tail + 1) % SIZE;
    size--;

    return true;
}

template <typename T, size_t SIZE>
bool RingBuffer<T, SIZE>::isFull() const {
    return size == SIZE;
}

template <typename T, size_t SIZE>
bool RingBuffer<T, SIZE>::isEmpty() const {
    return size == 0;
}

template <typename T, size_t SIZE>
size_t RingBuffer<T, SIZE>::count() const {
    return size;
}

// Explicit instantiation (IMPORTANT for ESP32 builds)
template class RingBuffer<int, 64>;

}