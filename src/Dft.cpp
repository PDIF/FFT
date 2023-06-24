#include "Dft.h"


using complex_t   = std::complex<double>;
using base_wave_t = BaseSineWave;
using size_vec_t  = std::vector<size_t>;

Dft::Dft(
    const base_wave_t* initBaseSineWave,
    const size_vec_t&  initHarmonics,
    double             initAngle,
    double             initAmplitude)
:  FourierTransform(initBaseSineWave, initHarmonics, initAngle, initAmplitude)
{
    //ctor
};


Dft::~Dft()
{
    //dtor
}



void Dft::update(double newValue)
{
    if (!FourierTransform::isValid()) {
        return;
    };

    complex_t complexValue(FourierTransform::_correction * newValue);

   _instant.push_front(complexValue);

    for (size_t i = 0; i < FourierTransform::_harmonics.size(); ++i) {

        size_t harmonic = FourierTransform::_harmonics[i];

       _result[i] = _instant[0];

        for (size_t j = 1; j < _baseSineWave->size(); ++j) {

             size_t _position = j * harmonic % _baseSineWave->size();

            _result[i] += _instant[j] * (*_baseSineWave)[_position];
        };
    };
};


void Dft::setNewBase(const base_wave_t* newBaseSineWave)
{
    FourierTransform::setNewBase(newBaseSineWave);
};


void Dft::setNewHarmonicalSet(const size_vec_t& newSet)
{
    FourierTransform::setNewHarmonicalSet(newSet);
};


