#ifndef BASESINEWAVE_H
#define BASESINEWAVE_H

#include <vector>
#include <complex>
#include <cmath>

#include <assert.h>

class BaseSineWave
{
    using complex_t     = std::complex<double>;
    using complex_vec_t = std::vector<complex_t>;

    ///����� ��� �������� ���� � ������� ������� ���������
    class Step
    {
        friend class BaseSineWave;

    public:

        ///���������� ����� (�����) �� ������
        size_t count() const noexcept {
            return _count;
        };

        ///�������� ���� � ��������
        double degree() const noexcept {
            return _degree;
        };

        ///�������� ���� � ��������
        double radian() const noexcept {
            return _radian;
        };

        ///����������� �������� ����
        complex_t complex() const noexcept {
            return _complex;
        };

    private:
        //=================
        //  �������� ����
        //=================
        size_t      _count;
        double      _degree;
        double      _radian;
        complex_t   _complex;

        //=================
        // �������� ������
        //=================
        ///����������� � ��������� ����� ����� �� ������
        Step(size_t initStep) noexcept
        : _count  ( initStep)
        , _degree (_computeDegree(initStep))
        , _radian (_computeRadian(initStep))
        , _complex(_computeComplex(initStep))
        { };

        ~Step()
        { };


        double _computeDegree(size_t newStep) noexcept
        {
            //assert (newStep > 0 && "Null step value in _computeDegree");
            return 2.0 * BaseSineWave::Pi::deg / newStep;

        };

        double _computeRadian(size_t newStep) noexcept
        {
            //assert (newStep > 0 && "Null step value in _computeRadian");
            return 2.0 * BaseSineWave::Pi::rad / newStep;
        };

        complex_t _computeComplex(size_t newStep) noexcept
        {
            //assert (newStep > 0 && "Null step value in _computeComplex");
            return std::exp(complex_t(0.0, -_computeRadian(newStep)));
        };

    };

public:

    ///��������� ��� �������� ����� ��
    struct Pi
    {
        Pi() noexcept
        { };

        ///�������� � ��������
        static constexpr double deg = 180.0;

        ///�������� � ��������
        static constexpr double rad = acos(-1.0);

        ///������� �������� � �������
        static double degToRad(double angle) noexcept {
            return angle * rad / deg;
        };

        ///������� ������ � �������
        static double radToDeg(double angle) noexcept {
            return angle * deg / rad;
        };

    };


    ///����������� � �������� ������� (����� ����� �� ������)
    BaseSineWave(size_t initSize = defaultSize());

    ///�������������� ���� ��������� ���������
    Step    step;

    ///������ ��������� ��������� (����� ����� �� ������)
    size_t  size()  const noexcept;

    ///�������� []
    const
    complex_t&  operator[](size_t index) const;


    virtual ~BaseSineWave();


private:

    //=================
    //  �������� ����
    //=================
    complex_vec_t   _sineWave;

    //=================
    // �������� ������
    //=================

    ///���������� ������� ����
    Step          _setStep(size_t initStep);

    ///������ ��������� ���������
    complex_vec_t _setWave(size_t initStep);


    ///����� ����� �� ������ � ������������ � ������������� ��������
    static constexpr size_t defaultSize() noexcept {
        return 96;
    };


    ///����������� ���������� ������ ������� ���������
    static constexpr size_t maxSize() noexcept
    {
        // ������������ �������� ����� �� ������, �������� ��������������
        // �������, ���������� 288. ��������� ����������� ���������� ��������
        // � ��������� �������, �������� �������� �� 300
        return 300;
    };

    ///���������� ���������� ������ ������� ���������
    static constexpr size_t minSize() noexcept
    {
        // ������ ����, ��� �������, ��� �����
        return 2;
    };

};

#endif // BASESINEWAVE_H
