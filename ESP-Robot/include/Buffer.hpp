#pragma once

#include <Arduino.h>

template<typename T, size_t N>
class Buffer
{
public:
    Buffer() = default;

    void push(T const& value)
    {
        buffer[idx] = value;
        if (idx < N)
            idx++;
    }

    bool isFull() const
    {
        return idx >= N;
    }

    void clear()
    {
        idx = 0;
    }

    const T operator[](size_t index) const
    {
        return buffer[index];
    }

private:
    T buffer[N];
    size_t idx = 0;
};