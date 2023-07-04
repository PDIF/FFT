#include "FourierTransform.h"

using base_wave_t    = BaseSineWave;
using size_vec_t     = std::vector<size_t>;
using complex_t      = std::complex<double>;
using ring_complex_t = boost::circular_buffer<complex_t>;
using complex_map_t  = std::map<size_t, complex_t>;


using complex_map_t  = std::map<size_t, complex_t>;
using size_set_t     = std::set<size_t>;



FourierTransform::FourierTransform(
    const base_wave_t* initBaseSineWave,
    const size_vec_t&  initHarmonics,
    double             initAngle,
    double             initAmplitude)
: _result      (_initResult(initHarmonics))
, _baseSineWave(_initBaseSineWave(initBaseSineWave))
, _instant     (_initInstant(initBaseSineWave))
, _correction  (_initCorrection(initAngle,  initAmplitude))
, _angle       ( initAngle)
, _amplitude   ( initAmplitude)
, _valid       ( Valid(initBaseSineWave))
{
    //ctor
};


FourierTransform::~FourierTransform()
{
    //dtor
};


//��������� ������ ����������� ��������
void FourierTransform::setNewHarmonicalSet(const size_vec_t& newHarmonicalSet)
{
//    _harmonics = _initHarmonics(newHarmonicalSet);
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
const complex_t& FourierTransform::getData(size_t harmonic) const
{
    if (_result.find(harmonic) != _result.end()) {
        return _result.at(harmonic);
    };

    throw std::logic_error{"There is no calculating harmonic"};
};


//��������� ������� ����������� �������� ������ ��������
const complex_map_t& FourierTransform::getData() const
{
    return _result;
};



//==================
// ��������� ������
//==================

//������������ �� ����� ��������� ���������
void FourierTransform::setNewBase(const base_wave_t* newBaseSineWave)
{
    //��������� ������� ����������� � ����� � ��������� �� ����� ����
    _clearResult();

     //������� ���������
    _baseSineWave = _initBaseSineWave(newBaseSineWave);

     //��������� ������� ������� ���������� ��������
    _instant      = _initInstant(newBaseSineWave);

     //�������� ������� ���������
    _correction   = _initCorrection(_angle, _amplitude);

     //���������� ���������� ����������
    _valid        =  Valid(newBaseSineWave);

};



//===================
// ���������� ������
//===================

//�������� ���������� ��������� �� ������� ���������
bool FourierTransform::isValid() const noexcept
{
    return _valid.isValid(_baseSineWave);
}



//==================
// ��������� ������
//==================

//������� ������� ����������� �������� ��������
void FourierTransform::_clearResult() noexcept
{
    for (auto& harmonic : _result) {
        harmonic.second = _zero;
    }
};


//������������� ������� ����������� �������� ��������
complex_map_t FourierTransform::_initResult(const size_vec_t& initVecHarmonics)
{
    try {

        complex_map_t tmpResult;
        for (auto const& harmonic: initVecHarmonics) {
            tmpResult[harmonic] = _zero;
        };
        return tmpResult;

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

