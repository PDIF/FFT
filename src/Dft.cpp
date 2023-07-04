#include "Dft.h"

using base_wave_t = BaseSineWave;
using complex_t   = std::complex<double>;
using size_vec_t  = std::vector<size_t>;
using complex_vec_t  = std::vector<complex_t>;


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
//    if (!FourierTransform::isValid()) {
//
//        throw std::length_error{"Unauthorized base sine wave modification"};
//    };

    complex_t complexValue(FourierTransform::_correction * newValue);

   _instant.push_front(complexValue);

    //std::fill(_result.begin(), _result.end(), complexValue);

    /*
    for (auto& [harmonic, value] : _result) {
        value = complexValue;
    };
*/
    const auto& size = _baseSineWave->size();

    for (auto& [harmonic, value] : _result) {

        value = complexValue;

        size_t position = 0;
        for (size_t j = 1; j < size; ++j) {

            position += harmonic;

            if (position >= size) {
                position -= size;
            };

            value += _instant[j] * _baseSineWave->operator[](position);
        };

    }


//
//
//    auto  resultHarmonic = _result.begin();
//
//    for (const auto& harmonic : FourierTransform::_result) {
//
//        size_t position = 0;
//
//        for (size_t j = 1; j < size; ++j) {
//
//            position += harmonic.first;
//
//            if (position >= size) {
//                position -= size;
//            }
//
//            resultHarmonic->second += _instant[j] *
//                                      _baseSineWave->operator[](position);
//        };
//
//        ++resultHarmonic;
//    };

};


void Dft::setNewBase(const base_wave_t* newBaseSineWave)
{
    FourierTransform::setNewBase(newBaseSineWave);
};


void Dft::setNewHarmonicalSet(const size_vec_t& newSet)
{
    FourierTransform::setNewHarmonicalSet(newSet);
};


