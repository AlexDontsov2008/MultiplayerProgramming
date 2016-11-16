/* 
 * File:   Uncopyable.hpp
 * Author: dontsov
 *
 * Created on November 16, 2016, 10:08 PM
 */

#ifndef UNCOPYABLE_HPP
#define UNCOPYABLE_HPP

class Uncopyable {
protected:
    Uncopyable() {}
    ~Uncopyable() {}

private:
    Uncopyable(const Uncopyable&) = delete;
    Uncopyable& operator=(const Uncopyable&) = delete;
};

#endif /* UNCOPYABLE_HPP */