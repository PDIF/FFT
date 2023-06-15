#ifndef REFERENCESINEWAVE_H
#define REFERENCESINEWAVE_H

#include <complex>
#include <vector>
#include "RingBuffer.h"

//#include <iostream>
#include <assert.h>

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

class Convolution
{
    using size_vec_t    = std::vector<size_t>;
    using size_matrix_t = std::vector<size_vec_t>;

public:

    Convolution(size_t initPointsPerPeriod)
    : _primeFactors(_calculatePrimeFactors(initPointsPerPeriod))
    , _nodes       (_calculateNodes(_primeFactors))
    , _full        (_calculateFull(_primeFactors))
    , _degrees     (_calculateDegrees(_primeFactors))
    { };


    void rebuild(size_t newPointsPerPeriod)
    {
        _primeFactors = _calculatePrimeFactors(newPointsPerPeriod);
        _nodes        = _calculateNodes(_primeFactors);
        _full         = _calculateFull(_primeFactors);
        _degrees      = _calculateDegrees(_primeFactors);
    };



    const size_vec_t& operator[](size_t index) const
    {
        return _nodes[index];
    };


    size_t length() const
    {
        return _nodes.back().back() + 1;
    };


    size_t size() const
    {
        return _nodes.size();
    };


    const size_matrix_t& nodes() const
    {
        return _nodes;
    };


    const size_vec_t& degrees() const
    {
        return _degrees;
    };

    size_t degrees(size_t index) const
    {
        return _degrees[index];
    };


    const size_vec_t& primeFactors() const
    {
        return _primeFactors;
    };


    const size_vec_t& expand() const
    {
        return _full;
    };

private:

    size_vec_t      _primeFactors;
    size_matrix_t   _nodes;
    size_vec_t      _full;
    size_vec_t      _degrees;



    size_vec_t _calculatePrimeFactors(size_t pointsPerPeriod)
    {

        assert(pointsPerPeriod > 0);

        size_vec_t tmpPrimeFactor;
        size_t currentMultiplier = 2;

        while (pointsPerPeriod > 1) {
            if (pointsPerPeriod %  currentMultiplier == 0) {
                pointsPerPeriod /= currentMultiplier;
                tmpPrimeFactor.push_back(currentMultiplier);
            } else {
                ++currentMultiplier;
            }
        }

        assert(tmpPrimeFactor.size() > 0);

        return {tmpPrimeFactor.crbegin() + 1, tmpPrimeFactor.crend()};

    };


    size_matrix_t _calculateNodes(const size_vec_t& newPrimeFactor)
    {
        size_matrix_t tmpNodes;

        size_t base = 0;
        size_t step = 1;
        for (auto factor : newPrimeFactor) {

            assert(factor > 0 && "Null primeFactor");

            size_vec_t tmpPosition(factor - 1);

            for (auto& node : tmpPosition) {
                base += step;
                node  = base - 1;

            }

            tmpNodes.push_back(tmpPosition);

            step *= factor;
        }

        return tmpNodes;

    };



    size_vec_t _calculateFull(const size_vec_t& newPrimeFactor)
    {
        size_vec_t tmpFull;

        size_t base = 0;
        size_t step = 1;
        for (auto factor : newPrimeFactor) {

            assert(factor > 0 && "Null primeFactor");
            for (size_t i = 0; i < factor - 1; ++i) {
                base += step;
                tmpFull.push_back(base - 1);

            }

            step *= factor;
            //base += step;
        }

        return tmpFull;

    };



    size_vec_t _calculateDegrees(const size_vec_t& newPrimeFactor)
    {
        size_vec_t tmpDegrees(newPrimeFactor);

        size_t multiplier = 1;
        for (size_t i = 0; i < tmpDegrees.size(); ++i) {
            tmpDegrees[i]  = multiplier;
            multiplier    *= newPrimeFactor[i];
        }
        return tmpDegrees;
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
