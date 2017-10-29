#include <string> // std::string
#include <iostream> // std::cout
#include <sstream> // std::ostringstream

template <typename T>
std::string to_string (T number) {
	std::string result;
	std::ostringstream convert;
	convert << number;
	result = convert.str();
	return result;
}