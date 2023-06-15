#include "Step.h"

using complex_t = std::complex<double>;

Step::Step(size_t initPointsPerPeriod)
: _stepRadian(_computePiRadian(initPointsPerPeriod))
, _stepDegree(_computePiDegree(initPointsPerPeriod))
, _stepComplex(_computePiComplex(initPointsPerPeriod))
{
    //ctor
};


void Step::rebuild(size_t newPointsPerPeriod)
{
   _stepRadian  = _computePiRadian(newPointsPerPeriod);
   _stepDegree  = _computePiDegree(newPointsPerPeriod);
   _stepComplex = _computePiComplex(newPointsPerPeriod);
};


double Step::Degree() const
{
    return _stepDegree;
};


double Step::Radian() const
{
    return _stepRadian;
};


complex_t Step::Complex() const
{
    return _stepComplex;
};



double Step::_computePiRadian(size_t newPointsPerPeriod)
{
    assert(newPointsPerPeriod > 0 && "Points per period zero value");
    return 2.0 * piRadian() / newPointsPerPeriod;
};

double Step::_computePiDegree(size_t newPointsPerPeriod)
{
    assert(newPointsPerPeriod > 0 && "Points per period zero value");
    return 2.0 * piDegree() / newPointsPerPeriod;
};

complex_t Step::_computePiComplex(size_t newPointsPerPeriod)
{
    assert(newPointsPerPeriod > 0 && "Points per period zero value");
    return std::exp(complex_t{0, _computePiRadian(newPointsPerPeriod)});
};


Step::~Step()
{
    //dtor
}

