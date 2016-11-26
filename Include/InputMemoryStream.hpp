/* 
 * File:   InputMemoryStream.hpp
 * Author: dontsov
 *
 * Created on November 26, 2016, 1:27 PM
 */

#ifndef INPUTMEMORYSTREAM_HPP
#define INPUTMEMORYSTREAM_HPP

#include <stdint.h>
#include <stddef.h>
#include <type_traits>

class InputMemoryStream {
public:
    static const uint32_t MAX_BUFFER_SIZE;
    
    InputMemoryStream(char* inBuffer, uint32_t inByteCount);
    InputMemoryStream();
    ~InputMemoryStream();

    uint32_t GetRemainingDataSize() const;

    template<typename T>
    void Read(T outData) {
        static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value,
                "Generic Write only supports primitive data types");
        Read(&outData, sizeof(outData));
    }

    void Read(void* outData, uint32_t inByteCount);

    char* GetBuffer();
    void Clear();

private:
    char* mBuffer;
    uint32_t mHead;
    uint32_t mCapacity;
};

#endif /* INPUTMEMORYSTREAM_HPP */