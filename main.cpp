#include <iostream>

//#include <cmath>
#include <complex>

#include "DigitalSignalProcess.h"

#include "RingBuffer.h"
#include "ReferenceSineWave.h"

using namespace std;

int main()
{
    cout << "Hello world!" << endl;

    RingBuffer<double> f(5, 2.0);

    f.push_front(10);
    f.push_front(9);
    f.push_front(8);
    f.push_front(7);
    f.push_front(6);
    f.push_front(5);

    for (int i = 0; i < 4; ++i) {
        std::cout << f[i] << " ";
    }
    std::cout << "\n\n";


    ReferenceSineWave a(96);

    DigitalSignalProcess b(a, {0,1,2,3,4,5,6,7}, 0, 0.25);


    a.update(8);

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

size_t harm = 1;

/*
    b.update(2);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.414214);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(-1.414214);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(-2);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(-1.414214);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.414214);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
*/

/*
    b.update(3);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0.707107);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(-0.707107);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(-3);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(-0.707107);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0.707107);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
*/



    b.update(12);
//std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.7071);
//std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(9);
//std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0.2929);
//std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(6);
//std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0.2929);
//std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(9);
//std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.7071);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(12);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.7071);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(9);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0.2929);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(6);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0.2929);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(9);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.7071);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";

    b.update(12);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.7071);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(9);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0.2929);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(6);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0.2929);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(9);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.7071);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";


b.update(12);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.7071);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(9);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0.2929);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(6);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0.2929);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(9);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.7071);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";


b.update(12);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.7071);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(9);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0.2929);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(6);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0.2929);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(9);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.7071);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";


b.update(12);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.7071);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(9);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0.2929);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(6);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0.2929);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(9);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.7071);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";


b.update(12);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.7071);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(9);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0.2929);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(6);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0.2929);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(9);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.7071);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";

/*


    b.update(2);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.414214);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(-1.414214);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(-2);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(-1.414214);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(0);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
    b.update(1.414214);
std::cout << "current: " << b.value(harm) << " \t" << std::abs(b.value(harm)) << "\n";
*/
    a.update(96);

    return 0;
}
