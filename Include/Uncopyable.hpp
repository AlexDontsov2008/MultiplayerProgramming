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
    virtual ~Uncopyable() {}

private:
    Uncopyable(const Uncopyable&);
    Uncopyable& operator=(const Uncopyable&);
};

#endif /* UNCOPYABLE_HPP */