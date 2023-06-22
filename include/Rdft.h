#ifndef RDFT_H
#define RDFT_H

//#include "RingBuffer.h"
#include "FourierTransform.h"

class Rdft : public FourierTransform
{
    using base_wave_t   = BaseSineWave;
    using size_vec_t    = std::vector<size_t>;
    using complex_t     = std::complex<double>;
    using complex_vec_t = std::vector<complex_t>;

    ///Класс подсчета шагов для учета компенсации вычислительной погрешности
    class RotateStep
    {
    public:

        RotateStep(
            const base_wave_t* initBaseSineWave,
            size_t             initPeriodNumber = 200)
        : _periodSize    ( initBaseSineWave ? initBaseSineWave->size() : 0)
        , _periodesNumber( initPeriodNumber)
        , _max           (_periodSize * _periodesNumber)
        , _currentStep   (0)
        { };

        void rebuild(size_t newPeriodSize) {
           _periodSize  = newPeriodSize;
           _max         = newPeriodSize * _periodesNumber;
           _currentStep = 0;
        };

        void reset() {
            _currentStep = 0;
        };

        bool status() {
            return this->_currentStep >= this->_max;
        };

        size_t maxStep() {
            return _max;
        };

        RotateStep& operator++() {
            ++_currentStep;
            return *this;
        };

    private:
        size_t  _periodSize;
        size_t  _periodesNumber;
        size_t  _max;
        size_t  _currentStep;
    };

public:
    Rdft(
        const base_wave_t* initBaseSineWave = nullptr,
        const size_vec_t&  initHarmonics    = defaultHarmonics(),
        double             initnAngle       = defaultZero(),
        double             initAmplitude    = defaultOne());

    virtual ~Rdft();

    ///Добавление новой величины и обновление данных
    void update(double newValue) override;

    ///Переключение на новую эталонную синусоиду
    void setNewBase(const BaseSineWave* newBaseSineWave) override;

    ///Установка набора вычисляемых гармоник
    virtual void setNewHarmonicalSet(const size_vec_t& newSet) override;

private:

    //========
    //  Поля
    //========

    //Компенсация вычислительной погрешности
    RotateStep      _rotateStep;
    complex_vec_t   _rotateFactor;

    //========
    // Методы
    //========

    ///Вычисление корректирующего множителя по гармоникам
    complex_vec_t   _computeCorrectionFactor();

};

#endif // RDFT_H
