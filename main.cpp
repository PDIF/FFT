#include <iostream>

#include <fstream>
#include <complex>

#include "Fft.h"
#include "Dft.h"

#include<ctime>
#include "ReferenceSineWave.h"



using namespace std;

int main()
{
    ReferenceSineWave a(96);
    Fft fft(a, {1, 3, 5});

    std::cout << "points per period   = " << a.size() << "\n"
              << "base size           = " << a.base.size() << "\n"
              << "step degree         = " << a.step.Degree() << "\n"
              << "step radian         = " << a.step.Radian() << "\n"
              << "step complex        = " << a.step.Complex() << "\n"
              << "point prime factor  = " << a.convolution.degrees().size() << "\n=====\n";


    for (size_t i = 0; i < a.convolution.degrees().size(); ++i) {

        std::cout << a.convolution.degrees()[i] << "\n";
    };
    std::cout << "\n====\n";


    std::cout << "\n=reference sine wave===\n";
    for (size_t i = 0; i < a.size(); ++i) {

        std::cout << a[i] << "\n";
    };
    std::cout << "\n====\n";

    std::cout << "\n=primary===\n";
    for (size_t i = 0; i < a.base.size(); ++i) {
        std::cout << "\n" << i << ":\n";
        for (size_t j = 0; j < a.base.size() - 1; ++j) {
            std::cout << a.base[i][j] << " ";
        }
    };
    std::cout << "\n====\n";






    std::cout << "step: " << a.base.length() << " " << a.convolution.length() << "\n";


    std::ifstream reading("values");
    std::string line;

    std::vector<double> data;


    while (std::getline(reading, line))
    {
        data.push_back(stod(line));
    };

    reading.close();


    auto startTime = clock();

    for(auto i : data) {
        fft.update(i);
    }

/*
    std::cout << "0 : " << abs(fft.value(0)) << "\n";
    std::cout << "1 : " << abs(fft.value(1)) << "\n";
    std::cout << "2 : " << abs(fft.value(2)) << "\n";
    std::cout << "3 : " << abs(fft.value(3)) << "\n";
    std::cout << "4 : " << abs(fft.value(4)) << "\n";
    std::cout << "5 : " << abs(fft.value(5)) << "\n";
    std::cout << "6 : " << abs(fft.value(6)) << "\n";
    std::cout << "7 : " << abs(fft.value(7)) << "\n";
    std::cout << "9 : " << abs(fft.value(9)) << "\n";
    std::cout << "11: " << abs(fft.value(11)) << "\n";
    std::cout << "12: " << abs(fft.value(12)) << "\n";
    std::cout << "13: " << abs(fft.value(13)) << "\n";
*/
    auto endTime = clock();
    std::cout << "TotalTime FFT: " << endTime - startTime << "\n";

    Dft dft(a);

    startTime = clock();

    for(auto i : data) {
        dft.update(i);
    }

//    std::cout << "dft: " << abs(dft.value()) << "\n";

    endTime = clock();

    std::cout << "TotalTime DFT: " << endTime - startTime << "\n";

    return 0;
}
