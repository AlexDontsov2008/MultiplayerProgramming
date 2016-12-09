/*
 * File:   OutputMemoryBitStream.cpp
 * Author: dontsov
 *
 * Created on November 30, 2016, 9:12 PM
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "OutputMemoryBitStream.hpp"

OutputMemoryBitStream::OutputMemoryBitStream() {
    ReallocBuffer(256);
}

OutputMemoryBitStream::~OutputMemoryBitStream() {
    free(mBuffer);
}

const char* OutputMemoryBitStream::GetBufferPtr() const {
    return mBuffer;
}

uint32_t OutputMemoryBitStream::GetBitLength() const {
    return mBitCapacity;
}

uint32_t OutputMemoryBitStream::GetByteLength() const {
    return (mBitCapacity + 7) >> 3;
}

void OutputMemoryBitStream::WriteBits(uint8_t inData, size_t inBitCount) {
    uint32_t nextBitHead = mBitHead + static_cast<uint32_t>(inBitCount);
    if (nextBitHead > mBitCapacity) {
        ReallocBuffer(std::max(mBitCapacity * 2, nextBitHead));
    }

    // Считаем смещение в байтах и битах в нашем буфере
    uint32_t byteOffset = mBitHead >> 3;
    uint32_t bitOffset = mBitHead & 0x7;

    // Вычисление маски для сохранения использованных битов
    uint8_t currentMask = ~(0xff << bitOffset);

    // Записываем информацию в текущий байт, сохраняя занятые биты
    mBuffer[byteOffset] = (mBuffer[byteOffset] & currentMask) | (inData << bitOffset);

    // Вычисляем количество неиспользуемых бит в текущем байте
    uint32_t bitsFreeThisByte = 8 - bitOffset; 

    // Если бит для записи больше оставшегося, то записываем данные в следующий байт
    if (bitsFreeThisByte < inBitCount) {
        mBuffer[byteOffset + 1] = inData >> bitsFreeThisByte;
    }
    mBitHead = nextBitHead;
}

void OutputMemoryBitStream::WriteBits(const void* inData, size_t inBitCount) {
    const char* srcByte = static_cast<const char*>(inData);
    while (inBitCount > BITS_IN_BYTE_COUNT) {
        WriteBits(*srcByte, BITS_IN_BYTE_COUNT);
        ++srcByte;
        inBitCount -= BITS_IN_BYTE_COUNT;
    }

    if (inBitCount > 0) {
        WriteBits(*srcByte, inBitCount);
    }
}

void OutputMemoryBitStream::ReallocBuffer(uint32_t inNewBitCapacity) {
    if (mBuffer == nullptr) {
        mBuffer = static_cast<char*>(malloc(inNewBitCapacity >> 3));
        memset(mBuffer, 0, inNewBitCapacity >> 3);
    } else {
        char* tempBuffer = static_cast<char*>(std::malloc(inNewBitCapacity >> 3));
        memset(tempBuffer, 0, inNewBitCapacity >> 3);
        memcpy(tempBuffer, mBuffer, mBitCapacity >> 3);
        free(mBuffer);
        mBuffer = tempBuffer;
    }

    assert(mBuffer);
    mBitCapacity = inNewBitCapacity;
}