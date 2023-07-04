#ifndef FOURIERTRANSFORM_H
#define FOURIERTRANSFORM_H

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <map>

#include "boost/circular_buffer.hpp"

#include "BaseSineWave.h"

#include <iostream>


class FourierTransform
{
    using base_wave_t    = BaseSineWave;
    using size_vec_t     = std::vector<size_t>;
    using complex_t      = std::complex<double>;
    using complex_vec_t  = std::vector<complex_t>;
    using complex_map_t  = std::map<size_t, complex_t>;
    using ring_complex_t = boost::circular_buffer<complex_t>;


    ///����� ��� �������� ���������� ������� ���������
    class Valid
    {
    public:

        Valid(const base_wave_t* initValidWave) noexcept
        : _validWave( initValidWave)
        , _size     ( initValidWave->size())
        { };

        bool isValid(const base_wave_t* checkedWave) const noexcept {

            return (_validWave == checkedWave)   &&
                   (_size == checkedWave->size() );
        };

    private:
        const base_wave_t* _validWave;
        size_t             _size;
    };

public:

    //========
    // ������
    //========

    FourierTransform(
        const base_wave_t* initBaseSineWave,
        const size_vec_t&  initHarmonics    = defaultHarmonics(),
        double             initnAngle       = defaultZero(),
        double             initAmplitude    = defaultOne());

    virtual ~FourierTransform();


    ///���������� ����� �������� � ���������� ������
    virtual void update(double newValue) = 0;


    ///������������ �� ����� ��������� ���������
    virtual void setNewBase(const base_wave_t* newBaseSineWave) = 0;


    ///��������� ������ ����������� ��������
    virtual void setNewHarmonicalSet(const size_vec_t& newSet) = 0;


    ///��������� ������������ ��������� ���������
    virtual void setCorrection(double angle = 0.0, double amplitude = 1.0);


    ///��������� �������� ������������ �������� ���������
    const complex_t& getData(size_t harmonic) const;


    ///��������� ������� ����������� �������� ������ ��������
    const complex_map_t& getData() const;

protected:

    //========
    //  ����
    //========

    ///������ ����������� ���������� ��������
    complex_map_t   _result;


    ///��������� �� ��������� ���������
    const
    base_wave_t*    _baseSineWave;


    ///������ ���������� ��������
    ring_complex_t  _instant;


    ///����������� ��������� ��������� �������� �������
    complex_t       _correction;


    ///����������� ����
    static constexpr complex_t _zero = complex_t(0.0, 0.0);


    ///����������� �������
    static constexpr complex_t _one  = complex_t(1.0, 0.0);


    //========
    // ������
    //========

    ///�������� ���������� ��������� �� ������� ���������
    bool isValid() const noexcept;


    ///�������� �������������� ����� ����������� ��������
    static const size_vec_t defaultHarmonics() {
        return size_vec_t{0, 1, 2, 3, 4, 5, 6, 7, 9, 11, 12, 13};
    };


    static constexpr double defaultZero() noexcept {
        return 0.0;
    };

    static constexpr double defaultOne() noexcept {
        return 1.0;
    };

    static constexpr double zeroHarmonicalMultiplier() noexcept {
        return 0.5;
    };

private:

    //========
    //  ����
    //========

    //�������������� ���������, �� ��������� �� ������� ������� ���������
    double    _angle;
    double    _amplitude;

    //�������� ����������
    Valid     _valid;

    //========
    // ������
    //========

    ///������� ������� ����������� �������� ��������
    void               _clearResult() noexcept;


    ///������������� ������� ����������� �������� ��������
    complex_map_t      _initResult(const size_vec_t& initVecHarmonics);


    ///������������� ��������� ���������
    const base_wave_t* _initBaseSineWave(const base_wave_t* initBaseSineWave);


    ///������������ ������� ���������� ��������
    ring_complex_t     _initInstant(const base_wave_t* initBaseSineWave);


    ///������������� ������������ ��������� ��� ��������� �������� �������
    complex_t          _initCorrection(double angle, double amplitude) noexcept;

};

#endif // FOURIERTRANSFORM_H
