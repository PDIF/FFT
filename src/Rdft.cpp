#include "Rdft.h"

using base_wave_t   = BaseSineWave;
using size_vec_t    = std::vector<size_t>;
using complex_t     = std::complex<double>;
using complex_vec_t = std::vector<complex_t>;
using complex_map_t = std::map<size_t, complex_t>;

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

//    if (!_rotateStep.status()) {

        for (auto& [harmonic, value] : _result) {
            value *= (*_baseSineWave)[harmonic];
            value += complexValue - _instant[_baseSineWave->size() - 1];
        };

//    } else {
/*
       _rotateStep.reset();
        const auto& size    = _baseSineWave->size();

        static complex_map_t::iterator step = _result.begin();



        size_t     harmonic = step->first;
        complex_t& value    = step->second;

        value = complexValue;

        size_t     position = harmonic;
        for (size_t j = 0; j + 1 < size; ++j) {

            if (position >= size) {
                position -= size;
            };
            value    += _instant.at(j) * _baseSineWave->operator[](position);
            position +=  harmonic;
        };

        ++step;
        if (step == _result.end()) {
            step = _result.begin();
        };


        std::cout << step->first << "\t" << step->second << "\n"; // _result[step->first] << "\n";
    };
*/
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
