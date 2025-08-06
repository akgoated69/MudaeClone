#include <string>

bool containsNumber(const std::string& s) {
    return s.find_first_of("0123456789") != std::string::npos;
}