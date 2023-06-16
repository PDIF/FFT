#include "ReferenceSineWave.h"

using complex_t        = std::complex<double>;
using complex_vec_t   = std::vector<complex_t>;
using size_vec_t        = std::vector<size_t>;
using size_matrix_t  = std::vector<size_vec_t>;

ReferenceSineWave::ReferenceSineWave(const size_t& initPointsPerPeriod)
: _pointsPerPeriod    (initPointsPerPeriod)
, _fullPrimeFactor    (_computeFullPrimeFactor())
,  step(Step(initPointsPerPeriod))
,  base(Base(initPointsPerPeriod, _fullPrimeFactor.back()))
,  convolution(Convolution(initPointsPerPeriod))
, _sineWaveData       (_computeSineWave())
{ };


size_t ReferenceSineWave::size() const
{
    return _pointsPerPeriod;
};


const complex_t& ReferenceSineWave::operator[](size_t position) const
{
    assert((position < _pointsPerPeriod) && "Index more than vector size");
    return _sineWaveData[position];
};


complex_vec_t ReferenceSineWave::_computeSineWave()
{
    complex_vec_t tmpSineWave(_pointsPerPeriod, defaultComplex());
    for(size_t i = 1; i < _pointsPerPeriod; ++i) {
        tmpSineWave[i] = tmpSineWave[i - 1] * step.Complex();
    }
    return tmpSineWave;
};


size_vec_t ReferenceSineWave::_computeFullPrimeFactor()
{
    size_vec_t tmpPrimeFactor;

    size_t currentPoints     = _pointsPerPeriod;
    size_t currentMultiplier =  static_cast<size_t>(2);

    while (currentPoints > 1) {
        if (currentPoints %  currentMultiplier == 0) {
            currentPoints /= currentMultiplier;
            tmpPrimeFactor.push_back(currentMultiplier);
        } else {
            ++currentMultiplier;
        }
    }
    return tmpPrimeFactor;
};


void ReferenceSineWave::update(const size_t& newPointsPerPeriod)
{
    _pointsPerPeriod     =  newPointsPerPeriod;
    _fullPrimeFactor     = _computeFullPrimeFactor();

     step                = Step(newPointsPerPeriod);
     base                = Base(newPointsPerPeriod, _fullPrimeFactor.back());
     convolution         = Convolution(newPointsPerPeriod);

    _sineWaveData        = _computeSineWave();
};


ReferenceSineWave::~ReferenceSineWave()
{
    //dtor
}


