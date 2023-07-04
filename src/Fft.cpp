#include "Fft.h"

using base_wave_t    = BaseSineWave;
using size_vec_t     = std::vector<size_t>;
using complex_t      = std::complex<double>;
using complex_vec_t  = std::vector<complex_t>;
//using ring_complex_t = RingBuffer<complex_t>;
using ring_complex_t = boost::circular_buffer<complex_t>;
using ring_base_t    = std::vector<ring_complex_t>;


Fft::Fft(
    const base_wave_t* initBaseSineWave,
    const size_vec_t&  initHarmonics,
    double             initAngle,
    double             initAmplitude)
:  FourierTransform ( initBaseSineWave, initHarmonics, initAngle, initAmplitude)
, _convolutionLayout( Convolution(initBaseSineWave))
, _baseLayout       ( Base(initBaseSineWave))
, _convolutionData  (_initConvolutionData(
                         _result.size(),
                         _convolutionLayout.length()))
, _baseData         (_initBaseData(
                         _baseLayout.size(),
                         _baseLayout.step()))
, _baseResult       (_initBaseResult(_baseLayout.size()))
{
    //ctor
};

Fft::~Fft()
{
    //dtor
}

void Fft::update(double newValue)// noexcept
{
    if (!FourierTransform::isValid()) {
        throw std::length_error{"Unauthorized base sine wave modification"};
    };

    complex_t complexValue(FourierTransform::_correction * newValue);

   _updateBase(complexValue);

   _updateResult();

   _instant.push_front(complexValue);

};


void Fft::setNewBase(const base_wave_t* newBaseSineWave)
{
    FourierTransform::setNewBase(newBaseSineWave);

   _convolutionLayout =  Convolution(newBaseSineWave);
   _baseLayout        =  Base(newBaseSineWave);
   _convolutionData   = _initConvolutionData(
                            _result.size(),
                            _convolutionLayout.length());
   _baseData          = _initBaseData(
                            _baseLayout.size(),
                            _baseLayout.step());
   _baseResult        = _initBaseResult(_baseLayout.size());
};


void Fft::setNewHarmonicalSet(const size_vec_t& newSet)
{
    FourierTransform::setNewHarmonicalSet(newSet);

   _convolutionData   = _initConvolutionData(
                            _result.size(),
                            _convolutionLayout.length());
};


void Fft::_updateBase(const complex_t& newValue) noexcept
{
    //Обновление результатов расчета базы
    for (size_t i = 0; i < _baseLayout.size(); ++i) {

       _baseResult[i]      =  newValue;
        size_t column      = _baseLayout.step() - 1;

        for (const auto& row : _baseLayout[i]) {
           _baseResult[i] += _baseData[row][column];
            column        += _baseLayout.step();
        };
    };

    //Добавление нового значения в матрицу базы
    size_t power = 0;

    for (auto& row : _baseData) {
        row.push_front(newValue * (*_baseSineWave)[power]);
        power += _baseLayout.step();
    };
};


void Fft::_updateResult() noexcept
{
    for (auto& [harmonic, value] : _result) {


    };










    for (size_t i = 0; i < _result.size(); ++i) {

        size_t    currentHarmonic = _harmonics[i];
        auto&     sourceHarmonic  = _convolutionData[i];
        complex_t resultHarmonic  = _baseResult[currentHarmonic %
                                               _baseLayout.size()];

        //Обновление результатов расчета текущей гармоники
       _result[i] = resultHarmonic;

        for (const auto& position : _convolutionLayout.expand()) {
           _result[i] += sourceHarmonic[position];
        };

        //Обновление матрицы свертки
        const auto& sizeConvolution = _convolutionLayout.size();

        //Если свертка существует, то разворачиваем и вычисляем ее
        if (sizeConvolution > 0) {

            complex_t   adding       =  resultHarmonic *
                                        (*_baseSineWave)[currentHarmonic];
            const auto& sizeSineWave = _baseSineWave->size();

            //1. Обработка матрицы свертки до предпоследнего набора позиций
            for (size_t j = 0; j + 1 < sizeConvolution; ++j) {

                const auto& positionSet = _convolutionLayout[j];

                for (size_t k = 0; k + 1 < positionSet.position.size(); ++k) {

                    size_t     power    = positionSet.power *
                                          currentHarmonic % sizeSineWave;
                    const auto position = positionSet.position[k];
                    resultHarmonic     += sourceHarmonic[position];
                    sourceHarmonic[position] *= (*_baseSineWave)[power];
                };

                size_t power    = _convolutionLayout[j + 1].power *
                                   currentHarmonic % sizeSineWave;
                auto position   =  positionSet.position.back();
                resultHarmonic +=  sourceHarmonic[position];
                sourceHarmonic[position] = resultHarmonic *
                                           (*_baseSineWave)[power];
            };
            //2. Обработка последнего набора позиций матрицы свертки
            const auto& lastPosition = _convolutionLayout[sizeConvolution - 1];
            size_t      power        =  lastPosition.power *
                                        currentHarmonic % sizeSineWave;
            for (size_t k = 0; k + 1 < lastPosition.position.size(); ++k) {

               const auto position = lastPosition.position[k];
               sourceHarmonic[position] *= (*_baseSineWave)[power];
            };

            sourceHarmonic.push_front(adding);
        };

    };

};


ring_base_t Fft::_initConvolutionData(size_t newHarmonicsNumber,
                                      size_t newConvolutionLength)
{
    return ring_base_t(
               newHarmonicsNumber,
               ring_complex_t(newConvolutionLength, _zero));
};


ring_base_t Fft::_initBaseData(size_t newBaseLayoutSize,
                               size_t newBaseLayoutStep)
{
    return ring_base_t(
               newBaseLayoutSize,
               ring_complex_t((newBaseLayoutSize - 1) * newBaseLayoutStep,
                              _zero));
};


complex_vec_t Fft::_initBaseResult(size_t newBaseLayoutSize)
{
    return complex_vec_t(newBaseLayoutSize);
};
