#ifndef DFT_H
#define DFT_H

#include <complex>
#include "RingBuffer.h"
#include "ReferenceSineWave.h"

class Dft
{
    using base_wave_t    = const ReferenceSineWave;
    using complex_t      = std::complex<double>;
    using ring_complex_t = RingBuffer<complex_t>;

public:
    Dft(const base_wave_t& initReferenceSineWave);
    virtual ~Dft();

    ///добавление новой величины и обновление данных
    void update(double newValue);

    ///комплексное значение
    const complex_t& value() const;

private:

    //ссылка на эталонную синусоиду
    base_wave_t&   _baseSineWave;

    ring_complex_t _data;

    complex_t      _valueCorrection;

    complex_t      _result;

    ring_complex_t _initData(size_t);

    complex_t      _initCorrection(double angle, double amplitude);

};

#endif // DFT_H
