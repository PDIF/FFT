#ifndef DFT_H
#define DFT_H

#include "FourierTransform.h"

class Dft : public FourierTransform
{
    using base_wave_t = BaseSineWave;
    using complex_t   = std::complex<double>;
    using size_vec_t  = std::vector<size_t>;
    using complex_vec_t  = std::vector<complex_t>;

public:

    Dft(const base_wave_t* initBaseSineWave = nullptr,
        const size_vec_t&  initHarmonics    = defaultHarmonicsDft(),
        double             initnAngle       = defaultZero(),
        double             initAmplitude    = defaultOne());

    virtual ~Dft();

    ///Добавление новой величины и обновление данных
    void update(double newValue) override;

    ///Переключение на новую эталонную синусоиду
    void setNewBase(const BaseSineWave* newBaseSineWave) override;

    ///Установка набора вычисляемых гармоник
    virtual void setNewHarmonicalSet(const size_vec_t& newSet) override;

private:

    //========
    // Методы
    //========

    static const size_vec_t defaultHarmonicsDft() {
        return size_vec_t{1, 3, 5};
    };

};

#endif // DFT_H

