/*
 * File:   OutputMemoryBitStream.hpp
 * Author: dontsov
 *
 * Created on November 30, 2016, 9:12 PM
 */

#ifndef OUTPUTMEMORYBITSTREAM_HPP
#define OUTPUTMEMORYBITSTREAM_HPP

#include <stdint.h>
#include <stddef.h>
#include <type_traits>

const size_t BITS_IN_BYTE_COUNT = 8;

class OutputMemoryBitStream {
public:
    OutputMemoryBitStream();
    ~OutputMemoryBitStream();

    // Работает только для little-endian платформ
    // Для big-endian платформ необходимо поменять биты местами
    // В шаблонной функции Write перед вызовом WriteBits
    void WriteBits(uint8_t inData, size_t inBitCount);
    void WriteBits(const void* inData, size_t inBitCount);

    const char* GetBufferPtr() const;
    uint32_t GetBitLength() const;
    uint32_t GetByteLength() const;

    template <typename T>
    void Write(T inData, size_t inBitCount = sizeof(T) * BITS_IN_BYTE_COUNT) {
        static_assert(  std::is_arithmetic<T>::value || std::is_enum<T>::value, 
                        "Generic Write only supports primitive data types");
        WriteBits(&inData, inBitCount);
    }

    void Write(bool inData) {
        WriteBits(&inData, 1);
    }

private:
    void ReallocBuffer(uint32_t inNewBitCapacity);

    char* mBuffer;
    uint32_t mBitHead;
    uint32_t mBitCapacity;
};

#endif /* OUTPUTMEMORYBITSTREAM_HPP */