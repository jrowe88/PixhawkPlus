// 
// 
// 

#include "BufferedSmoothValue.h"

template <class T>
BufferedSmoothValue<T>::BufferedSmoothValue()
{
}

template <class T>
BufferedSmoothValue<T>::BufferedSmoothValue(int bufferSize, double* coefficients, int coeffSize)
{
	_bufferSize = bufferSize;
	_data = static_cast<T*>(calloc(bufferSize, sizeof(T)));
	_coefficients = coefficients;
	_coeffSize = coeffSize;
	_currentIndex = 0;
}

template <class T>
BufferedSmoothValue<T>::~BufferedSmoothValue()
{
	if (_data != nullptr) free(_data);
}

template <class T>
void BufferedSmoothValue<T>::SetCoefficients(double* coefficients, int size)
{
	_coeffSize = size;
	_coefficients = coefficients;
}

template <class T>
T BufferedSmoothValue<T>::GetValue()
{
	return _data[_currentIndex];
}

template <class T>
T BufferedSmoothValue<T>::GetSmoothedValue()
{
	return FIRAvg();
}

template <class T> 
void BufferedSmoothValue<T>::SetValue(T value)
{
	int buffLen = _bufferSize - 1;
	int lastWrittenSample = _currentIndex;

	int idx = lastWrittenSample & buffLen;
	_data[idx] = value;

	// Update the current index of our ring buffer.
	_currentIndex++;
	_currentIndex &= buffLen;
}

/*
Use a FIR filter to smooth the data.  Works on the circular buffer starting at position n-1/ncoeff
*/
template <class T>
T BufferedSmoothValue<T>::FIRAvg()
{
	int i;
	double tmp = 0;
	int buffLen = _bufferSize - 1;
	int lastWrittenSample = _currentIndex;

	int startOffset = _coeffSize / 2;
	for (i = 0; i < _coeffSize; i++)  //position in coefficients array
	{
		int idx = (i + lastWrittenSample - startOffset) & buffLen;  //determine location in ring buffer
		tmp += _coefficients[i] * _data[idx];
	}

	return static_cast<T>(tmp);
}


template class BufferedSmoothValue<float>;
template class BufferedSmoothValue<int16_t>;
