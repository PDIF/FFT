#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <vector>
#include <assert.h>

//#include <map>
//#include <iostream>

template <typename T>
class RingBuffer
{
public:

    ///Конструктор
    RingBuffer(size_t initBufferSize)
    : _bufferSize(initBufferSize)
    , _current   (defaultCurrent())
    , _data      (std::vector<T>(_bufferSize, static_cast<T>(0)))
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
        return _bufferSize;
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

    size_t         _bufferSize;
    size_t         _current;
    std::vector<T> _data;


    size_t _setBufferIndex(size_t index) noexcept
    {
        assert((index < _bufferSize) && "Index exceeds buffer size");

        index += _current;
        if (index < _bufferSize) {
            return index;
        };

        index -= _bufferSize;
        return index;
    };


    void _moveCurrent() noexcept
    {

        if (_current > 0) {
            --_current;
            return;
        };

       _current = _bufferSize - 1;


    };


    static constexpr size_t defaultCurrent() noexcept {
        return 0;
    }

};

#endif // RINGBUFFER_H
