#ifndef SIGNALGENERATE_H
#define SIGNALGENERATE_H

#include <vector>
#include <array>
#include <math.h>
#include <cassert>

class SignalGenerate
{

    enum class Location: size_t {
        FREQUENCY   ,
        AMPLITUDE   ,
        PHASE_LAMBDA,
        SIZE //The SIZE parameter should be placed last in enum class
    };
    static constexpr double PI_SINGLE = acos(-1.0);
    static constexpr double PI_TWICE  = PI_SINGLE * 2;
    static constexpr double F  = 50.0;

public:
    SignalGenerate();
    virtual ~SignalGenerate();





    std::vector<double> get();
    std::vector<double> get(double inputFrequency);

    //std::array <double, static_cast<size_t>(Location::SIZE)> parameters(size_t);

private:

    void addSinusoidalData (std::vector<double>& data,
                            double amplitude,  double frequency, double phase);

    void addExponentialData(std::vector<double>& data,
                            double amplitude, double lambda);

    std::vector<double> getData(size_t position);
    std::vector<double> getData(double frequency);


    double start_ ;
    double step_  ;

    size_t size_  ;


    std::vector<std::array<double, static_cast<size_t>(Location::SIZE)>> data_;
};

#endif // SIGNALGENERATE_H
