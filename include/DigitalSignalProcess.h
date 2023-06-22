#ifndef Fft_H
#define Fft_H

#include <complex>
#include <vector>
#include <algorithm>

#include <assert.h>

#include <iostream>
#include "ReferenceSineWave.h"
#include "RingBuffer.h"

class Fft
{
    using base_wave_t    = const ReferenceSineWave;
    using size_vec_t     = std::vector<size_t>;
    using complex_t      = std::complex<double>;
    using complex_vec_t  = std::vector<complex_t>;
    using ring_complex_t = RingBuffer<complex_t>;
    using ring_base_t    = std::vector<ring_complex_t>;

public:

    //ctor
    Fft(
        const base_wave_t& initReferenceSineWave,
        const size_vec_t&  initHarmonics     = defaultHarmonicsData(),
        double       initAngleCorrection     = defaultAngleCorrection(),
        double       initAmplitudeCorrection = defaultAmplitudeCorrection());

    ///добавление новой величины и обновление данных
    void update(double newValue);

    ///вектор комплексных значений вектора гармоник
    const complex_vec_t& value() const;

    ///комплексное значение заданной гармоники
    const complex_t& value(size_t harmonic) const;

    ///установка коррекции значений
    void setCorrection(double angleDegree, double amplitude);

    //dtor
    virtual ~Fft();


private:

    //ссылка на эталонную синусоиду
    base_wave_t&    _baseSineWave;

    //вектор искомых гармоник
    size_vec_t      _harmonics;

    //матрица значений, умноженных на комплексы эталонной синусоиды
    ring_base_t     _base;

    //матрица сверток по гармоникам
    ring_base_t     _convolution;

    //вектор модготовленных мгновенных значений по гармоникам
    complex_vec_t   _current;

    //результат ЦОС (вектор искомых значений по гармоникам)
    complex_vec_t   _value;

    //множитель коррекции новой (входящей) величины
    complex_t       _valueCorrection;

    //дефолтное нулевое комплексное значение
    const complex_t _defaultComplex;


    //Методы
    size_vec_t      _initNodes();

    complex_vec_t   _initComplexVector(size_t newSize);

    ring_base_t     _initConvolution(size_t newSize);

    ring_base_t     _initBase();

    complex_t       _initCorrection(double angle, double amplitude);

    void            _updateCurrent(const complex_t& newValue);

    void            _updateBase(complex_t newValue);

    void            _updateResult();

    void            _updateConvolution();


    static constexpr double defaultAngleCorrection() {
        return 0.0;
    };

    static constexpr double defaultAmplitudeCorrection() {
        return 1.0;
    };

    static constexpr complex_t zeroComplex() {
        return complex_t(0.0, 0.0);
    };

    static const size_vec_t defaultHarmonicsData() {
        return size_vec_t{0, 1, 2, 3, 4, 5, 6, 7, 9, 11, 12, 13};
        //return size_vec_t{1};
    };

};



#endif // Fft_H
