//
// Created by Ivan Kalesnikau on 21.03.2023.
//

#ifndef FO1_QAM_CONSTELLATIONGENERATOR_HPP
#define FO1_QAM_CONSTELLATIONGENERATOR_HPP

#include "utils.hpp"
#include "GrayCodeGenerator.hpp"

template<uint constellationOrder>
constellation<constellationOrder> generateQAMConstellation()
{
    using namespace std::complex_literals;
    uint numberOfIQPoints = std::pow(2, constellationOrder);
    uint sideSize = std::pow(2, constellationOrder / 2);

    // Расчет iq точек
    std::vector<complex> iqPoints(numberOfIQPoints);
    const double h = 1. / (sideSize - 1);
    for (uint idx = 0; idx < numberOfIQPoints; ++idx)
        iqPoints[idx] = -0.5 + 0.5i - 1.i * h * static_cast<double>(idx % sideSize) +
                        h * static_cast<uint>(idx / sideSize);

    for (uint row = 0; row < sideSize; row += 2)
        std::reverse(std::next(iqPoints.begin(), row * sideSize),
                     std::next(iqPoints.begin(), (1 + row) * sideSize));

    // Подсчет нормы
    double constellationNorm = 0.;
    for (uint idx = 0; idx < numberOfIQPoints; ++idx)
        constellationNorm += std::norm(iqPoints[idx]);
    constellationNorm = std::sqrt(constellationNorm / numberOfIQPoints);

    // Код Грея
    std::vector<std::bitset<constellationOrder>> grayCode = generateGrayCode<constellationOrder>();

    // Итог
    constellation<constellationOrder> resultingConstellation;
    resultingConstellation.reserve(numberOfIQPoints);
    for (uint idx = 0; idx < numberOfIQPoints; ++idx)
        resultingConstellation.insert({grayCode[idx], iqPoints[idx] / constellationNorm});

    return resultingConstellation;
}

template<>
constellation<5> generateQAMConstellation<5>()
{
    using namespace std::complex_literals;
    constellation<5> qam32{{0b01100, 1. + 1.i},
                           {0b01101, 3. + 1.i},
                           {0b11101, 5. + 1.i},
                           {0b01110, 1. + 3.i},
                           {0b11110, 1. + 5.i},
                           {0b11100, 3. + 5.i},
                           {0b11111, 5. + 3.i},
                           {0b01111, 3. + 3.i},
                           {0b01000, 1. + -1.i},
                           {0b01001, 3. + -1.i},
                           {0b11001, 5. + -1.i},
                           {0b01010, 1. + -3.i},
                           {0b11010, 1. + -5.i},
                           {0b11000, 3. + -5.i},
                           {0b11011, 5. + -3.i},
                           {0b01011, 3. + -3.i},
                           {0b00000, -1. + -1.i},
                           {0b00010, -1. + -3.i},
                           {0b10010, -1. + -5.i},
                           {0b00001, -3. + -1.i},
                           {0b10001, -5. + -1.i},
                           {0b10011, -5. + -3.i},
                           {0b10000, -3. + -5.i},
                           {0b00011, -3. + -3.i},
                           {0b00100, -1. + 1.i},
                           {0b00110, -1. + 3.i},
                           {0b10110, -1. + 5.i},
                           {0b00101, -3. + 1.i},
                           {0b10101, -5. + 1.i},
                           {0b10111, -5. + 3.i},
                           {0b10100, -3. + 5.i},
                           {0b00111, -3. + 3.i}
    };
    return qam32;
}


constellation<4> qam16SA()
{
    using namespace std::complex_literals;
    constellation<4> qam16{
            {0b0000, 1. + 1.i},
            {0b0010, 3. + 1.i},
            {0b0001, 1. + 3.i},
            {0b0011, 3. + 3.i},
            {0b0100, 1. - 1.i},
            {0b0110, 3. - 1.i},
            {0b0101, 1. - 3.i},
            {0b0111, 3. - 3.i},
            {0b1000, -1. + 1.i},
            {0b1010, -3. + 1.i},
            {0b1001, -1. + 3.i},
            {0b1011, -3. + 3.i},
            {0b1100, -1. - 1.i},
            {0b1110, -3. - 1.i},
            {0b1101, -1. - 3.i},
            {0b1111, -3. - 3.i}
    };
    // Подсчет нормы
    double constellationNorm = 0.;
    for (auto &[code, iqPoint]: qam16)
        constellationNorm += std::norm(iqPoint);
    constellationNorm = std::sqrt(constellationNorm / 16);
    for (auto &[code, iqPoint]: qam16)
        iqPoint /= constellationNorm;
    return qam16;
}

#endif //FO1_QAM_CONSTELLATIONGENERATOR_HPP
