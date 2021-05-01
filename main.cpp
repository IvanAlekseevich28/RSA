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

struct s_open_key
{
    cpp_int n;
    cpp_int exp;
    cpp_int init_vec;
};

struct s_key
{
    cpp_int key;
    s_open_key open;
};

ostream& operator<<(ostream& os, const s_open_key& ok)
{
    os << ok.exp << SPLITER
       << converter::cpp_int_to_str64(ok.n) << SPLITER
       << converter::cpp_int_to_str64(ok.init_vec) << SPLITER;

    return os;
}

ostream& operator<<(ostream& os, const s_key& k)
{
    os << k.open
       << converter::cpp_int_to_str64(k.key) << SPLITER;

    return os;
}

istream& operator>>(istream& is, s_open_key& ok)
{
    string  str64_n, str64_init_vec;
    is >> ok.exp >> str64_n >> str64_init_vec;

    ok.exp = converter::str64_to_cpp_int(str64_n);
    ok.exp = converter::str64_to_cpp_int(str64_init_vec);

    return is;
}

istream& operator>>(istream& is, s_key& key)
{
    string str64_key;
    is >> key.open >> str64_key;

    key.key = converter::str64_to_cpp_int(str64_key);

    return is;
}

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

s_key key_gen()
{
    s_key key;
    math::prime pg;

    auto p = pg();
    auto q = pg();
    auto N = (p-1)*(q-1);

    key.open.n = p*q;
    key.open.exp = findE(N);
    key.open.init_vec = pg.random();

    key.key = findD(key.open.exp, N);

    return key;
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

vector<cpp_int> encrypt_CBC(const std::string& mes, const s_open_key& open_key)
{
    vector<cpp_int> crp_blocks;
    auto open_blocks = converter::string_to_blocks(mes);
    auto crp_block = open_key.init_vec;
    for (const auto& ob : open_blocks)
    {
        crp_block = encrypt_block(ob, open_key.n, open_key.exp , crp_block);
        crp_blocks.push_back(crp_block);
    }

    return crp_blocks;
}

std::string decrypt_CBC(const vector<cpp_int>& crp_blocks, const s_key& key)
{
    string mes;
    auto crp_block = key.open.init_vec;
    for (const auto& cb : crp_blocks)
    {
        auto open_block = decrypt_block(cb, key.open.n, key.key, crp_block);
        crp_block = cb;

        mes += converter::block_to_string(open_block);
    }

    return mes;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cerr << "Uncorrect count of args!\n"
             << "This is demo version!\n";

        return 1;
    }
    string mes = argv[1];
    cout << "Programm works in demo mode."
         << "Generation of keys... It takes around 1 minute.\n";

    auto key = key_gen();

    cout << key;

    auto crp = encrypt_CBC(mes, key.open);
    auto decrp = decrypt_CBC(crp, key);
    cout << "Crypt:\n" << crp << endl;
    cout << "Message:\n" << decrp << endl;

    return 0;
}
