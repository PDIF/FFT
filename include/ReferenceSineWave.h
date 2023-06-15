#ifndef REFERENCESINEWAVE_H
#define REFERENCESINEWAVE_H

#include <complex>
#include <vector>
#include "RingBuffer.h"

//#include <iostream>
#include <assert.h>

#include "Convolution.h"

class Step
{
    using complex_t = std::complex<double>;

public:
    Step(size_t initPointsPerPeriod)
    : _stepRadian(_computePiRadian(initPointsPerPeriod))
    , _stepDegree(_computePiDegree(initPointsPerPeriod))
    , _stepComplex(_computePiComplex(initPointsPerPeriod))
    { };

    void rebuild(size_t newPointsPerPeriod)
    {
       _stepRadian  = _computePiRadian(newPointsPerPeriod);
       _stepDegree  = _computePiDegree(newPointsPerPeriod);
       _stepComplex = _computePiComplex(newPointsPerPeriod);
    };


    double Degree() const
    {
        return _stepDegree;
    };

    double Radian() const
    {
        return _stepRadian;
    };

    complex_t Complex() const
    {
        return _stepComplex;
    };


private:

    double      _stepRadian;
    double      _stepDegree;
    complex_t   _stepComplex;

    double _computePiRadian(size_t newPointsPerPeriod)
    {
        assert(newPointsPerPeriod > 0 && "Points per period zero value");
        return 2.0 * piRadian() / newPointsPerPeriod;
    };

    double _computePiDegree(size_t newPointsPerPeriod)
    {
        assert(newPointsPerPeriod > 0 && "Points per period zero value");
        return 2.0 * piDegree() / newPointsPerPeriod;
    };

    complex_t _computePiComplex(size_t newPointsPerPeriod)
    {
        assert(newPointsPerPeriod > 0 && "Points per period zero value");
        return std::exp(complex_t{0, _computePiRadian(newPointsPerPeriod)});
    };

    static constexpr double piDegree() {
        return 180.0;
    };

    static constexpr double piRadian() {
        return acos(-1);
    };
};

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

    ReferenceSineWave(const size_t& pointsPerPeriod = defaultSize());

    Step        step;
    Base        base;
    Convolution convolution;

    size_t      getPointsPerPeriod()    const;

    const
    complex_t&  operator[](size_t position) const;

    void        update(const size_t& newPointsPerPeriod);

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
