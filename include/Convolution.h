#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <vector>
#include <assert.h>

class Convolution
{
    using size_vec_t    = std::vector<size_t>;
    using size_matrix_t = std::vector<size_vec_t>;

public:

    Convolution(size_t initPointsPerPeriod);

    virtual ~Convolution();

    void            rebuild(size_t newPointsPerPeriod);

    size_t          length() const;

    size_t          size() const;

    size_t          degrees(size_t index) const;

    const
    size_vec_t&     operator[](size_t index) const;

    const
    size_matrix_t&  nodes() const;

    const
    size_vec_t&     degrees() const;

    const
    size_vec_t&     primeFactors() const;

    const
    size_vec_t&     expand() const;


private:

    size_vec_t      _primeFactors;
    size_matrix_t   _nodes;
    size_vec_t      _full;
    size_vec_t      _degrees;

    //Methods
    size_vec_t      _calculatePrimeFactors(size_t pointsPerPeriod);

    size_matrix_t   _calculateNodes(const size_vec_t& newPrimeFactor);

    size_vec_t      _calculateFull(const size_vec_t& newPrimeFactor);

    size_vec_t      _calculateDegrees(const size_vec_t& newPrimeFactor);

};

#endif // CONVOLUTION_H

