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
#ifndef _BUFFEREDSMOOTHVALUE_h
#define _BUFFEREDSMOOTHVALUE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include "Configuration.h"

/// <summary>
/// Uses a circular buffer to store reading values. Get method allows you to retrieve
/// a value using the specified smoothing coefficients.
/// </summary>
template <class T> class BufferedSmoothValue
{	
public:
	BufferedSmoothValue();
	BufferedSmoothValue(double* coefficients, size_t coeffSize);
	~BufferedSmoothValue();

	void SetCoefficients(double* coefficients, int size);
	T GetValue();
	T GetSmoothedValue();
	void SetValue(T value);
	T FIRAvg();
	

private:
	static const uint32_t _bufferSize = 128;
	T _data[_bufferSize];	
	double* _coefficients;
	uint32_t _coeffSize;
	uint32_t _currentIndex;
};

#endif

