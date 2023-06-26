#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <vector>
#include <stdexcept>

template <typename T>
class RingBuffer
{
public:

    ///�����������
    RingBuffer(size_t initBufferSize)
    : _bufferSize(_setBufferSize(initBufferSize))
    , _data      (_setDataVector(initBufferSize))
    , _current   ( defaultCurrent())
    { };

    virtual ~RingBuffer()
    { };

    ///������ ������
    size_t size()  const noexcept
    {
        return _bufferSize;
    };


    ///���������� ������ �������� � ������ ������
    void push_front(const T& newData) noexcept
    {
       _moveCurrent();
       _data[_current] = newData;
    };


    ///���������� ������� �������� � ������ ������
    void push_front(const std::vector<T>& newData) noexcept
    {
        for (const auto & i : {newData.crbegin(), newData.crend()}) {
            push_front(i);
        };
    };


    ///��������� � ���������� �������� ������� (0 - ����� ����� �������)
    T& operator[](size_t index)
    {
        return _data[_setBufferIndex(index)];
    };

    const T& operator[](size_t index) const
    {
        return _data[_setBufferIndex(index)];
    };

private:

    size_t         _bufferSize;
    std::vector<T> _data;
    size_t         _current;


    ///��������� ����������� ������� ������
    size_t _setBufferSize(size_t newBufferSize)
    {
        if (newBufferSize < minSize()) { //(newBufferSize > maxSize() || newBufferSize < minSize()) {
            throw std::length_error{"Exceeding the allowed buffer size"};
        };
        return newBufferSize;
    };


    ///�������� ������� ��� �������� ������
    std::vector<T> _setDataVector(size_t newBufferSize)
    {
        return std::vector<T>(newBufferSize, static_cast<T>(0));
    };


    ///���������� ������� �������� ��������
    size_t _setBufferIndex(size_t index)
    {
        if (index >= _bufferSize) {
            throw std::out_of_range{"Index exceeding the allowed buffer size"};
        };

        index += _current;
        return index < _bufferSize ? index : index - _bufferSize;
    };


    ///��������� �������� ��������� �� ��������� ������� ������
    void _moveCurrent() noexcept
    {
        if (_current != 0) {
            --_current;
            return;
        };

       _current = _bufferSize - 1;

    };


    ///������� ������ �� ���������
    static constexpr size_t defaultCurrent() noexcept
    {
        return 0;
    };



    /*
    ///����������� ���������� ������ ������
    static constexpr size_t maxSize() noexcept
    {
        // ������������ �������� ����� �� ������, �������� ��������������
        // �������, ���������� 288. ��������� ����������� ���������� ��������
        // � ��������� �������, �������� �������� �� 300
        return 300;
    };
    */

    ///���������� ���������� ������ ������
    static constexpr size_t minSize() noexcept
    {
        // ������ ����, ��� �������, ���� �����
        return 1;
    };

};

#endif // RINGBUFFER_H
