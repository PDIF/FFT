#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <vector>
#include <assert.h>

//#include <iostream>

template <typename T>
class RingBuffer
{
public:
    RingBuffer(
        const size_t& initWindowSize,
        const double& initNumberOfPeriods = defaultPeriodNumber())
    : _windowSize(initWindowSize)
    , _bufferSize(_computeBufferSize(initNumberOfPeriods))
    , _current   (defaultCurrent())
    , _data      (std::vector<T>(_bufferSize, static_cast<T>(0)))
    { };

    virtual ~RingBuffer()
    { };

    size_t getWindowSize()  const
    {
        return _windowSize;
    };

    size_t getBufferSize() const
    {
        return _bufferSize;
    };

    void push_front(const T& newData)
    {
       _moveCurrent();
       _data[_current] = newData;
    };

    void push_front(const std::vector<T>& newData)
    {
        for (const auto & i : {newData.crbegin(), newData.crend()}) {
            push_front(i);
        };
    };

    T& operator[](size_t index) {
        return _data[_setBufferIndex(index)];
    };

    const T& operator[](size_t index) const {
        return _data[_setBufferIndex(index)];
    };

private:

    size_t         _windowSize  ;
    size_t         _bufferSize  ;
    size_t         _current     ;
    std::vector<T> _data        ;



    size_t _setBufferIndex(size_t index) {

        assert((index < _bufferSize) && "Index exceeds buffer size");

        index += _current;
        if (index < _bufferSize) {
            return index;
        }

        index -= _bufferSize;
        return index;
    };


    void _moveCurrent()
    {
        if (_current > 0) {
            --_current;
            return;
        };

        _current = _bufferSize - 1;
    };


    size_t _computeBufferSize(double newNumberOfPeriods) const
    {
        return static_cast<size_t>(newNumberOfPeriods * _windowSize);
    };

    static constexpr size_t defaultCurrent() {
        return 0;
    };

    static constexpr double defaultPeriodNumber() {
        return 1.0;
    };



};

#endif // RINGBUFFER_H
