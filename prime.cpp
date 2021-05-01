#include "prime.h"

using namespace math;
using namespace boost::multiprecision;
using namespace boost::random;
using namespace std;

prime::prime() :
    m_prime(0),
    m_count_concurrency(thread::hardware_concurrency() - 1),
    m_gen(mt11213b(clock()))
{

}

void prime::thread_search_prime()
{
    cpp_int n;
    do
    {
        n = this->random();
    } while (!miller_rabin_test(n, 25) and !m_prime);
    if (!m_prime)
        m_prime = n;
}

cpp_int prime::random()
{
    cpp_int n;
    m_mutex.lock();
    n = (m_gen)();
    m_mutex.unlock();

    return n;
}

cpp_int prime::operator()()
{
    m_prime = 0;

    for (unsigned i = 0; i < m_count_concurrency; i++)
        m_arr_threads.push_back(thread(&prime::thread_search_prime, this));

    for (auto& th : m_arr_threads)
        th.join();

    m_arr_threads.clear();

    return m_prime;
}
