#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <vector>
#include <stdexcept>

template <typename T>
class RingBuffer
{
public:

    ///Конструктор
    RingBuffer(size_t initBufferSize)
    : _bufferSize(_setBufferSize(initBufferSize))
    , _data      (_setDataVector(initBufferSize))
    , _current   ( defaultCurrent())
    { };

    virtual ~RingBuffer()
    { };

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


    ///Установка корректного размера буфера
    size_t _setBufferSize(size_t newBufferSize)
    {
        if (newBufferSize < minSize()) { //(newBufferSize > maxSize() || newBufferSize < minSize()) {
            throw std::length_error{"Exceeding the allowed buffer size"};
        };
        return newBufferSize;
    };


    ///Создание вектора для хранения данных
    std::vector<T> _setDataVector(size_t newBufferSize)
    {
        return std::vector<T>(newBufferSize, static_cast<T>(0));
    };


    ///Вычисление индекса текущего элемента
    size_t _setBufferIndex(size_t index)
    {
        if (index >= _bufferSize) {
            throw std::out_of_range{"Index exceeding the allowed buffer size"};
        };

        index += _current;
        return index < _bufferSize ? index : index - _bufferSize;
    };


    ///Изменение значения указателя на начальный элемент буфера
    void _moveCurrent() noexcept
    {
        if (_current != 0) {
            --_current;
            return;
        };

       _current = _bufferSize - 1;

    };


    ///Текущий индекс по умолчанию
    static constexpr size_t defaultCurrent() noexcept
    {
        return 0;
    };



    /*
    ///Максимально допустимый размер буфера
    static constexpr size_t maxSize() noexcept
    {
        // Максимальное значение точек на период, согласно корпоративному
        // профилю, составляет 288. Принимаем максимально допустимое значение
        // с небольшим запасом, округлив величину до 300
        return 300;
    };
    */

    ///Минимально допустимый размер буфера
    static constexpr size_t minSize() noexcept
    {
        // Должна быть, как минимум, одна точка
        return 1;
    };

};

#endif // RINGBUFFER_H
