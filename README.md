# FFT
The project implements the following classes:
1. Discrete Fourier Transform (DFT).
2. Fast Fourier Transform (FFT),
3. Recurrent Discrete Fourier Transform (RDFT)

   All classes are designed to implement the same functionality: complex amplitudes calculation of required harmonics set. The classes efficiency is determined by the processing time of the same text file containing the harmonic signal instantaneous values.

   To DFT calculation, the following files *.h and *.cpp must be included to the project:
   - BaseSineWave;
   - FourierTransform;
   - RingBuffer;
   - Dft.

   Working with the class is carried out in the following order:
1. A basic sinusoidal wave is formed with the required number of points per period of industrial frequency setting:
   
   BaseSineWave baseSineWave(96);

2. The Dft class instance is created, where the first parameter is reference of the baseSineWave, second (optional) parameter is std::vector<size_t> set of calculated harmonics, third and fourth (optional) parameters is correctional angle (degree) and amplitude (p.u.):

   Dft  dft(&baseSineWave, {1});

   If second parameter (set of calculating harmonics) is absent, then the following harmonics will be calculated: 1, 3, 5.

3. New value of the double type adding is done by the update(double) command:

   dft.update(newDoubleValue);

   After an each new value addition, the set of complex amplitudes is calculated with data of the last period of the industrial frequency.
     
