#pragma region Copyright
//This file is part of Pixhawk Plus
//
//Jim Rowe Copyright (c) 2015
//https://github.com/jrowe88/PixhawkPlus
//
//Pixhawk Plus is free software : you can redistribute it and / or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//Foobar is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with Pixhawk Plus. If not, see <http://www.gnu.org/licenses/>.
//
#pragma endregion

#include "Coefficients.h"
#include "BufferedSmoothValue.h"

template <class T>
BufferedSmoothValue<T>::BufferedSmoothValue()
{
	_coefficients = Coefficients::Hamming;
	_coeffSize = 21;
	_currentIndex = 0;
}

template <class T>
BufferedSmoothValue<T>::BufferedSmoothValue(double* coefficients, size_t coeffSize)
{
	_coefficients = coefficients;
	_coeffSize = coeffSize;
	_currentIndex = 0;
}

template <class T>
BufferedSmoothValue<T>::~BufferedSmoothValue()
{
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
	int idx = _currentIndex;
	_data[idx] = value;

	// Update the current index of our ring buffer.
	idx++;
	_currentIndex = (idx % _bufferSize);	
}

/*
Use a FIR filter to smooth the data.  Works on the circular buffer starting at position n-1/ncoeff
*/
template <class T>
T BufferedSmoothValue<T>::FIRAvg()
{
	double tmp = 0;
	int i = 0;
	int32_t start = (_currentIndex - _coeffSize + 1 + _bufferSize) % _bufferSize;

	for (i = 0; i < _coeffSize; i++)  //position in coefficients array
	{
		int idx = (start + i) % _bufferSize;  //determine location in ring buffer
		tmp += _coefficients[i] * _data[idx];
		//debugSerial.printf("%d=%8.6f * %5.3f\r\n", idx, _coefficients[i], _data[idx]);
	}

	return static_cast<T>(tmp);
}


template class BufferedSmoothValue<float>;
template class BufferedSmoothValue<int16_t>;
