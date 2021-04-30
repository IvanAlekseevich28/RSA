#include <iostream>
#include <string>
#include "prime.h"
#include <boost/math/common_factor.hpp>
#include <boost/preprocessor/arithmetic/mod.hpp>
#include <boost/math/special_functions/prime.hpp>
#include <boost/integer/mod_inverse.hpp>
#include "converter.h"

using namespace std;
using namespace boost::math;
using namespace boost::multiprecision;

auto findE(const cpp_int& n)
{
    cpp_int e;
    for ( unsigned i = 0; i < max_prime; i++ )
    {
        e = prime(i);
        if (gcd(e, n) == 1 )
        {
            return e;
        }
    }

    return cpp_int(-1);
}

inline auto findD(const cpp_int& e, const cpp_int& N)
{
    return boost::integer::mod_inverse(e, N);
}

cpp_int encrypt_block(const cpp_int& mes, cpp_int open_key, cpp_int open_e, cpp_int noise = 0)
{
    auto m(mes);
    m ^= noise;

    return powm(m, open_e, open_key);
}

cpp_int decrypt_block(const cpp_int& crp, cpp_int open_key, cpp_int key, cpp_int noise = 0)
{
    auto c(crp);
    c ^= noise;

    return powm(c, key, open_key);
}


int main()
{
    string mes("The general advice is this: Don't invent a new cryptosystem if you actually care about security; or if you must, at least get it publicly peer reviewed by serious cryptography researchers.");
    auto open_blocks = converter::string_to_blocks(mes);

    auto p = math::prime()();
    auto q = math::prime()();
    auto n = p*q;
    auto N = (p-1)*(q-1);
    auto e = findE(N);
    auto d = findD(e, N);


    cout << "openkey:\n" << n << "\n"
         << "e: " << e << "\n"
         << "privatekey:\n" << d << "\n";

    auto crp = encrypt_block(open_blocks[0], n, e);
    auto decrp = decrypt_block(crp, n, d);
    cout << "Message:\n" << converter::blocks_to_string(vector<cpp_int>{decrp}) << endl;

    return 0;
}
