/* 
 * File:   InputMemoryStream.cpp
 * Author: dontsov
 * 
 * Created on November 26, 2016, 1:27 PM
 */

#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "InputMemoryStream.hpp"

const uint32_t InputMemoryStream::MAX_BUFFER_SIZE = 1470;

InputMemoryStream::InputMemoryStream(char* inBuffer, uint32_t inByteCount)
: mBuffer(inBuffer)
, mCapacity(inByteCount)
, mHead(0) {}

InputMemoryStream::InputMemoryStream() 
: mBuffer(nullptr)
, mCapacity(MAX_BUFFER_SIZE)
, mHead(0) {
    mBuffer = static_cast<char*>(malloc(mCapacity));
    assert(mBuffer);
}

InputMemoryStream::~InputMemoryStream() {
    memset(mBuffer, 0, mCapacity);
    free(mBuffer);
}

uint32_t InputMemoryStream::GetRemainingDataSize() const {
    return mCapacity - mHead;
}

void InputMemoryStream::Read(void* outData, uint32_t inByteCount) {
    bool isMemoryEnough = GetRemainingDataSize() > inByteCount;
    assert(isMemoryEnough);
    
    memcpy(outData, mBuffer + mHead, inByteCount);
    mHead += inByteCount;
}

char* InputMemoryStream::GetBuffer() {
    return mBuffer;
}

void InputMemoryStream::Clear() {
    if (mBuffer) {
        memset(mBuffer, 0, mCapacity);
        mHead = 0;
    }
}