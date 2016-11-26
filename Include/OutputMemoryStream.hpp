/* 
 * File:   OutputMemoryStream.hpp
 * Author: dontsov
 *
 * Created on November 24, 2016, 9:43 PM
 */

#ifndef OUTPUTMEMORYSTREAM_HPP
#define OUTPUTMEMORYSTREAM_HPP

#include <stdint.h>
#include <stddef.h>
#include <type_traits>

#include "ByteSwap.hpp"

#define STREAM_ENDIANESS 0
#define PLATFORM_ENDIANESS 0

class OutputMemoryStream {
public:
    OutputMemoryStream();
    virtual ~OutputMemoryStream();

    const char* GetBufferPtr() const;
    uint32_t GetLength() const;

    template <typename T>
    void Write(T inData) {
        static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value,
                        "Generic Write only supports primitive data types");
        if (STREAM_ENDIANESS == PLATFORM_ENDIANESS) {
            Write(&inData, sizeof(inData));
        } else {
            T swappedData = ByteSwap(inData);
            Write(&swappedData, sizeof(swappedData));
        }
    }

    void Write(const void* inData, size_t inByteCount);

private:
    void ReallocBuffer(uint32_t inNewLength);
    char* mBuffer;
    uint32_t mHead;
    uint32_t mCapacity;
};


inline const char* OutputMemoryStream::GetBufferPtr() const {
    return mBuffer;
}

inline uint32_t OutputMemoryStream::GetLength() const {
    return mHead;
}

#endif /* OUTPUTMEMORYSTREAM_HPP */