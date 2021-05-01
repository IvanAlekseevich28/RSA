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
    std::string block_to_string(const boost::multiprecision::cpp_int& block);

    std::string cpp_int_to_str64(const boost::multiprecision::cpp_int& key);
    boost::multiprecision::cpp_int str64_to_cpp_int(const std::string& str);
}

std::ostream& operator<<(std::ostream& os, const std::vector<boost::multiprecision::cpp_int>& blocks);
#endif // CONVERTER_H
