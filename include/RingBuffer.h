#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <vector>
#include <assert.h>

//#include <map>
//#include <iostream>

template <typename T>
class RingBuffer
{
    //using size_map_t = std::map<size_t, size_t>;
    using size_vec_t = std::vector<size_t>;

public:

    ///Конструктор
    RingBuffer(size_t initBufferSize)
    //: _currentPosition(_setCurrentPosition(initBufferSize))
    : _currentPosition(_setCurrentPosition(initBufferSize))
    //: _bufferSize(initBufferSize)
    , _current   (defaultCurrent())
    , _data      (std::vector<T>(_currentPosition.size(), static_cast<T>(0)))
    //, _data      (std::vector<T>(_bufferSize, static_cast<T>(0)))
    { };

    virtual ~RingBuffer()
    { };
/*
    ///Переназначение размеров буфера
    void setBuffer(size_t newBufferSize)
    {
        _bufferSize =  newBufferSize;
        //_currentPosition = _setCurrentPosition(newBufferSize);

        _current    =  defaultCurrent();
        //_data       =  std::vector<T>(_currentPosition.size(), static_cast<T>(0));
        _data       =  std::vector<T>(_bufferSize, static_cast<T>(0));
    };
*/
    ///Размер буфера
    size_t size()  const noexcept
    {

        //return _bufferSize;
        return _currentPosition.size();
    };

    ///Добавление нового значения в начало буфера
    void push_front(const T& newData) noexcept
    {
       _moveCurrent();
       _data[_current] = newData;
    };

    ///Добавление массива значений в начало буфера
    void push_front(const std::vector<T>& newData) noexcept
    {
        for (const auto & i : {newData.crbegin(), newData.crend()}) {
            push_front(i);
        };
    };

    ///Обращение к отдельному элементу массива (0 - самый новый элемент)
    T& operator[](size_t index) noexcept {
        return _data[_setBufferIndex(index)];
    };

    const T& operator[](size_t index) const noexcept {
        return _data[_setBufferIndex(index)];
    };

private:

    //size_map_t     _currentPosition;
    size_vec_t     _currentPosition;

    //size_t         _bufferSize;
    size_t         _current;
    std::vector<T> _data;



    size_t _setBufferIndex(size_t index) noexcept
    {


        size_t _size = _currentPosition.size();
        assert((index < _size) && "Index exceeds buffer size");

        index += _current;
        if (index < _size) {
            return index;
        };

        index -= _size;
        return index;

/*
        assert((index < _bufferSize) && "Index exceeds buffer size");

        index += _current;
        if (index < _bufferSize) {
            return index;
        };

        index -= _bufferSize;
        return index;
*/
    };


    size_vec_t _setCurrentPosition(size_t newSize) noexcept
    //size_map_t _setCurrentPosition(size_t newSize)
    {
        assert(newSize > 0 && "NewSize too small");

        //size_map_t tmpCurrentPosition;
        size_vec_t tmpCurrentPosition(1, newSize - 1);

        //tmpCurrentPosition[0] = newSize - 1;

        for (size_t current = 0; current < newSize - 1; ++current) {
            //tmpCurrentPosition[current] = current - 1;
            tmpCurrentPosition.push_back(current);
        };

        return tmpCurrentPosition;
    };


    void _moveCurrent() noexcept
    {
        _current = _currentPosition[_current];
    };


/*

    void _moveCurrent() noexcept
    {

        if (_current > 0) {
            --_current;
            return;
        };

       _current = _bufferSize - 1;


    };
*/


    static constexpr size_t defaultCurrent() noexcept {
        return 0;
    }

};

#endif // RINGBUFFER_H
