#include <iostream>		// std::string
#include <algorithm>	// count_if, isalpha

bool strIsAlpha(const std::string& s) {
    return (unsigned int) count_if(s.begin(), s.end(), [](unsigned char c){ return isalpha(c); }) == (unsigned int) strlen(s.c_str());
}