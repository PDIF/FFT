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

    ///перестроение эталонной свертки
    void            rebuild(size_t newPointsPerPeriod);

    ///длина буфера для хранения свертки от одной гармоники
    size_t          length() const;

    ///количество простых множителей, из которых состоят степени в свертке
    size_t          size() const;


    ///вектор индексов элементов в пределах одного простого множителя свертки,
    ///которые нужно суммировать при расчете
    const
    size_vec_t&     operator[](size_t index) const;

    ///матрица (вектор векторов) индексов элементов простых множителей свертки
    const
    size_matrix_t&  nodes() const;

    ///вектор степеней, из которых состоит свертка
    const
    size_vec_t&     degrees() const;

    ///значение степени, имеющей индекс index
    size_t          degrees(size_t index) const;

    ///вектор простых множителей
    const
    size_vec_t&     primeFactors() const;

    ///вектор индексов которые нужно суммировать при расчете
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

