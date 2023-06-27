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

4. The calculation result (the complex amplitude value) can be obtained by executing the getData(size_t) command:

   dft.getData(neededSize_tHarmonic);

   If the desired harmonic was not specified when creating an instance of the class (second parameter which sets harmonics dataset), the getData(size_t) command will return a complex zero.

5. The results vector of calculating the harmonics complex values can be obtained using the getData() command:

   dft.getData(neededSize_tHarmonic).


   Example of using the Dft class

=========================================================
   
   //Initialization and filling of the basic sine wave by 8 points per period
   
   BaseSineWave baseSineWave(8);       		

   //Instantiation DFT for calculatind 1, 2, 3 harmonical values
   
   Dft  dft(&baseSineWave, {1, 2, 3});	

   //Adding new values
   
   dft.update(2.0);
   dft.update(1.4142);
   dft.update(0);
   dft.update(-1.4142);
   dft.update(-2.0);
   dft.update(-1.4142);
   dft.update(0);
   dft.update(1.4142);
   dft.update(2.0);

   //Getting results
   
   std::cout << dft.getData(1) << "\t" << dft.getData(2) << "\t" << dft.getData(3) << "\t";

=========================================================

   Information is displayed on the monitor:
   
   (1.99999,-3.38786e-016) (0,0)   (9.59005e-006,2.71837e-016)
   
=========================================================



The interface of the Fft and Rdft classes does not differ from the described Dft interface.

     
   To FFT calculation, the following files *.h and *.cpp must be included to the project:
   - BaseSineWave;
   - FourierTransform;
   - RingBuffer;
   - Fft.


   To RDFT calculation, the following files *.h and *.cpp must be included to the project:
   - BaseSineWave;
   - FourierTransform;
   - RingBuffer;
   - Rdft.


main.cpp used for checking results and comparing classes.
