#include <bitset>

// QPSK, 16QAM, 64QAM
#include "GrayCodeGenerator.hpp"
#include "QAM_ConstellationGenerator.hpp"
#include "ModulateDemodulate.hpp"
#include "Noise.hpp"
#include "Shaping.hpp"


int main()
{
    std::fstream file;
    std::mt19937 randomEngine;
    constexpr uint constellationOrder = 4;
    constellation<constellationOrder> qamConstellation = generateQAMConstellation<constellationOrder>();

    std::cout << "-------" << std::endl;

    constexpr uint dataSize = 1048576 / 2 * 3;
    std::bitset<dataSize> data = randomBitset<dataSize>(randomEngine);;
    std::vector<iq> iqPoints = modulate<dataSize, constellationOrder>(data, qamConstellation);


    std::bitset<dataSize> demodTestData = demodulate<dataSize / constellationOrder, constellationOrder>(iqPoints,
                                                                                                        qamConstellation);
    assert(data == demodTestData);
    assert(std::fabs(snr2sigma(sigma2snr(1)) - 1.) <= 1e-15);

    constellation<4> qam16Constellation = qam16SA();//generateQAMConstellation<4>();
    file.open("../iq.txt", std::ios::out);
    for (auto &[code, iqPoint]: qam16Constellation)
        file << code.to_string() << "\t" << iqPoint.real() << "\t" << iqPoint.imag() << std::endl;
    file.close();
    std::bitset<dataSize> CCDM_data = shapeData<4, 3, 5, dataSize>(data, CCDM_lookup);
    std::bitset<dataSize> SS_data = shapeData<4, 3, 5, dataSize>(data, SS_lookup);

    file.open("../ss_data.txt", std::ios::out);
    for (uint idx = 0; idx<dataSize; idx++)
        file << SS_data[idx];
    file.close();

    // ---------------------------

    iqPoints = modulate<dataSize, 4>(CCDM_data, qam16Constellation);
    file.open("../iq_CCDM.txt", std::ios::out);
    for (auto &iqPoint: iqPoints)
    {
        file << iqPoint.real() << "\t" << iqPoint.imag() << std::endl;
    }
    file.close();
    file.open("../ber_QAM16_CCDM.txt", std::ios::out);
    for (double snr = 0; snr < 40; snr += 2)
    {
        std::vector<iq> noisedIQPoints = noiseIQPoints(randomEngine, iqPoints, snr, 1);
        std::bitset<dataSize> demodData = demodulate<dataSize / 4, 4>(noisedIQPoints, qam16Constellation);
        file << snr << "\t" << estimateBER<dataSize>(demodData, CCDM_data) << std::endl;
    }
    file.close();

    // ---------------------------

    iqPoints = modulate<dataSize, 4>(SS_data, qam16Constellation);
    file.open("../iq_SS.txt", std::ios::out);
    for (auto &iqPoint: iqPoints)
    {
        file << iqPoint.real() << "\t" << iqPoint.imag() << std::endl;
    }
    file.close();

    file.open("../ber_QAM16_SS.txt", std::ios::out);
    for (double snr = 0; snr < 40; snr += 2)
    {
        std::vector<iq> noisedIQPoints = noiseIQPoints(randomEngine, iqPoints, snr, 1);
        std::bitset<dataSize> demodData = demodulate<dataSize / 4, 4>(noisedIQPoints, qam16Constellation);
        file << snr << "\t" << estimateBER<dataSize>(demodData, SS_data) << std::endl;
    }
    file.close();

    // ---------------------------

    iqPoints = modulate<dataSize, 4>(data, qam16Constellation);
    file.open("../ber_QAM16.txt", std::ios::out);
    for (double snr = 0; snr < 40; snr += 2)
    {
        std::vector<iq> noisedIQPoints = noiseIQPoints(randomEngine, iqPoints, snr, 1);
        std::bitset<dataSize> demodData = demodulate<dataSize / 4, 4>(noisedIQPoints, qam16Constellation);
        file << snr << "\t" << estimateBER<dataSize>(demodData, data) << std::endl;
    }
    file.close();


    return 0;
}
