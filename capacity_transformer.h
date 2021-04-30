#pragma once

#include <string>
#include <cmath>

class CapacityTransformer
{
public:
    CapacityTransformer(const std::string& alf) : m_alf(alf)
    {
    }

    std::string operator()(const std::string &input, unsigned fromCap, unsigned toCap)const
    {
        std::string output;
        if (fromCap == toCap)
            return input;

        for (unsigned i = 0; i < input.size(); i++)
        {
            if (fromCap > toCap)
            {
                output += transformLetter(&input[i], fromCap, toCap);
            } else
            {
                unsigned digitsCount = getDigitCount(fromCap, toCap);
                std::string block(&(input[i]), (&(input[i])) + digitsCount);
                i += digitsCount - 1;
                output += transformLetter(block, fromCap, toCap);
            }
        }


        return output;
    }

private:
    std::string transformLetter(std::string input, unsigned fromCap, unsigned toCap)const
    {
        std::string output;
        const int dc = getDigitCount(fromCap, toCap);
        if (fromCap > toCap)
        {
            const unsigned letPos = m_alf.find(input[0]);
            for (int i = dc; i > 0; i--)
            {
                const unsigned highDigit = (unsigned)pow(toCap, i);
                const unsigned lowDigit = (unsigned)pow(toCap, i - 1);
                const unsigned transformedPos = (letPos % highDigit) / lowDigit;
                output.push_back(m_alf[transformedPos]);
            }
        } else
        {
            unsigned neededLetPos = 0;
            for (int i = 0; i < input.size(); i++)
            {
                const unsigned power = input.size() - i - 1;
                const unsigned letPos = m_alf.find(input[i]);
                const unsigned digitPow = (unsigned)pow(fromCap, power);
                neededLetPos += letPos * digitPow;

            }
            output.push_back(m_alf[neededLetPos]);
        }

        return output;
    }
    inline unsigned getDigitCount(unsigned fromCap, unsigned toCap)const
    {
        if (fromCap > toCap)
            return (unsigned)ceil(pow(fromCap, 1.0 / toCap));
        else
            return (unsigned)ceil(pow(toCap, 1.0 / fromCap));
    }

private:
    std::string m_alf;

};
