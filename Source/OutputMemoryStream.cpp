/* 
 * File:   OutputMemoryStream.cpp
 * Author: dontsov
 * 
 * Created on November 24, 2016, 9:43 PM
 */

#include <numeric>
#include <stdlib.h>
#include <algorithm>
#include <string.h>

#include "OutputMemoryStream.hpp"

OutputMemoryStream::OutputMemoryStream()
: mBuffer(nullptr)
, mHead(0)
, mCapacity(0) {
    ReallocBuffer(32);
}

OutputMemoryStream::~OutputMemoryStream() {
}

void OutputMemoryStream::ReallocBuffer(uint32_t inNewLength) {
    mBuffer = static_cast<char*>(realloc(mBuffer, inNewLength));
    if (!mBuffer) {
        // handle realloc failure
        //...
        exit(1);
    }
    mCapacity = inNewLength;
}

void OutputMemoryStream::Write(const void* inData, size_t inByteCount) {
    // make sure we have space
    uint32_t resultHead = mHead + static_cast<uint32_t>(inByteCount);
    if (resultHead > mCapacity) {
        ReallocBuffer(std::max(mCapacity * 2, resultHead));
    }

    memcpy(mBuffer + mHead, inData, inByteCount);
    mHead = resultHead;
}