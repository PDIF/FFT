#include "DigitalSignalProcess.h"

using base_wave_t    = const ReferenceSineWave;
using size_vec_t     = std::vector<size_t>;
using complex_t      = std::complex<double>;
using complex_vec_t  = std::vector<complex_t>;
using ring_complex_t = RingBuffer<complex_t>;
using ring_base_t    = std::vector<ring_complex_t>;


DigitalSignalProcess::DigitalSignalProcess(
        const base_wave_t& initReferenceSineWave,
        const size_vec_t&  initHarmonics        ,
        double             initAngleDegree      ,
        double             initAmplitude        )
: _baseSineWave   ( initReferenceSineWave)
, _harmonics      ( initHarmonics)
, _base           (_initBase())
, _convolution    (_initConvolution(initHarmonics.size()))
, _current        (_initComplexVector(initHarmonics.size()))
, _value          (_initComplexVector(initHarmonics.size()))
, _valueCorrection(_initCorrection(initAngleDegree, initAmplitude))
, _defaultComplex ( zeroComplex())
{
    //ctor
};

DigitalSignalProcess::~DigitalSignalProcess()
{
    //dtor
}

void DigitalSignalProcess::update(double newValue)
{
    complex_t complexValue(_valueCorrection * newValue);

    _updateCurrent(complexValue);
    _updateBase(complexValue);
    _updateResult();
    _updateConvolution();
};


const complex_vec_t& DigitalSignalProcess::value() const
{
    return _value;
};


const complex_t& DigitalSignalProcess::value(size_t harmonic) const
{
    auto itFind =  std::find(_harmonics.begin(), _harmonics.end(), harmonic);

    if (itFind != _harmonics.end()) {

        size_t  index = std::distance(_harmonics.begin(), itFind);
        return _value[index];
    }

    return _defaultComplex;
};


void DigitalSignalProcess::setCorrection(double angleDegree, double amplitude)
{
    _valueCorrection = _initCorrection(angleDegree, amplitude);
};


size_vec_t DigitalSignalProcess::_initNodes()
{
    size_vec_t  tmpNodes;

    const auto& convolution = _baseSineWave.convolution;

    for (size_t i = 0; i < convolution.size(); ++i) {

        for (size_t j = 0; j < convolution[i]. size(); ++j) {

            tmpNodes.push_back(convolution[i][j]);
        }
    }
    return tmpNodes;
};


complex_vec_t DigitalSignalProcess::_initComplexVector(size_t newSize)
{
    return complex_vec_t(newSize, _defaultComplex);
};


ring_base_t DigitalSignalProcess::_initConvolution(size_t newSize)
{
    size_t length = _baseSineWave.convolution.length();
    return ring_base_t(newSize, ring_complex_t(length, 1.0));
};


ring_base_t DigitalSignalProcess::_initBase()
{
    size_t matrixSize   = _baseSineWave.base.size();
    size_t matrixLength = _baseSineWave.base.length();
    ring_complex_t initBase(matrixLength, 1.0);

    return ring_base_t(matrixSize, initBase);
};


void DigitalSignalProcess::_updateCurrent(const complex_t& newValue)
{
    for (size_t i = 0; i < _harmonics.size(); ++i) {

       _current[i] = newValue;

        size_t  harmonic    = _harmonics[i];
        size_t  position    = _baseSineWave.base.step() - 1;
        const auto& address = _baseSineWave.base[harmonic];

        for (const auto& degree : address) {
           _current[i] += _base[degree][position];
            position   += _baseSineWave.base.step();
        }
    }
};


void DigitalSignalProcess::_updateBase(complex_t newValue)
{
    for (auto& harmonic : _base) {
        harmonic.push_front(newValue);
        newValue *= _baseSineWave[_baseSineWave.base.step()];
    }
};


void DigitalSignalProcess::_updateResult()
{
    for (size_t i = 0; i < _harmonics.size(); ++i) {

       _value[i] = _current[i];

        for (auto index : _baseSineWave.convolution.expand()) {

           _value[i] += _convolution[i][index];
        }

        if (_harmonics[i] == 0) {
           _value[i] *= 0.5;
        }
    }
};


void DigitalSignalProcess::_updateConvolution()
{
    const auto& convolutionBase = _baseSineWave.convolution;

    for (size_t i = 0; i < _harmonics.size(); ++i) {

        size_t    harmonic        = _harmonics[i];
        auto&     convolutionCurr = _convolution[i];
        complex_t current         = _current[i];

        size_t degree = harmonic;

        for (size_t j = 0; j < convolutionBase.size(); ++j) {

            for (size_t k = 0; k < convolutionBase[j].size() - 1; ++k) {

                size_t index  = convolutionBase[j][k];
                current      += convolutionCurr[index];
                convolutionCurr[index] *= _baseSineWave[degree];
            }

            if (j != convolutionBase.size() - 1) {

                size_t index  = convolutionBase[j].back();
                current      += convolutionCurr[index];

                degree  =  convolutionBase.degrees()[j + 1] * harmonic;
                degree %= _baseSineWave.getPointsPerPeriod();

                convolutionCurr[index] = current * _baseSineWave[degree];
            }
        }
        convolutionCurr.push_front(_current[i] * _baseSineWave[harmonic]);
    }
};


complex_t DigitalSignalProcess::_initCorrection(double angle, double amplitude)
{
    angle *= _baseSineWave.step.Radian();
    angle /= _baseSineWave.step.Degree();

    amplitude *= 2.0 / _baseSineWave.getPointsPerPeriod();

    return amplitude * std::exp(complex_t{0, angle});
};

