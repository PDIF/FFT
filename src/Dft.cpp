#include "Dft.h"

using base_wave_t    = const ReferenceSineWave;
using complex_t      = std::complex<double>;
using ring_complex_t = RingBuffer<complex_t>;

Dft::Dft(const base_wave_t& initReferenceSineWave)
: _baseSineWave  (initReferenceSineWave)
, _data(_initData(initReferenceSineWave.size()))
, _valueCorrection(_initCorrection(0.0, 1.0))
{
    //ctor
}

Dft::~Dft()
{
    //dtor
}



void Dft::update(double newValue)
{
    complex_t complexValue(_valueCorrection * newValue);

   _data.push_front(complexValue);

   _result = complexValue;

    for (size_t i = 1; i < _baseSineWave.size(); ++i) {
       _result += _baseSineWave[i] * _data[i];
    }
};


const complex_t& Dft::value() const
{
    return _result;
}


complex_t Dft::_initCorrection(double angle, double amplitude)
{
    angle *= _baseSineWave.step.Radian();
    angle /= _baseSineWave.step.Degree();

    amplitude *= 2.0 / _baseSineWave.size();

    return amplitude * std::exp(complex_t{0, angle});
};


ring_complex_t Dft::_initData(size_t dataSize)
{
    return ring_complex_t (dataSize, 1.0);
};
