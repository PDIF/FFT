#include "Convolution.h"

using size_vec_t    = std::vector<size_t>;
using size_matrix_t = std::vector<size_vec_t>;


Convolution::Convolution(size_t initPointsPerPeriod)
: _primeFactors(_calculatePrimeFactors(initPointsPerPeriod))
, _nodes       (_calculateNodes(_primeFactors))
, _full        (_calculateFull(_primeFactors))
, _degrees     (_calculateDegrees(_primeFactors))
{
    //ctor
};



void Convolution::rebuild(size_t newPointsPerPeriod)
{
    _primeFactors = _calculatePrimeFactors(newPointsPerPeriod);
    _nodes        = _calculateNodes(_primeFactors);
    _full         = _calculateFull(_primeFactors);
    _degrees      = _calculateDegrees(_primeFactors);
};



const size_vec_t& Convolution::operator[](size_t index) const
{
    return _nodes[index];
};


size_t Convolution::length() const
{
    return _nodes.back().back() + 1;
};


size_t Convolution::size() const
{
    return _nodes.size();
};


const size_matrix_t& Convolution::nodes() const
{
    return _nodes;
};


const size_vec_t& Convolution::degrees() const
{
    return _degrees;
};

size_t Convolution::degrees(size_t index) const
{
    return _degrees[index];
};


const size_vec_t& Convolution::primeFactors() const
{
    return _primeFactors;
};


const size_vec_t& Convolution::expand() const
{
    return _full;
};



size_vec_t Convolution::_calculatePrimeFactors(size_t pointsPerPeriod)
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



size_matrix_t Convolution::_calculateNodes(const size_vec_t& newPrimeFactor)
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



size_vec_t Convolution::_calculateFull(const size_vec_t& newPrimeFactor)
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



size_vec_t Convolution::_calculateDegrees(const size_vec_t& newPrimeFactor)
{
    size_vec_t tmpDegrees(newPrimeFactor);

    size_t multiplier = 1;
    for (size_t i = 0; i < tmpDegrees.size(); ++i) {
        tmpDegrees[i]  = multiplier;
        multiplier    *= newPrimeFactor[i];
    }
    return tmpDegrees;
};


Convolution::~Convolution()
{
    //dtor
}
