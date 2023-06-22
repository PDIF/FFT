#ifndef BASESINEWAVE_H
#define BASESINEWAVE_H

#include <vector>
#include <complex>
#include <cmath>

#include <assert.h>

class BaseSineWave
{

    using complex_t     = std::complex<double>;
    using complex_vec_t = std::vector<complex_t>;

    ///Класс для хранения шага и размера базовой синусоиды
    class Step
    {
        friend class BaseSineWave;

    public:

        ///Количество шагов (точек) на период
        size_t count() const {
            return _count;
        };

        ///Значение шага в градусах
        double degree() const {
            return _degree;
        };

        ///Значение шага в радианах
        double radian() const {
            return _radian;
        };

        ///Комплексное значение шага
        complex_t complex() const {
            return _complex;
        };

    private:
        //=================
        //  Закрытые поля
        //=================
        size_t      _count;
        double      _degree;
        double      _radian;
        complex_t   _complex;

        //=================
        // Закрытые методы
        //=================
        ///Конструктор с указанием числа точек на период
        Step(size_t initStep)
        : _count  (initStep)
        , _degree (_computeDegree(initStep))
        , _radian (_computeRadian(initStep))
        , _complex(_computeComplex(initStep))
        { };

        ///Перестроение шага с новым количеством точек на период
        void update(size_t newStep) {
            assert (newStep > 0 && "Null step value in update");
           _count   =  newStep;
           _degree  = _computeDegree(newStep);
           _radian  = _computeRadian(newStep);
           _complex = _computeComplex(newStep);
        };

        ~Step()
        { };


        double _computeDegree(size_t newStep) {
            assert (newStep > 0 && "Null step value in _computeDegree");
            return 2.0 * BaseSineWave::Pi::deg / newStep;

        };

        double _computeRadian(size_t newStep) {
            assert (newStep > 0 && "Null step value in _computeRadian");
            return 2.0 * BaseSineWave::Pi::rad / newStep;
        };

        complex_t _computeComplex(size_t newStep) {
            assert (newStep > 0 && "Null step value in _computeComplex");
            return std::exp(complex_t(0.0, -_computeRadian(newStep)));
        };

    };

public:

    ///Структура для хранения числа ПИ
    struct Pi
    {
        ///Значение в градусах
        static constexpr double deg = 180.0;

        ///Значение в радианах
        static constexpr double rad = acos(-1.0);
    };


    ///Конструктор с заданием размера (число точек на период)
    BaseSineWave(size_t initSize = defaultSize());

    ///Характеристики шага эталонной синусоиды
    Step        step;

    ///Переопределение шага на период эталонной синусоиды
    void        update(size_t newSize = defaultSize());

    ///Размер эталонной синусоиды (число точек на период)
    size_t      size()    const;

    ///Оператор []
    const
    complex_t&  operator[](size_t index) const;

    ///Перевод градусов в радианы
    static double degToRad(double angle) {
        return angle * Pi::rad / Pi::deg;
    };

    ///Перевод радиан в градусы
    static double radToDeg(double angle) {
        return angle * Pi::deg / Pi::rad;
    };

    virtual ~BaseSineWave();


private:

    //=================
    //  Закрытые поля
    //=================
    complex_vec_t   _sineWave;

    //=================
    // Закрытые методы
    //=================
    Step            _setStep(size_t initStep);
    complex_vec_t   _setWave(size_t initStep);

    ///Число точек на период в соответствии с корпоративным профилем
    static constexpr size_t defaultSize() {
        return 96;
    };

};

#endif // BASESINEWAVE_H
