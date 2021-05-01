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
    for ( unsigned i = 1; i < max_prime; i++ )
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
    cpp_int c = powm(crp, key, open_key);
    c ^= noise;

    return c;
}

vector<cpp_int> encrypt_CBC(const std::string& mes, const cpp_int& open_key, const cpp_int& exp)
{
    vector<cpp_int> crp_blocks;
    auto open_blocks = converter::string_to_blocks(mes);
    auto crp_block = open_key;
    for (const auto& ob : open_blocks)
    {
        crp_block = encrypt_block(ob, open_key, exp , crp_block);
        crp_blocks.push_back(crp_block);
    }

    return crp_blocks;
}

std::string decrypt_CBC(const vector<cpp_int>& crp_blocks, const cpp_int& open_key, const cpp_int& key)
{
    string mes;
    auto crp_block = open_key;
    for (const auto& cb : crp_blocks)
    {
        auto open_block = decrypt_block(cb, open_key, key, crp_block);
        crp_block = cb;

        mes += converter::block_to_string(open_block);
    }

    return mes;
}

int main()
{
    string mes("The general advice is this: Don't invent a new cryptosystem if you actually care about security; or if you must, at least get it publicly peer reviewed by serious cryptography researchers. \nThat said, the best I've got is that this sounds like you're treating RSA as a kind of block cipher, and so you probably want to use a standard block cipher mode. The obvious thing to do, of just encrypting each block separately with the same key, sounds equivalent to Electronic Code-Book (ECB) mode, which is not secure. Perhaps you can use one of the better modes like CBC with RSA?");
//    string mes("Don't invent a new cryptosystem if you actually care about security");
    auto p = math::prime()();
    auto q = math::prime()();
    auto n = p*q;
    auto N = (p-1)*(q-1);
    auto e = findE(N);
    auto d = findD(e, N);


    cout << "openkey:\n" << converter::cpp_int_to_str64(n) << "\n"
         << "e: " << e << "\n"
         << "privatekey:\n" << converter::cpp_int_to_str64(d) << "\n";

    auto crp = encrypt_CBC(mes, n, e);
    auto decrp = decrypt_CBC(crp, n, d);
    cout << "Message:\n" << decrp << endl;

    return 0;
}
