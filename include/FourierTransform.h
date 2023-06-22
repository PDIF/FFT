#ifndef FOURIERTRANSFORM_H
#define FOURIERTRANSFORM_H

#include <vector>
#include <algorithm>
#include "BaseSineWave.h"
#include "RingBuffer.h"

class FourierTransform
{
    using base_wave_t    = BaseSineWave;
    using size_vec_t     = std::vector<size_t>;
    using complex_t      = std::complex<double>;
    using complex_vec_t  = std::vector<complex_t>;
    using ring_complex_t = RingBuffer<complex_t>;

    ///Класс для проверки валидности базовой синусоиды
    class Valid
    {
    public:

        Valid(const base_wave_t* initValidWave)
        : _validWave( initValidWave)
        , _size     (_getSize())
        { };

        bool isValid(const base_wave_t* checkedWave) const {

            return _validWave  && _size        &&
                   _validWave  ==  checkedWave &&
                   _size       ==  checkedWave->size();
        };

    private:

        const base_wave_t* _validWave;
        size_t             _size;

        //Получение размера буфера по указателю
        size_t _getSize() {
            return _validWave ? _validWave->size() : 0;
        };
    };

public:

    //========
    // Методы
    //========

    FourierTransform(
        const base_wave_t* initBaseSineWave = nullptr,
        const size_vec_t&  initHarmonics    = defaultHarmonics(),
        double             initnAngle       = defaultZero(),
        double             initAmplitude    = defaultOne());

    virtual ~FourierTransform();


    ///Добавление новой величины и обновление данных
    virtual void update(double newValue) = 0;


    ///Переключение на новую эталонную синусоиду
    virtual void setNewBase(const base_wave_t* newBaseSineWave) = 0;


    ///Установка набора вычисляемых гармоник
    virtual void setNewHarmonicalSet(const size_vec_t& newSet) = 0;


    ///Установка комплексного множителя коррекции
    virtual void setCorrection(double angle = 0.0, double amplitude = 1.0);


    ///Получение текущего комплексного значения гармоники
    const complex_t& getData(size_t harmonic) const;


    ///Получение вектора комплексных значений набора гармоник
    const complex_vec_t& getData() const;

protected:

    //========
    //  Поля
    //========

    ///Набор гармоник, которые необходимо вычислить
    size_vec_t      _harmonics;


    ///Вектор результатов вычисления гармоник
    complex_vec_t   _result;


    ///Указатель на эталонную синусоиду
    const
    base_wave_t*    _baseSineWave;


    ///Вектор мгновенных значений
    ring_complex_t  _instant;


    ///Комплексный множитель коррекции входного сигнала
    complex_t       _correction;


    ///Комплексный ноль
    static constexpr complex_t _zero = complex_t(0.0, 0.0);


    ///Комплексная единица
    static constexpr complex_t _one  = complex_t(1.0, 0.0);


    //========
    // Методы
    //========

    ///Проверка валидности указателя на базовую синусоиду
    bool isValid() const;


    ///Инициализаия вектора мгновенных значений
    ring_complex_t  _initInstant();


    ///Наиболее востребованный набор вычисляемых гармоник
    static const size_vec_t defaultHarmonics() {
        return size_vec_t{1, 2, 3, 4, 5, 6, 7, 9, 11, 12, 13};
    };

    static constexpr double defaultZero() {
        return 0.0;
    };

    static constexpr double defaultOne() {
        return 1.0;
    };

private:

    //========
    //  Поля
    //========

    //Корректирующие параметры, не зависящие от размера базовой синусоиды
    double    _angle;
    double    _amplitude;

    //Проверка валидности
    Valid     _valid;

    //========
    // Методы
    //========

    //Вычисление комплексного множителя коррекции
    complex_t _computeCorrection(double angle, double amplitude);

};

#endif // FOURIERTRANSFORM_H
