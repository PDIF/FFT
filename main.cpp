#include <iostream>

#include <fstream>
#include <complex>

#include "Fft.h"
#include "Dft.h"
#include "Rdft.h"

#include<ctime>

#include "BaseSineWave.h"


using namespace std;

int main()
{
auto startTime = clock();

    std::ifstream reading("values");
    std::string   line;
    std::vector<double> data;


    while (std::getline(reading, line))
    {
        data.push_back(stod(line));
    };
    reading.close();

auto endTime = clock();
std::cout << "Reading time: " << endTime - startTime << "\n";


    BaseSineWave baseSineWave(96);

std::cout << "Preparing sine wave time: " << endTime - startTime << "\n";

startTime = clock();

    Fft  fft(&baseSineWave, {0, 1, 2, 3, 5});
    Dft  dft(&baseSineWave, {1});
    Rdft rdft(&baseSineWave);


/*
    for(auto i : data) {
        fft.update(i);
        dft.update(i);
        rdft.update(i);

        //std::cout << fft.value(1) << "\t" << dft.value() << "\t" << rdft.value() << "\n";

        //writing << abs(fft.value(1)) << "\t" << abs(dft.value()) << "\t" << abs(rdft.value()) << "\n";
    }

    */

    //writing.close();

    //std::swap(bbb, ccc);

    //std::cout << abs(fft.value(1)) << "\t" << abs(dft.value()) << "\t" << abs(rdft.value()) << "\n";

endTime = clock();
std::cout << "Preparing objects time: " << endTime - startTime << "\n\n";


    size_t harmonic = 1;


startTime = clock();

    for(auto i : data) {
        fft.update(i);
    }
    std::cout << "fft :\t" << fft.getData(harmonic) << "\t" << abs(fft.getData(harmonic)) << "\n";

endTime = clock();

std::cout << "TotalTime FFT: " << endTime - startTime << "\n\n";


startTime = clock();

    for(auto i : data) {
        dft.update(i);
    }
    std::cout << "dft :\t" << dft.getData(harmonic) << "\t" << abs(dft.getData(harmonic)) << "\n";
endTime = clock();
std::cout << "TotalTime DFT: " << endTime - startTime << "\n\n";


startTime = clock();
    for(auto i : data) {
        rdft.update(i);
    }
    std::cout << "rdft :\t" << rdft.getData(harmonic) << "\t" << abs(rdft.getData(harmonic)) << "\n";

endTime = clock();
std::cout << "TotalTime RDFT: " << endTime - startTime << "\n\n";



    std::cout << "BBBB: " << BaseSineWave::Pi::deg << "\n";

    return 0;
}
