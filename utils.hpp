//
// Created by Ivan Kalesnikau on 21.03.2023.
//

#ifndef FO1_UTILS_HPP
#define FO1_UTILS_HPP

#include <iostream>
#include <complex>
#include <bitset>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <ios>
#include <fstream>
#include <array>

using complex = std::complex<double>;
using iq = complex;

template<uint constellationOrder>
using constellation = std::unordered_map<std::bitset<constellationOrder>, iq>;

template<uint n, typename randomEngineType>
std::bitset<n> randomBitset(randomEngineType &randomEngine)
{
    std::uniform_int_distribution dist(0, 1);
    std::bitset<n> bitset;
    for (uint i = 0; i < n; ++i)
        bitset[i] = dist(randomEngine);
    return bitset;
}

template<uint n>
double estimateBER(const std::bitset<n> &b1, const std::bitset<n> &b2)
{
    uint errSum = 0;
    for (uint i = 0; i < n; ++i)
        errSum += b1[i] != b2[i];
    return static_cast<double>(errSum) / static_cast<double>(n);
}

#endif //FO1_UTILS_HPP
