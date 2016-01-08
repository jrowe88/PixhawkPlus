// BufferedSmoothValue.h

#ifndef _BUFFEREDSMOOTHVALUE_h
#define _BUFFEREDSMOOTHVALUE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define CIRC_BUFFER_SIZE 128 //must be power of two; keep it larger than 2X the filter coefficient size


template <class T> class BufferedSmoothValue
{	
public:
	BufferedSmoothValue();
	BufferedSmoothValue(int bufferSize, double* coefficients, int coeffSize);
	~BufferedSmoothValue();

	void SetCoefficients(double* coefficients, int size);
	T GetValue();
	T GetSmoothedValue();
	void SetValue(T value);
	T FIRAvg();
	

private:

	T* _data;
	int _bufferSize;
	double* _coefficients;
	int _coeffSize;
	int _currentIndex;
};

#endif

