#include "SignalGenerate.h"

//using component_t = std::array<double, static_cast<size_t>(SignalGenerate::Location::SIZE)>;

/*
//template<typename T>
//SignalGenerate<T>::
SignalGenerate::SignalGenerate()
{
    //ctor
}
//template<typename T>
//SignalGenerate<T>::
SignalGenerate::~SignalGenerate()
{
    //dtor
}





void SignalGenerate::addExponentialData(std::vector<double>& data,
                                        double amplitude         ,
                                        double lambda            ) {
    double currentTime = start_;
    for (size_t i = 0; i < size_; ++i) {

        currentTime += step_;
        data[i] = amplitude * exp(-lambda * currentTime);

    }
}

void SignalGenerate::addSinusoidalData (std::vector<double>& data,
                                        double amplitude         ,
                                        double frequency         ,
                                        double phase             ) {
    double currentTime = start_;
    for (size_t i = 0; i < size_; ++i) {

        currentTime += step_;
        data[i] = amplitude * cos(frequency * currentTime + phase);
    }
}



std::vector<double> SignalGenerate::getData(size_t position) {

    assert(position < data_.size() && position >= 0);

    auto  coefficientsSet = data_[position];
    std::vector<double> data(size_);

    double coefficientFrequency   = coefficientsSet[Location::FREQUENCY]   ;
    double coefficientAmplitude   = coefficientsSet[Location::AMPLITUDE]   ;
    double coefficientPhaseLambda = coefficientsSet[Location::PHASE_LAMBDA];

    if (coefficientFrequency != 0.0) {
        addSinusoidalData(data                   ,
                          coefficientAmplitude   ,
                          coefficientFrequency   ,
                          coefficientPhaseLambda );
    } else {
        addExponentialData(data                  ,
                           coefficientAmplitude  ,
                           coefficientPhaseLambda);
    }

    return data;
}




std::vector<double> SignalGenerate::getData(double inputFrequency) {


    //Output data initialization
    std::vector<double> result(size_);

    //Frequency search
    auto itData = data_.begin();
    while (itData != data_.end()) {

        size_t position = static_cast<size_t>(Location::FREQUENCY);
        if ((*itData)[position] == inputFrequency) {

        }



        ++itData;
    }













    //Each moment time calculation
    for (size_t i = 0; i < vectorSize; ++i) {

        //Current time
        double currentTime = start_ + step_ * i;

        for (auto & j : data_) {

            double frequency = j[static_cast<size_t>(Location::FREQUENCY)];
            double amplitude = j[static_cast<size_t>(Location::AMPLITUDE)];
            double phase     = j[static_cast<size_t>(Location::PHASE)];

            double instant   = frequency != 0.0
                               ? amplitude * cos(phase      +
                                                 PI_TWICE   *
                                                 frequency  *
                                                 currentTime)
                               : amplitude * exp(-phase * currentTime);

            instantSum += instant;
        }

        result[i] = instantSum;
    }




}



std::vector<double> SignalGenerate::get() {

    size_t vectorSize;
    try {
        vectorSize = (end_ - start_) / step_;
    } catch (...) {
        throw;
    }


    //Output data initialization
    std::vector<double> result(vectorSize);

    //Each moment time calculation
    for (size_t i = 0; i < vectorSize; ++i) {

        //Current time
        double currentTime = start_ + step_ * i;

        //Each frequency value calculation
        double instantSum = 0.0;

        for (auto & j : data_) {

            double frequency = j[static_cast<size_t>(Location::FREQUENCY)];
            double amplitude = j[static_cast<size_t>(Location::AMPLITUDE)];
            double phase     = j[static_cast<size_t>(Location::PHASE)];

            double instant   = frequency != 0.0
                               ? amplitude * cos(phase      +
                                                 PI_TWICE   *
                                                 frequency  *
                                                 currentTime)
                               : amplitude * exp(-phase * currentTime);

            instantSum += instant;
        }

        result[i] = instantSum;
    }

    return result;

}
*/

//std::array <double, static_cast<size_t>(Location::SIZE)> parameters(size_t);
