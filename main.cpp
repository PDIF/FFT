#include <iostream>

#include <fstream>
#include <complex>
#include <ctime>

#include "Fft.h"
#include "Dft.h"
#include "Rdft.h"
#include "BaseSineWave.h"

int main()
{

    //1. Reading data from file
    auto startTime = clock();

    std::ifstream       reading("TestData");
    std::string         line;
    std::vector<double> instant;

    while (std::getline(reading, line)) {
        instant.push_back(stod(line));
    };
    reading.close();
    auto endTime = clock();
    std::cout << "Reading data time: " << endTime - startTime << "\n\n";


    //2. Preparing objects time
    startTime = clock();

    size_t harmonic = 1;
    size_t steps    = 96;
    std::vector<size_t> harmonicsSet{1, 2, 3};

    BaseSineWave baseSineWave(steps);

    Dft  dft(&baseSineWave, harmonicsSet);
    Fft  fft(&baseSineWave, harmonicsSet);
    Rdft rdft(&baseSineWave, harmonicsSet);

    endTime = clock();
    std::cout << "Preparing objects time: " << endTime - startTime << "\n\n";

    //3. Information process Dft
    startTime = clock();
    for(auto& value : instant) {
        dft.update(value);
    };

    endTime = clock();
    std::cout << "DFT values:\t" << dft.getData(harmonic) <<
                 "\t" << abs(dft.getData(harmonic)) << "\n";
    std::cout << "DFT time: " << endTime - startTime << "\n\n";


    //4. Information process Fft
    startTime = clock();
    for(auto& value : instant) {
        fft.update(value);
    };

    endTime = clock();
    std::cout << "FFT values:\t" << fft.getData(harmonic) <<
                 "\t" << abs(fft.getData(harmonic)) << "\n";
    std::cout << "FFT time: " << endTime - startTime << "\n\n";


    //5. Information process Rdft
    startTime = clock();
    for(auto& value : instant) {
        rdft.update(value);
    };

    endTime = clock();
    std::cout << "RDFT values:\t" << rdft.getData(harmonic) <<
                 "\t" << abs(rdft.getData(harmonic)) << "\n";
    std::cout << "RDFT time: " << endTime - startTime << "\n\n";


    //6. The usual DFT 1st harmonic for comparison
    double pi = std::acos(-1.0);
    std::complex<double>              one{1.0, 0.0};
    std::complex<double>              result{0.0, 0.0};
    std::vector<std::complex<double>> base(steps, one);
    std::vector<double>               valuesData(steps, 0.0);

    for (size_t i = 1; i < steps; ++i) {
        std::complex<double> power{0.0, 2.0 * pi * i / steps};
        base[i] = std::exp(power);
    };

    startTime = clock();

    for (auto& value: instant) {
        result        = valuesData[0];
        valuesData[0] = valuesData[1];

        for (size_t i = 1; i < steps; ++i) {
            result += valuesData[i] * base[steps - i];
            valuesData[i] = valuesData[i + 1];
        };

        valuesData[steps - 1] = value;
    };
    endTime = clock();

    std::cout << "Usual DFT values:\t" << result * (2.0 / steps) <<
                 "\t" << abs(result * (2.0 / steps)) << "\n";
    std::cout << "Usual DFT time: " << endTime - startTime << "\n\n";


    return 0;
}
