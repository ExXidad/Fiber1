//
// Created by Ivan Kalesnikau on 21.03.2023.
//

#ifndef FO1_SHAPING_HPP
#define FO1_SHAPING_HPP

#include "utils.hpp"

std::unordered_map<std::bitset<3>, std::bitset<5>> CCDM_lookup{
        {0b000, 0b00011},
        {0b001, 0b00101},
        {0b010, 0b00110},
        {0b011, 0b01001},
        {0b100, 0b01010},
        {0b101, 0b01100},
        {0b110, 0b10001},
        {0b111, 0b10010}
};

std::unordered_map<std::bitset<3>, std::bitset<5>> SS_lookup{
        {0b000, 0b00000},
        {0b001, 0b00001},
        {0b010, 0b00010},
        {0b011, 0b00100},
        {0b100, 0b01000},
        {0b101, 0b10000},
        {0b110, 0b00011},
        {0b111, 0b00101}
};

template<uint constellationOrder, uint lookupFromDim, uint lookupToDim, uint dataSize>
std::bitset<dataSize>
shapeData(const std::bitset<dataSize> &data,
          std::unordered_map<std::bitset<lookupFromDim>, std::bitset<lookupToDim>> &lookupTable)
{
    // Разбиение входных данных на знаковые и амплитудные биты
    std::bitset<dataSize / 2> signBits;
    std::bitset<dataSize / 2> amplitudeBits;
    uint bitIdx = 0;
    for (uint idx = 0; idx < dataSize / constellationOrder; ++idx)
    {
        for (int j = 0; j < constellationOrder / 2; ++j)
        {
            signBits[bitIdx] = data[idx * constellationOrder + j];
            amplitudeBits[bitIdx] = data[idx * constellationOrder + constellationOrder / 2 + j];
            bitIdx++;
        }
    }

    // Разбиение амплитудных битов на блоки по lookupFromDim
    std::array<std::bitset<lookupFromDim>, amplitudeBits.size() / lookupFromDim> subdividedAmplitudeBits;
    for (uint idx = 0; idx < subdividedAmplitudeBits.size(); idx += 1)
        for (int j = 0; j < lookupFromDim; ++j)
            subdividedAmplitudeBits[idx][j] = amplitudeBits[idx * lookupFromDim + j];

    // Преобразование блоков по таблице
    std::array<std::bitset<lookupToDim>, subdividedAmplitudeBits.size()> shapedSubdividedAmplitudeBits;
    for (uint idx = 0; idx < subdividedAmplitudeBits.size(); idx++)
        shapedSubdividedAmplitudeBits[idx] = lookupTable[subdividedAmplitudeBits[idx]];

    // Cборка блоков
    std::bitset<shapedSubdividedAmplitudeBits.size() * lookupToDim> shapedAmplitudeBits;
    for (uint idx = 0; idx < shapedSubdividedAmplitudeBits.size(); ++idx)
        for (uint j = 0; j < lookupToDim; ++j)
            shapedAmplitudeBits[idx * lookupToDim + j] = shapedSubdividedAmplitudeBits[idx][j];

    // Итоговая сборка
    std::bitset<dataSize> result;
    uint shapedAmplitudeBitIdx = 0;
    uint signBitIdx = 0;
    for (uint idx = 0; idx < dataSize / constellationOrder; ++idx)
    {
        for (uint j = 0; j < constellationOrder / 2; ++j)
            result[idx * constellationOrder + j] = signBits[signBitIdx++];
        for (uint j = 0; j < constellationOrder / 2; ++j)
            result[idx * constellationOrder + constellationOrder / 2 +
                   j] = shapedAmplitudeBits[shapedAmplitudeBitIdx++];
    }

    return result;
}


template<uint originalConstellationOrder, uint mappedConstellationOrder>
constellation<originalConstellationOrder> mapConstellation(
        std::unordered_map<std::bitset<originalConstellationOrder>, std::bitset<mappedConstellationOrder>> &lookupTable,
        constellation<mappedConstellationOrder> &mapToConstellation
)
{
    const uint numberOfPoints = std::pow(2, originalConstellationOrder);
    assert(numberOfPoints == lookupTable.size());

    constellation<originalConstellationOrder> mappedConstellation;
    mappedConstellation.reserve(numberOfPoints);
    for (uint i = 0; i < numberOfPoints; ++i)
    {
        const auto[originalData, mappedData] = *std::next(lookupTable.begin(), i);
        mappedConstellation.insert({originalData, mapToConstellation[mappedData]});
    }
    return mappedConstellation;
}

#endif //FO1_SHAPING_HPP
