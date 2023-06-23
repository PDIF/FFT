#include <iostream>

#include <fstream>
#include <complex>

#include "Fft.h"
#include "Dft.h"
#include "Rdft.h"

#include<ctime>
#include "ReferenceSineWave.h"

#include "BaseSineWave.h"


using namespace std;

int main()
{
auto startTime = clock();

    std::ifstream reading("values 1");
    std::string   line;
    std::vector<double> data;


    while (std::getline(reading, line))
    {
        data.push_back(stod(line));
    };
    reading.close();

auto endTime = clock();
std::cout << "Reading time: " << endTime - startTime << "\n";

    //std::ofstream writing("result");


startTime = clock();

    ReferenceSineWave a(96);

    BaseSineWave bbb(8);
    BaseSineWave ccc(96);

    RingBuffer<std::complex<double>> storeData(96);

endTime = clock();
std::cout << "Preparing sine wave time: " << endTime - startTime << "\n";

startTime = clock();
    //Fft fft(a, {1, 2, 3, 4, 5, 6, 7, 9, 11, 12, 13});
    Fft fft(&bbb, {1});
    Dft dft(&bbb, {1, 2, 3});
    Rdft rdft(&bbb);
    //Rdft rdft;
    //bbb.update(80);
    //rdft.setNewBase(&ccc);
    //dft.setNewBase(&ccc);

/*
    for(auto i : data) {
        fft.update(i);
        dft.update(i);
        rdft.update(i);

        //std::cout << fft.value(1) << "\t" << dft.value() << "\t" << rdft.value() << "\n";

        //writing << abs(fft.value(1)) << "\t" << abs(dft.value()) << "\t" << abs(rdft.value()) << "\n";
    }
    //writing.close();
*/

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

    /*
    rdft.setNewBase(&bbb);
    for(auto i : data) {
        rdft.update(i);
    }
    std::cout << "rdft :\t" << rdft.getData(harmonic) << "\t" << abs(rdft.getData(harmonic)) << "\n";

    */
endTime = clock();
std::cout << "TotalTime RDFT: " << endTime - startTime << "\n\n";



    std::cout << "BBBB: " << BaseSineWave::Pi::deg << "\n";
/*

    ReferenceSineWave a(96);
    Fft fft(a, {1, 3, 5});

    auto startTime = clock();

    for(auto i : data) {
        fft.update(i);
        writing << fft.value(1) << "\n";
    }
    writing.close();


    std::cout << "fft :\t" << fft.value(1) << "\t" << abs(fft.value(1)) << "\n";


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

    auto endTime = clock();
    std::cout << "TotalTime FFT: " << endTime - startTime << "\n";

    Dft dft(a);

    startTime = clock();

    for(auto i : data) {
        dft.update(i);
    }

    std::cout << "dft :\t" << dft.value() << "\t" << abs(dft.value()) << "\n";

    endTime = clock();

    std::cout << "TotalTime DFT: " << endTime - startTime << "\n";


    Rdft rdft(a);

    startTime = clock();

    for(auto i : data) {
        rdft.update(i);
    }

    std::cout << "rdft :\t" << rdft.value() << "\t" << abs(rdft.value()) << "\n";

    endTime = clock();

    std::cout << "TotalTime RDFT: " << endTime - startTime << "\n";

*/
    return 0;
}
