#ifndef Fft_H
#define Fft_H


#include <complex>
#include <vector>

#include "ReferenceSineWave.h"
#include "BaseSineWave.h"
#include "FourierTransform.h"

#include "boost/circular_buffer.hpp"

class Fft : public FourierTransform
{
    using base_wave_t    = BaseSineWave;
    using size_vec_t     = std::vector<size_t>;
    using size_matrix_t  = std::vector<size_vec_t>;
    using complex_t      = std::complex<double>;
    using complex_vec_t  = std::vector<complex_t>;
    //using ring_complex_t = RingBuffer<complex_t>;
    using ring_complex_t = boost::circular_buffer<complex_t>;

    using ring_base_t    = std::vector<ring_complex_t>;

    //���������� �������� ������ ��� ����������� � ���� ������������� �������
    class Base;

    ///����� ������ �������
    class Convolution
    {
        //������������� �����
        friend class Base;

        ///��������� ��� �������� ������ �������� �������
        struct Data
        {
            ///������� �������� ������� ��� ������ ���������
            const size_t  power;

            ///������ ������� ��������� ������� �������
            const size_vec_t position;

        };

        using data_vec_t     = std::vector<Data>;

    public:

        Convolution(const base_wave_t* initBaseSineWave)
        : _position(_computePosition(initBaseSineWave))
        , _expand  (_computeExpand(_position))
        { };

        ///����� ������ ������� ��� �������� �������� �������
        const size_t length() const noexcept {
            return   _position.size() > 0
                   ? _position.back().position.back() + 1
                   :  0;
        };

        ///���������� �������� �������
        const size_t size() const noexcept {
            return _position.size();
        };

        ///���������� ������ ������� ��������� �������
        const Data& operator[](size_t index) const {
            return _position[index];
        };

        ///����� ������� ��� ���������� ������������
        const size_vec_t& expand() const noexcept {
            return _expand;
        };

    private:

        //========
        //  ����
        //========
        data_vec_t  _position;
        size_vec_t  _expand;
        //========
        // ������
        //========

        ///���������� ������� �������� ������� ����������� ��������� � ��������
        data_vec_t  _computePosition(const base_wave_t* newBaseSineWave) {

            size_vec_t  primeFactors = _computeFactors(newBaseSineWave);

            size_t      step = 1;
            size_t      base = 0;
            data_vec_t  tmpPosition;

            for (size_t i = 0; i < primeFactors.size(); ++i) {

                size_vec_t tmpAddress(primeFactors[i] - 1);

                for (size_t j = 0; j + 2 < primeFactors[i]; ++j) {
                    tmpAddress[j] = base;
                    base += step;
                };

                tmpAddress.back() = base;
                tmpPosition.push_back(Data{step, tmpAddress});
                step *= primeFactors[i];
                base += step;
            };

            return tmpPosition;
        };


        ///���������� ������� ������ ������� ��� ���������� ������������
        size_vec_t  _computeExpand(const data_vec_t& inputPosition) {

            size_vec_t tmpExpand;

            for (const auto& data : inputPosition) {
                for (size_t i = 0; i < data.position.size(); ++i) {
                    tmpExpand.push_back(data.position[i]);
                };
            };

            return tmpExpand;
        };


        ///���������� ������� ������� ����������
        static
        size_vec_t _computeFactors(const base_wave_t* newBaseSineWave) {


            size_vec_t tmpPrimeFactor;
            size_t currentSize       = newBaseSineWave->size();
            size_t currentMultiplier = 2;

            //assert(currentSize > 0);

            while (currentSize > 1) {
                if (!(currentSize % currentMultiplier)) {
                    currentSize /= currentMultiplier;
                    tmpPrimeFactor.push_back(currentMultiplier);
                } else {
                    ++currentMultiplier;
                };
            };

            //assert(tmpPrimeFactor.size() > 0);
            return {tmpPrimeFactor.crbegin() + 1, tmpPrimeFactor.crend()};
        };

    };

    ///����� ������ ����
    class Base
    {
    public:

        Base(const base_wave_t* initBaseSineWave)
        : _step(_computeStep(initBaseSineWave))
        , _data(_computeData(initBaseSineWave, _step))
        { };


        size_t size() {
            return _data.size();
        };

        size_t step() {
            return _step;
        };

        const size_vec_t& operator[](size_t index) {
            return _data[index];
        };

    private:

        size_t        _step;
        size_matrix_t _data;


        size_t _computeStep(const base_wave_t* newBaseSineWave) {

            size_vec_t primeFactors = Convolution::
                                     _computeFactors(newBaseSineWave);

            size_t tmpStep = 1;

            for (const auto& i : primeFactors) {
                tmpStep *= i;
            };

            return tmpStep;
        };


        size_matrix_t _computeData(const base_wave_t* newBaseSineWave,
                                   size_t newStep = 1) {

            size_t _size = newBaseSineWave->size() / newStep;

            size_matrix_t  tmpData(_size, size_vec_t(_size - 1));

            for (size_t i = 0; i < _size; ++i) {
                for (size_t j = 0; j < _size - 1; ++j) {
                    tmpData[i][j] = i * (j + 1) % _size;
                };
            };

            return tmpData;
        };
    };

public:

    Fft(const base_wave_t* initBaseSineWave = nullptr,
        const size_vec_t&  initHarmonics    = defaultHarmonicsFft(),
        double             initnAngle       = defaultZero(),
        double             initAmplitude    = defaultOne());

    ///���������� ����� �������� � ���������� ������
    void update(double newValue) override;

    ///������������ �� ����� ��������� ���������
    virtual void setNewBase(const base_wave_t* newBaseSineWave) override;

    ///��������� ������ ����������� ��������
    virtual void setNewHarmonicalSet(const size_vec_t& newSet) override;


    //dtor
    virtual ~Fft();


private:

    //========
    //  ����
    //========

    ///����� �������
    Convolution     _convolutionLayout;//

    ///����� ����
    Base            _baseLayout;//

    ///������� �������
    ring_base_t     _convolutionData;

    ///������� ����
    ring_base_t     _baseData;//

    ///��������� ������� ������� ����
    complex_vec_t   _baseResult;//


    //========
    // ������
    //========

    ///���������� ��������� ����
    void _updateBase(const complex_t& newValue) noexcept;

    ///���������� ������� ����������� ���������� �������� � ������� �������
    void _updateResult() noexcept;

    ///������������ ������� �������
    ring_base_t _initConvolutionData(size_t newHarmonicsNumber,
                                     size_t newConvolutionLength);
    ///������������ ������� ����
    ring_base_t _initBaseData(size_t newBaseLayoutSize,
                              size_t newBaseLayoutStep);
    ///������������ ������� ������� ������� ����
    complex_vec_t _initBaseResult(size_t newBaseLayoutSize);

    ///����� ����������� �������� �� ���������
    static const size_vec_t defaultHarmonicsFft() {
        return size_vec_t{0, 1, 2, 3, 5};
    };

};



#endif // Fft_H
