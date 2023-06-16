#include <iostream>


//#include <istream>
#include <fstream>

//#include <cmath>
#include <complex>

#include "DigitalSignalProcess.h"

#include "RingBuffer.h"
#include "ReferenceSineWave.h"

using namespace std;

int main()
{
    ReferenceSineWave a(96);

    DigitalSignalProcess b(a);

    std::cout << "points per period   = " << a.getPointsPerPeriod() << "\n"
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
    for (size_t i = 0; i < a.getPointsPerPeriod(); ++i) {

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

    for(auto i : data) {
        b.update(i);
    }

    std::cout << "0 : " << abs(b.value(0)) << "\n";
    std::cout << "1 : " << abs(b.value(1)) << "\n";
    std::cout << "2 : " << abs(b.value(2)) << "\n";
    std::cout << "3 : " << abs(b.value(3)) << "\n";
    std::cout << "4 : " << abs(b.value(4)) << "\n";
    std::cout << "5 : " << abs(b.value(5)) << "\n";
    std::cout << "6 : " << abs(b.value(6)) << "\n";
    std::cout << "7 : " << abs(b.value(7)) << "\n";
    std::cout << "9 : " << abs(b.value(9)) << "\n";
    std::cout << "11: " << abs(b.value(11)) << "\n";
    std::cout << "12: " << abs(b.value(12)) << "\n";
    std::cout << "13: " << abs(b.value(13)) << "\n";

    return 0;
}
