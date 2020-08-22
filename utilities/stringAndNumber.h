#ifndef THIRDPARTY_STRINGANDNUMBER_H
#define THIRDPARTY_STRINGANDNUMBER_H


#include <string>
#include <sstream>
#include <iomanip>


template<typename T>
std::string toString(T val) {
    std::ostringstream oss;
    oss << val;
    return oss.str();
}


template<typename T>
std::string toString(T val, const int &widthCellOut, const int &prec,
                     const bool &scientificFlag) {

    std::ostringstream oss;

    oss.precision(prec);

    if (scientificFlag)
        oss.setf(std::ios::scientific, std::ios::floatfield);
    else
        oss.setf(std::ios::fixed, std::ios::floatfield);

    oss << std::setw(widthCellOut) << val;

    return oss.str();

}


template<typename T>
T fromString(const std::string &s) {
    std::istringstream iss(s);
    T res;
    iss >> res;
    return res;
}


/*
std::string str;

int iVal;

float fVal;


str = toString(iVal);

str = toString(fVal);


iVal = fromString<int>(str);

fVal = fromString<float>(str);
*/

#endif // THIRDPARTY_STRINGANDNUMBER_H
