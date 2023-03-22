//
// Created by Ivan Kalesnikau on 21.03.2023.
//

#ifndef FO1_NOISE_HPP
#define FO1_NOISE_HPP

#include "utils.hpp"

double snr2sigma(const double snr)
{
    return std::sqrt(std::pow(10., -snr / 10.)) / M_SQRT2;
}

double sigma2snr(const double sigma)
{
    return -4.3429448190325175 * std::log(std::pow(sigma * M_SQRT2, 2));
}

template<typename randomEngineType>
iq noiseIQPoint(randomEngineType &randomEngine, const iq iqPoint, const double snr, const double ebno = 1)
{
    using namespace std::complex_literals;

    std::normal_distribution<double> dist(0., snr2sigma(snr + 10. * std::log10(ebno)));
    return iqPoint + dist(randomEngine) + 1i * dist(randomEngine);
}

template<typename randomEngineType>
std::vector<iq> noiseIQPoints(randomEngineType &randomEngine, const std::vector<iq> &iqPoints, const double snr,
                              const double ebno = 1)
{
    std::vector<iq> noisedIQPoints(iqPoints.size());
    for (uint i = 0; i < iqPoints.size(); ++i)
        noisedIQPoints[i] = noiseIQPoint(randomEngine, iqPoints[i], snr, ebno);
    return noisedIQPoints;
}


#endif //FO1_NOISE_HPP
