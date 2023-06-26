#include "BaseSineWave.h"

using complex_t     = std::complex<double>;
using complex_vec_t = std::vector<complex_t>;


BaseSineWave::BaseSineWave(size_t initSize)
:  step     (_setStep(initSize))
, _sineWave (_setWave(initSize))
{
    //ctor
}

BaseSineWave::~BaseSineWave()
{
    //dtor
}


BaseSineWave::Step BaseSineWave::_setStep(size_t initStep)
{
    if (initStep > maxSize() || initStep < minSize()) {
        throw std::length_error{
                  "Exceeding the allowed step number in BaseSineWave"};
    };
    return Step(initStep);
};


complex_vec_t BaseSineWave::_setWave(size_t initStep)
{
    try {

        complex_vec_t tmpWave(initStep, complex_t(1.0, 0.0));

        for (size_t i = 1; i < initStep; ++i) {
            tmpWave[i] = std::exp(complex_t(0.0, i * 2.0 * Pi::rad / initStep));
        };

        return tmpWave;

    } catch (...) {

        throw;
    };

};


size_t BaseSineWave::size() const noexcept
{
    return step.count();
};


const complex_t&  BaseSineWave::operator[](size_t index) const
{
    return _sineWave[index];
}
