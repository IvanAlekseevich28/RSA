#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>
#include <boost/multiprecision/cpp_int.hpp>
#include <vector>
#include "common.h"
#include <ostream>

namespace converter
{
    std::vector<boost::multiprecision::cpp_int> string_to_blocks(const std::string& message);
    std::string blocks_to_string(const std::vector<boost::multiprecision::cpp_int>& blocks);
}

std::ostream& operator<<(std::ostream& os, const std::vector<boost::multiprecision::cpp_int>& blocks);
#endif // CONVERTER_H
