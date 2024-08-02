#include "logger.hpp"

void Log(const std::string&& s) {
 #ifndef NDEBUG
    std::cout << s;
 #endif
}

void LogLine(const std::string&& s) {
 #ifndef NDEBUG
    std::cout << s << std::endl;
 #endif
}