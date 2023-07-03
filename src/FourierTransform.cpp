#include "FourierTransform.h"

using base_wave_t    = BaseSineWave;
using size_vec_t     = std::vector<size_t>;
using complex_t      = std::complex<double>;
using complex_vec_t  = std::vector<complex_t>;
using ring_complex_t = boost::circular_buffer<complex_t>;

FourierTransform::FourierTransform(
    const base_wave_t* initBaseSineWave,
    const size_vec_t&  initHarmonics,
    double             initAngle,
    double             initAmplitude)
: _harmonics   (_initHarmonics(initHarmonics))
, _result      (_initResult(initHarmonics))
, _baseSineWave(_initBaseSineWave(initBaseSineWave))
, _instant     (_initInstant(initBaseSineWave))
, _correction  (_initCorrection(initAngle,  initAmplitude))
, _angle       ( initAngle)
, _amplitude   ( initAmplitude)
, _valid       ( Valid(initBaseSineWave))
{
    //ctor

    std::cout << _instant.size() << "\n";



};


FourierTransform::~FourierTransform()
{
    //dtor
};


//������������ �� ����� ��������� ���������
void FourierTransform::setNewBase(const base_wave_t* newBaseSineWave)
{
    //��������� ������� ����������� � ����� � ��������� �� ����� ����
    _result       = _initResult(_harmonics);

     //������� ���������
    _baseSineWave = _initBaseSineWave(newBaseSineWave);

     //��������� ������� ������� ���������� ��������
    _instant      = _initInstant(newBaseSineWave);

     //�������� ������� ���������
    _correction   = _initCorrection(_angle, _amplitude);

     //���������� ���������� ����������
    _valid        =  Valid(newBaseSineWave);

};


//��������� ������ ����������� ��������
void FourierTransform::setNewHarmonicalSet(const size_vec_t& newHarmonicalSet)
{
    _harmonics = _initHarmonics(newHarmonicalSet);
    _result    = _initResult(newHarmonicalSet);
};


//��������� ������������ ��������� ���������
void FourierTransform::setCorrection(double angle, double amplitude)
{
    _angle      =  angle;
    _amplitude  =  amplitude;
    _correction = _initCorrection(angle, amplitude);
};


//��������� �������� ������������ �������� ���������
const complex_t& FourierTransform::getData(size_t harmonic) const noexcept
{
    auto itFound =  std::find(_harmonics.begin(), _harmonics.end(), harmonic);

    if (itFound != _harmonics.end()) {

        size_t  index = std::distance(_harmonics.begin(), itFound);

        return _result[index];
    };

    return _zero;
};


//��������� ������� ����������� �������� ������ ��������
const complex_vec_t& FourierTransform::getData() const noexcept
{
    return _result;
};


//�������� ���������� ��������� �� ������� ���������
bool FourierTransform::isValid() const noexcept
{
    return _valid.isValid(_baseSineWave);
}


//������������� ������� ������� ��������
size_vec_t FourierTransform::_initHarmonics(const size_vec_t& initHarmonics)
{
    if (initHarmonics.size() == 0) {
        throw std::logic_error{"There is no harmonic set to calculate"};
    };

    return initHarmonics;
};


//������������� ������� ����������� �������� ��������
complex_vec_t FourierTransform::_initResult(const size_vec_t& initHarmonics)
{
    try {
        return complex_vec_t(initHarmonics.size(), _zero);
    } catch (...) {
        throw;
    };
};


//������������� ��������� ���������
const base_wave_t* FourierTransform::_initBaseSineWave(
                       const base_wave_t* initBaseSineWave)
{
    if (!initBaseSineWave) {
        throw std::logic_error{"Empty pointer to reference sine wave"};
    };

    if (!(initBaseSineWave->size())) {
        throw std::logic_error{"Empty reference sine wave"};
    };

    return initBaseSineWave;
};


//������������ ������� ���������� ��������
ring_complex_t  FourierTransform::_initInstant(
                    const base_wave_t* initBaseSineWave)
{
    try {
        return ring_complex_t(_baseSineWave->size(), _zero);
    } catch (...) {
        throw;
    }
};


//������������� ������������ ��������� ��� ��������� �������� �������
complex_t FourierTransform::_initCorrection(
              double newAngle, double newAmplitude) noexcept
{
    complex_t tmpRotate;

    try {
        tmpRotate = std::exp(complex_t{defaultZero(),
                                       base_wave_t::Pi::degToRad(newAngle)});
    } catch (...) {
        tmpRotate = _one;
    };

    return 2.0 * newAmplitude * tmpRotate * (1.0 / _baseSineWave->size());
};
