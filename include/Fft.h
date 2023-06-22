#ifndef Fft_H
#define Fft_H

#include <complex>
#include <vector>
#include <algorithm>

#include <assert.h>

#include <iostream>
#include "ReferenceSineWave.h"
#include "RingBuffer.h"


#include "FourierTransform.h"

class Fft : public FourierTransform
{
    using base_wave_t   = BaseSineWave;
    using size_vec_t     = std::vector<size_t>;
    using size_matrix_t     = std::vector<size_vec_t>;
    using complex_t      = std::complex<double>;
    using complex_vec_t  = std::vector<complex_t>;
    using ring_complex_t = RingBuffer<complex_t>;
    using ring_base_t    = std::vector<ring_complex_t>;

    //Объявление базового класса для подключения к нему дружественных методов
    class Base;

    ///Класс макета свертки
    class Convolution
    {
        //Дружественный класс
        friend class Base;

        ///Класс содержимого макета свертки
        class Data
        {
        public:

            Data(const size_t& initDegree, const size_vec_t& initPosition)
            :  degree(initDegree)
            , _position(initPosition)
            { };

            ///Степень элемента свертки для первой гармоники
            const size_t  degree;

            ///Позиция элемента свертки текущей степени
            const size_t& operator[](size_t index) const {
                return _position[index];
            };

            ///Количество позиций элементов свертки текущей степени
            size_t size() const {
                return _position.size();
            };

            ///Позиция последнего элемента свертки текущей степени
            size_t back() const{
                return _position.back();
            };

        private:
            ///Вектор позиций элементов текущей степени
            const size_vec_t _position;
        };

        using data_vec_t     = std::vector<Data>;

    public:

        Convolution(const base_wave_t* initBaseSineWave)
        : _position(_computePosition(initBaseSineWave))
        { };

        ///Общий размер вектора для хранения значений свертки
        const size_t length(){
            return _position.back().back() + 1;
        };

        ///Количество степеней свертки
        const size_t size() const {
            return _position.size();
        };

        ///Содержимое макета свертки отдельной степени
        const Data& operator[](size_t index) const {
            return _position[index];
        };

        ///Набор позиций для выполнения суммирования
        const size_vec_t& expand() const {
            return _expand;
        }
    private:

        //========
        //  Поля
        //========
        data_vec_t  _position;
        size_vec_t  _expand;
        //========
        // Методы
        //========

        ///Вычисление вектора векторов с позициями
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


        ///Вычисление единого набора позиций для выполнения суммирования
        size_vec_t  _computeExpand(const data_vec_t& inputPosition) {

            size_vec_t tmpExpand;
            //tmpExpand.resize(Convolution::size());

            for (const auto& data : inputPosition) {
                for (size_t i = 0; i < data.size(); ++i) {
                    tmpExpand.push_back(data[i]);
                };
            };

            return tmpExpand;
        };


        ///Вычисление вектора простых множителей
        static
        size_vec_t _computeFactors(const base_wave_t* newBaseSineWave) {


            size_vec_t tmpPrimeFactor;
            size_t     currentSize       = newBaseSineWave->size();
            size_t     currentMultiplier = 2;

            assert(currentSize > 0);

            while (currentSize > 1) {
                if (!(currentSize % currentMultiplier)) {
                    currentSize /= currentMultiplier;
                    tmpPrimeFactor.push_back(currentMultiplier);
                } else {
                    ++currentMultiplier;
                };
            };

            assert(tmpPrimeFactor.size() > 0);
            return {tmpPrimeFactor.crbegin() + 1, tmpPrimeFactor.crend()};
        };

    };

    ///Класс макета базы
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

    ///Добавление новой величины и обновление данных
    void update(double newValue) override;




    ///Вектор комплексных значений вектора гармоник
    const complex_vec_t& value() const;

    ///комплексное значение заданной гармоники
    const complex_t& value(size_t harmonic) const;

    ///установка коррекции значений
    void setCorrection(double angleDegree, double amplitude);

    //dtor
    virtual ~Fft();


private:

    ///Макет свертки
    Convolution     _convolution;//

    ///Макет базы
    Base            _base;//

    ///Матрица базы
    ring_base_t     _baseData;//

    ///Матрица свертки
    ring_base_t     _convolutionData;

    ///Результат расчета матрицы базы
    complex_vec_t   _resultBase;//




    ///Обновление элементов базы
    void _updateBase(complex_t newValue) {

        complex_t complexValue(FourierTransform::_correction * newValue);


        //Добавление нового значения в матрицу базы
        size_t _degree = 0;

        for (auto& degree : _baseData) {

            degree.push_front(complexValue * (*_baseSineWave)[_degree]);
           _degree += _base.step();
        };


        //Обновление результатов расчета базы
        for (size_t i = 0; i < _base.size(); ++i) {

            size_t baseColumn = 0;

           _resultBase[i] = _baseData[0][baseColumn];

            for (size_t j = 0; j + 1 < _base.size(); ++j) {

                baseColumn    += _base.step();
                size_t baseRow = _base[i][j];
               _resultBase[i] += _baseData[baseRow][baseColumn];
            };
        };
    };


    ///Обновление вектора результатов вычисления гармоник
    void _updateResult() {

        //Обновление результатов расчета гармоник
        for (size_t i = 0; i < _harmonics.size(); ++i) {

            size_t harmonic = _harmonics[i];
           _result[i] = _resultBase[harmonic % _base.size()];


            for (const auto& position : _convolution.expand()) {

                _result[i] += _convolutionData[i][position];
            };
        };


        //Обновление матрицы свертки
        for (size_t i = 0; i < _harmonics.size(); ++i) {

            size_t harmonic = _harmonics[i];

            for (size_t j = 0; j + 1 < _convolution.size(); ++j) {
                for (size_t k = 0; k + 1 < _convolution[j].size(); ++k) {

                    size_t degree = _convolution[j].degree  *
                                     harmonic               %
                                    _baseSineWave->size();

                   _convolution[j][k] *= (*_baseSineWave)[degree];
                };

                size_t degree = _convolution[j].degree  *



            }
        _convolutionData

        }
    };

/*

    //ссылка на эталонную синусоиду
    //base_wave_t&    _baseSineWave;

    //вектор искомых гармоник
    size_vec_t      _harmonics;

    //матрица значений, умноженных на комплексы эталонной синусоиды
    ring_base_t     _base;

    //матрица сверток по гармоникам
    ring_base_t     _convolution;

    //вектор модготовленных мгновенных значений по гармоникам
    complex_vec_t   _current;

    //результат ЦОС (вектор искомых значений по гармоникам)
    complex_vec_t   _value;

    //множитель коррекции новой (входящей) величины
    complex_t       _valueCorrection;

    //дефолтное нулевое комплексное значение
    const complex_t _defaultComplex;


    //Методы
    size_vec_t      _initNodes();

    complex_vec_t   _initComplexVector(size_t newSize);

    ring_base_t     _initConvolution(size_t newSize);

    ring_base_t     _initBase();

    complex_t       _initCorrection(double angle, double amplitude);

    void            _updateCurrent(const complex_t& newValue);

    void            _updateBase(complex_t newValue);

    void            _updateResult();

    void            _updateConvolution();


    static constexpr double defaultAngleCorrection() {
        return 0.0;
    };

    static constexpr double defaultAmplitudeCorrection() {
        return 1.0;
    };

    static constexpr complex_t zeroComplex() {
        return complex_t(0.0, 0.0);
    };


*/




    static const size_vec_t defaultHarmonicsFft() {
        return size_vec_t{1, 2, 3, 5};
    };


};



#endif // Fft_H
