#include "FourierTransform.h"

using base_wave_t    = BaseSineWave;
using size_vec_t     = std::vector<size_t>;
using complex_t      = std::complex<double>;
using complex_vec_t  = std::vector<complex_t>;
using ring_complex_t = RingBuffer<complex_t>;

FourierTransform::FourierTransform(
    const base_wave_t* initBaseSineWave,
    const size_vec_t&  initHarmonics,
    double             initAngle,
    double             initAmplitude)
: _harmonics   ( initHarmonics)
, _result      ( complex_vec_t(initHarmonics.size(), _zero))
, _baseSineWave( initBaseSineWave)
, _instant     (_initInstant())
, _correction  (_computeCorrection(initAngle,  initAmplitude))
, _angle       ( initAngle)
, _amplitude   ( initAmplitude)
, _valid       ( Valid(initBaseSineWave))
{
    //ctor
}


FourierTransform::~FourierTransform()
{
    //dtor
};


void FourierTransform::setNewHarmonicalSet(const size_vec_t& newSet)
{
    _harmonics = newSet;
    _result    = complex_vec_t(newSet.size(), _zero);
};


void FourierTransform::setNewBase(const base_wave_t* newBaseSineWave)
{
    _baseSineWave =  newBaseSineWave;
    _correction   = _computeCorrection(_angle, _amplitude);

     //Обнуление вектора результатов в связи с переходом на новую базу
    _result       =  complex_vec_t(_harmonics.size(), _zero);

    //Изменение размера вектора мгновенных значений
    _instant      = _initInstant();

    //Обновление переменной валидности
    _valid       =   Valid(newBaseSineWave);
};


void FourierTransform::setCorrection(double angle, double amplitude)
{
    _angle      =  angle;
    _amplitude  =  amplitude;
    _correction = _computeCorrection(_angle, _amplitude);
};


const complex_t& FourierTransform::getData(size_t harmonic) const
{
    auto itFound =  std::find(_harmonics.begin(), _harmonics.end(), harmonic);

    //assert(itFound != _harmonics.end() && "Needed harmonic is not calculated");

    if (itFound != _harmonics.end()) {

        size_t  index = std::distance(_harmonics.begin(), itFound);

        return _result[index];
    };

    return _zero;
};


const complex_vec_t& FourierTransform::getData() const
{
    return _result;
};


bool FourierTransform::isValid() const
{
    return _valid.isValid(_baseSineWave);
}


ring_complex_t  FourierTransform::_initInstant()
{
    if (!_baseSineWave) {
        return ring_complex_t(0);
    };

    return ring_complex_t(_baseSineWave->size());
};


complex_t FourierTransform::_computeCorrection(double newAngle,
                                               double newAmplitude)
{
    if (!_baseSineWave) {
        return _zero;
    };

    complex_t tmpAmplitude = 2.0 * newAmplitude / _baseSineWave->size();
    complex_t tmpImage     = complex_t(0.0, base_wave_t::degToRad(newAngle));
    complex_t tmpRotate    = std::exp(tmpImage);

    return tmpAmplitude * tmpRotate;
};
