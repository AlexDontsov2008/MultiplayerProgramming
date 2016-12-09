/*
 * File:   BinaryNumber.hpp
 * Author: dontsov
 *
 * Created on December 3, 2016, 10:17 AM
 */

#ifndef BINARYNUMBER_HPP
#define BINARYNUMBER_HPP

#include <algorithm>

class BinaryNumber {
public:
    explicit BinaryNumber(uint32_t number = 0)
    : mShowBitsNumber(0)
    , mBinary(TranslateToBinary(number)) {}

    void Print() const {
        std::cout << mBinary << std::endl;
    }

    void SetNumber(uint32_t number) {
        mBinary = TranslateToBinary(number);
    }

private:
    uint8_t GetShowBitsNumber(uint32_t number) {
        uint64_t currNumber = 1;
        uint8_t showBitsNumber = 1;
        while (currNumber < number) {
            currNumber *= 2;
            showBitsNumber += 1;
        }
        return showBitsNumber;
    }

    std::string TranslateToBinary(uint32_t number) {
        std::string binary;
        mShowBitsNumber = GetShowBitsNumber(number);
        for (uint8_t offset = 0; offset < mShowBitsNumber; ++offset) {
            char bitValue = ((number >> offset) & 1) ? '1' : '0';
            binary += bitValue;
        }
        std::reverse(binary.begin(), binary.end());
        return binary;
    }

    uint8_t mShowBitsNumber;
    std::string mBinary;
};


#endif /* BINARYNUMBER_HPP */