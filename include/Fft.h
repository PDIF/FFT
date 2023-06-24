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
        , _expand  (_computeExpand(_position))
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
                    std::cout << data[i] << " ";
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


    ///Переключение на новую эталонную синусоиду
    virtual void setNewBase(const base_wave_t* newBaseSineWave) override;


    ///Установка набора вычисляемых гармоник
    virtual void setNewHarmonicalSet(const size_vec_t& newSet) override;


    //dtor
    virtual ~Fft();


private:

    ///Макет свертки
    Convolution     _convolutionLayout;//

    ///Макет базы
    Base            _baseLayout;//

    ///Матрица свертки
    ring_base_t     _convolutionData;

    ///Матрица базы
    ring_base_t     _baseData;//

    ///Результат расчета матрицы базы
    complex_vec_t   _baseResult;//




    ///Обновление элементов базы
    void _updateBase(const complex_t& newValue) {

        //Обновление результатов расчета базы
        for (size_t i = 0; i < _baseLayout.size(); ++i) {

           _baseResult[i]      =  newValue;
            size_t column      = _baseLayout.step() - 1;

            for (const auto& row : _baseLayout[i]) {
               _baseResult[i] += _baseData[row][column];
                column        += _baseLayout.step();
            };
        };

        //Добавление нового значения в матрицу базы
        size_t degree = 0;

        for (auto& row : _baseData) {
            row.push_front(newValue * (*_baseSineWave)[degree]);
            degree += _baseLayout.step();
        };
    };


    ///Обновление вектора результатов вычисления гармоник и матрицы свертки
    void _updateResult() {


        for (size_t i = 0; i < _harmonics.size(); ++i) {

            size_t    currentHarmonic = _harmonics[i];
            auto&     sourceHarmonic  = _convolutionData[i];
            complex_t resultHarmonic  = _baseResult[currentHarmonic %
                                                   _baseLayout.size()];

            //Обновление результатов расчета текущей гармоники
           _result[i] = resultHarmonic;

            for (const auto& position : _convolutionLayout.expand()) {
               _result[i] += sourceHarmonic[position];
            };

            //Обновление матрицы свертки
            complex_t adding  =  resultHarmonic *
                                 (*_baseSineWave)[currentHarmonic];
            const auto& _size = _baseSineWave->size();

            for (size_t j = 0; j + 1 < _convolutionLayout.size(); ++j) {

                const auto& positionSet = _convolutionLayout[j];

                for (size_t k = 0; k + 1 < positionSet.size(); ++k) {

                    size_t     degree   = positionSet.degree *
                                          currentHarmonic % _size;
                    const auto position = positionSet[k];
                    resultHarmonic     += sourceHarmonic[position];
                    sourceHarmonic[position] *= (*_baseSineWave)[degree];
                };

                size_t degree   = _convolutionLayout[j + 1].degree *
                                   currentHarmonic % _size;

                auto position   =  positionSet[positionSet.size() - 1];
                resultHarmonic +=  sourceHarmonic[position];
                sourceHarmonic[position] = resultHarmonic *
                                           (*_baseSineWave)[degree];
            };


            size_t degree = _convolutionLayout[_convolutionLayout.size() - 1].degree * currentHarmonic % _size;

            for (size_t k = 0; k + 1 < _convolutionLayout[_convolutionLayout.size() - 1].size(); ++k) {

               auto ppp = _convolutionLayout[_convolutionLayout.size() - 1][k];

               sourceHarmonic[ppp] *= _baseSineWave->operator[](degree);

            };

            sourceHarmonic.push_front(adding);// * (*_baseSineWave)[harmonic]);

        };
    };



//    //ссылка на эталонную синусоиду
//    //base_wave_t&    _baseSineWave;
//
//    //вектор искомых гармоник
//    size_vec_t      _harmonics;
//
//    //матрица значений, умноженных на комплексы эталонной синусоиды
//    ring_base_t     _base;
//
//    //матрица сверток по гармоникам
//    ring_base_t     _convolution;
//
//    //вектор модготовленных мгновенных значений по гармоникам
//    complex_vec_t   _current;
//
//    //результат ЦОС (вектор искомых значений по гармоникам)
//    complex_vec_t   _value;
//
//    //множитель коррекции новой (входящей) величины
//    complex_t       _valueCorrection;
//
//    //дефолтное нулевое комплексное значение
//    const complex_t _defaultComplex;
//
//
//    //Методы
//    size_vec_t      _initNodes();
//
//    complex_vec_t   _initComplexVector(size_t newSize);
//
//    ring_base_t     _initConvolution(size_t newSize);
//
//    ring_base_t     _initBase();
//
//    complex_t       _initCorrection(double angle, double amplitude);
//
//    void            _updateCurrent(const complex_t& newValue);
//
//    void            _updateBase(complex_t newValue);
//
//    void            _updateResult();
//
//    void            _updateConvolution();


    static constexpr double defaultAngleCorrection() {
        return 0.0;
    };

    static constexpr double defaultAmplitudeCorrection() {
        return 1.0;
    };

    static constexpr complex_t zeroComplex() {
        return complex_t(0.0, 0.0);
    };







    static const size_vec_t defaultHarmonicsFft() {
        return size_vec_t{1, 2, 3, 5};
    };


};



#endif // Fft_H
