#include "Rdft.h"

using base_wave_t   = BaseSineWave;
using size_vec_t    = std::vector<size_t>;
using complex_t     = std::complex<double>;
using complex_vec_t = std::vector<complex_t>;

Rdft::Rdft(
    const base_wave_t* initBaseSineWave,
    const size_vec_t&  initHarmonics,
    double             initAngle,
    double             initAmplitude)
:  FourierTransform( initBaseSineWave, initHarmonics, initAngle, initAmplitude)
, _rotateStep      ( RotateStep(initBaseSineWave))
//, _rotateFactor    (_computeCorrectionFactor())
{
    //ctor



};


Rdft::~Rdft()
{
    //dtor
};


void Rdft::update(double newValue)
{
    if (!FourierTransform::isValid()) {

        throw std::length_error{"Unauthorized base sine wave modification"};
    };

    complex_t complexValue(FourierTransform::_correction * newValue);



    ++_rotateStep;
    if (!_rotateStep.status()) {

        for (auto& [harmonic, value] : _result) {
            value *= (*_baseSineWave)[harmonic];
            value += complexValue - _instant[_baseSineWave->size() - 1];
        };

    } else {

       _rotateStep.reset();

        for (auto& [harmonic, value] : _result) {

            value = complexValue;
            size_t position = 0;

            const auto& size = _baseSineWave->size();

            for (size_t j = 1; j < size; ++j) {

                position += harmonic;

                if (position >= size) {
                    position -= size;
                };

                value += _instant[j] * _baseSineWave->operator[](position);
            };
        };
    };


    _instant.push_front(complexValue);










};


void Rdft::setNewBase(const base_wave_t* newBaseSineWave)
{
    FourierTransform::setNewBase(newBaseSineWave);
   _rotateStep   =  RotateStep(newBaseSineWave);
 //  _rotateFactor = _computeCorrectionFactor();
};


void Rdft::setNewHarmonicalSet(const size_vec_t& newSet)
{
    FourierTransform::setNewHarmonicalSet(newSet);
  // _rotateFactor = _computeCorrectionFactor();
};

/*
complex_vec_t  Rdft::_computeCorrectionFactor()
{

    complex_vec_t tmpCorrectionFactor(
                      FourierTransform::_harmonics.size(),
                      FourierTransform::_one);

    if (!FourierTransform::isValid()) {
        return tmpCorrectionFactor;
    };

    for (size_t i = 0; i < FourierTransform::_harmonics.size(); ++i) {

        complex_t tmpHarmonicalFactor = FourierTransform::_one;
        auto&     harmonic            = FourierTransform::_harmonics[i];

        for (size_t j = 0; j < _rotateStep.maxStep(); ++j){
            tmpHarmonicalFactor *= (*FourierTransform::_baseSineWave)[harmonic];
        }

        tmpCorrectionFactor[i] = FourierTransform::_one / tmpHarmonicalFactor;
    };

    return tmpCorrectionFactor;
};

*/
