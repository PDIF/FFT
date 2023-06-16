#ifndef REFERENCESINEWAVE_H
#define REFERENCESINEWAVE_H

#include <complex>
#include <vector>
#include "RingBuffer.h"

//#include <iostream>
#include <assert.h>

#include "Convolution.h"
#include "Step.h"

class Base
{
    using size_vec_t     = std::vector<size_t>;
    using size_matrix_t  = std::vector<size_vec_t>;

public:

    Base(size_t initPointsPerPeriod, size_t initSize)
    : _matrix(_createMatrix(initSize))
    , _length(_computeLength(initPointsPerPeriod, initSize))
    , _step  (_computeStep(initPointsPerPeriod, initSize))

    { };

    void rebuild(size_t newPointsPerPeriod, size_t newSize)
    {
        _matrix = _createMatrix(newSize);
        _length = _computeLength(newPointsPerPeriod, newSize);
        _step   = _computeStep(newPointsPerPeriod, newSize);
    };


    const size_vec_t& operator[](size_t index) const
    {
        index %= _matrix.size();
        return _matrix[index];
    };


    size_t size() const
    {
        return _matrix.size();
    };


    size_t step() const
    {
        return _step;
    }


    size_t length() const
    {
        return _length;
    };


private:

    size_matrix_t _matrix;
    size_t        _length;
    size_t        _step;


    size_matrix_t _createMatrix(size_t newSize)
    {
        assert((newSize > 0) && "The matrix size is too small");

        size_matrix_t tmpMatrix(newSize, size_vec_t(newSize - 1, 0));

        for (size_t i = 1; i < newSize; ++i) {
            for (size_t j = 0; j < newSize - 1; ++j) {
                tmpMatrix[i][j] = (i * (j + 1)) % newSize;
            };
        };
        return tmpMatrix;
    };


    size_t _computeStep(size_t pointsPerPeriod, size_t baseSize)
    {
        return pointsPerPeriod / baseSize;
    };


    size_t _computeLength(size_t pointsPerPeriod, size_t baseSize)
    {
        return pointsPerPeriod - pointsPerPeriod / baseSize; //+1;
    };
};




class ReferenceSineWave
{
    using complex_t      = std::complex<double>;
    using complex_vec_t  = std::vector<complex_t>;
    using size_vec_t     = std::vector<size_t>;
    using size_matrix_t  = std::vector<size_vec_t>;

    size_t      _pointsPerPeriod;
    size_vec_t  _fullPrimeFactor;


public:

    //ctor
    ReferenceSineWave(const size_t& pointsPerPeriod = defaultSize());

    ///блок вычисления шага между двумя смежными точками
    Step        step;

    ///блок эталонной базы для расчета первичного преобразования
    Base        base;

    ///блок свертки для дальнейшего использования ранее посчитанных величин
    Convolution convolution;

    ///число точек на период
    size_t      getPointsPerPeriod()    const;

    ///обращение к отдельному элементу эталонного массива
    const
    complex_t&  operator[](size_t position) const;

    ///изменение размера эталонного массива
    void        update(const size_t& newPointsPerPeriod);

    //dtor
    virtual    ~ReferenceSineWave();



private:

    complex_vec_t   _sineWaveData;

    size_vec_t      _computeFullPrimeFactor();
    complex_vec_t   _computeSineWave();






    //=============================
    // Private static const values
    //=============================
    static constexpr size_t defaultSize() {
        return 96;
    };

    static constexpr double defaultZero() {
        return 0.0;
    };

    static constexpr double defaultOne() {
        return 1.0;
    };

    static constexpr complex_t defaultComplex() {
        return complex_t{defaultOne(), defaultZero()};
    };


/*
    static constexpr double piDegree() {
        return 180.0;
    };
*/
};


#endif // REFERENCESINEWAVE_H
