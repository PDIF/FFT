#ifndef STEP_H
#define STEP_H

#include <complex>
#include <assert.h>

class Step
{

    using complex_t = std::complex<double>;

public:

    Step(size_t initPointsPerPeriod);
    virtual ~Step();

    void rebuild(size_t newPointsPerPeriod);


    double Degree() const;

    double Radian() const;

    complex_t Complex() const;


private:

    double      _stepRadian;
    double      _stepDegree;
    complex_t   _stepComplex;

    //Methods
    double      _computePiRadian(size_t newPointsPerPeriod);

    double      _computePiDegree(size_t newPointsPerPeriod);

    complex_t   _computePiComplex(size_t newPointsPerPeriod);


    static constexpr double piDegree() {
        return 180.0;
    };

    static constexpr double piRadian() {
        return acos(-1);
    };

};

#endif // STEP_H
