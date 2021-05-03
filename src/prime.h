#pragma once

#include <thread>
#include <mutex>
#include <memory>
#include <vector>
#include <boost/random/mersenne_twister.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include "common.h"

typedef boost::random::independent_bits_engine<boost::random::mt11213b, BITS, boost::multiprecision::cpp_int> GEN_ENG;

namespace math
{
class prime
{
public:
    prime();
    boost::multiprecision::cpp_int operator()();
    boost::multiprecision::cpp_int random();
private:
    void thread_search_prime();

private:
    boost::multiprecision::cpp_int m_prime;
    unsigned m_count_concurrency;
    std::vector<std::thread> m_arr_threads;
    std::mutex m_mutex;
    GEN_ENG m_gen;
};
}


