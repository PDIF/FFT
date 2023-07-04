#include "FourierTransform.h"

using base_wave_t    = BaseSineWave;
using size_vec_t     = std::vector<size_t>;
using complex_t      = std::complex<double>;
using ring_complex_t = boost::circular_buffer<complex_t>;
using complex_map_t  = std::map<size_t, complex_t>;


using complex_map_t  = std::map<size_t, complex_t>;
using size_set_t     = std::set<size_t>;



FourierTransform::FourierTransform(
    const base_wave_t* initBaseSineWave,
    const size_vec_t&  initHarmonics,
    double             initAngle,
    double             initAmplitude)
: _result      (_initResult(initHarmonics))
, _baseSineWave(_initBaseSineWave(initBaseSineWave))
, _instant     (_initInstant(initBaseSineWave))
, _correction  (_initCorrection(initAngle,  initAmplitude))
, _angle       ( initAngle)
, _amplitude   ( initAmplitude)
, _valid       ( Valid(initBaseSineWave))
{
    //ctor
};


FourierTransform::~FourierTransform()
{
    //dtor
};


//Установка набора вычисляемых гармоник
void FourierTransform::setNewHarmonicalSet(const size_vec_t& newHarmonicalSet)
{
//    _harmonics = _initHarmonics(newHarmonicalSet);
    _result    = _initResult(newHarmonicalSet);
};


//Установка комплексного множителя коррекции
void FourierTransform::setCorrection(double angle, double amplitude)
{
    _angle      =  angle;
    _amplitude  =  amplitude;
    _correction = _initCorrection(angle, amplitude);
};


//Получение текущего комплексного значения гармоники
const complex_t& FourierTransform::getData(size_t harmonic) const
{
    if (_result.find(harmonic) != _result.end()) {
        return _result.at(harmonic);
    };

    throw std::logic_error{"There is no calculating harmonic"};
};


//Получение вектора комплексных значений набора гармоник
const complex_map_t& FourierTransform::getData() const
{
    return _result;
};



//==================
// Публичные методы
//==================

//Переключение на новую эталонную синусоиду
void FourierTransform::setNewBase(const base_wave_t* newBaseSineWave)
{
    //Обнуление вектора результатов в связи с переходом на новую базу
    _clearResult();

     //Базовая синусоида
    _baseSineWave = _initBaseSineWave(newBaseSineWave);

     //Изменение размера вектора мгновенных значений
    _instant      = _initInstant(newBaseSineWave);

     //Пересчет вектора коррекции
    _correction   = _initCorrection(_angle, _amplitude);

     //Обновление переменной валидности
    _valid        =  Valid(newBaseSineWave);

};



//===================
// Защищенные методы
//===================

//Проверка валидности указателя на базовую синусоиду
bool FourierTransform::isValid() const noexcept
{
    return _valid.isValid(_baseSineWave);
}



//==================
// Приватные методы
//==================

//Очистка вектора вычисленных значений гармоник
void FourierTransform::_clearResult() noexcept
{
    for (auto& harmonic : _result) {
        harmonic.second = _zero;
    }
};


//Инициализация вектора вычисленных значений гармоник
complex_map_t FourierTransform::_initResult(const size_vec_t& initVecHarmonics)
{
    try {

        complex_map_t tmpResult;
        for (auto const& harmonic: initVecHarmonics) {
            tmpResult[harmonic] = _zero;
        };
        return tmpResult;

    } catch (...) {
        throw;
    };
};


//Инициализация эталонной синусоиды
const base_wave_t* FourierTransform::_initBaseSineWave(
                       const base_wave_t* initBaseSineWave)
{
    if (!initBaseSineWave) {
        throw std::logic_error{"Empty pointer to reference sine wave"};
    };

    if (!(initBaseSineWave->size())) {
        throw std::logic_error{"Empty reference sine wave"};
    };

    return initBaseSineWave;
};


//Инициализаия вектора мгновенных значений
ring_complex_t  FourierTransform::_initInstant(
                    const base_wave_t* initBaseSineWave)
{
    try {
        return ring_complex_t(_baseSineWave->size(), _zero);
    } catch (...) {
        throw;
    }
};


//Инициализация комплексного множителя для коррекции входного сигнала
complex_t FourierTransform::_initCorrection(
              double newAngle, double newAmplitude) noexcept
{
    complex_t tmpRotate;
    try {
        tmpRotate = std::exp(complex_t{defaultZero(),
                                       base_wave_t::Pi::degToRad(newAngle)});
    } catch (...) {
        tmpRotate = _one;
    };

    return 2.0 * newAmplitude * tmpRotate * (1.0 / _baseSineWave->size());
};

