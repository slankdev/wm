

#pragma once

#include <exception>
#include <sstream>
#include <ostream>
#include <string>


class wmexception : public std::exception {
private:
    std::string str;
public:
    explicit wmexception(const char* s="") noexcept 
    {
        str = s; 
    } 
    template<class T>
    exception& operator<<(const T& t) noexcept 
    {
        std::ostringstream os;
        os << " " << t ;
        str += os.str();
        return *this;
    }
    const char* what() const noexcept
    {
        return str.c_str(); 
    }
};
