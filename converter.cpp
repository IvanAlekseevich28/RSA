#include "converter.h"

using namespace boost::multiprecision;
using namespace std;

static cpp_int chars_to_block(const char cstr[]);
static string block_to_string(const cpp_int& block);

static const string alf = "+/0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

namespace converter
{
vector<cpp_int> string_to_blocks(const string& message)
{
    vector<cpp_int> blocks;
    string str_block;
    str_block.reserve(CSTR_BLOCK_LEN);

    for (unsigned i = 0; i < message.size(); i++)
    {
        str_block.push_back(message[i]);
        if (str_block.size() == CSTR_BLOCK_LEN)
        {
            blocks.push_back(chars_to_block(str_block.c_str()));
            str_block.clear();
        }
    }
    if (str_block.size() != 0)
    {
        for (int i = str_block.size(); i != 0; i--)
            str_block.push_back('\0');

        blocks.push_back(chars_to_block(str_block.c_str()));
    }


    return blocks;
}

string blocks_to_string(const vector<cpp_int>& blocks)
{
    string str;
    for (const auto& b : blocks)
        str += block_to_string(b);

    return str;
}

string cpp_int_to_str64(const cpp_int& key)
{
    string str64;
    auto b = key;
    for (unsigned i = 0; i < CSTR_BLOCK_LEN; i++)
    {
        str64.push_back(alf[static_cast<int>(b % cpp_int(64))]);
        b >>= 8;
    }

    return str64;
}

cpp_int str64_to_cpp_int(const string& str)
{
    cpp_int block(0);
    for (unsigned i = 0; i < str.size(); i++)
    {
        auto pos = alf.find(str[i]);
        if (pos == -1) pos = 0;
        block += (cpp_int)alf[pos] << (i * 6);
    }

    return block;
}

string block_to_string(const cpp_int& block)
{
    string str;
    auto b = block;
    for (unsigned i = 0; i < CSTR_BLOCK_LEN; i++)
    {
        str.push_back(static_cast<char>(b % cpp_int(0x100)));
        b >>= 8;
    }

    return str;
}
}

static cpp_int chars_to_block(const char cstr[])
{
    cpp_int block(0);
    for (unsigned i = 0; cstr[i] != '\0'; i++)
        block += (cpp_int)cstr[i] << (i * 8);

    return block;
}



ostream& operator<<(ostream& os, const vector<cpp_int>& blocks)
{
    for (const auto& b : blocks)
        os << b << " ";

    return os;
}
