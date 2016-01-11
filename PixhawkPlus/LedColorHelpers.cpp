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

#include "LedColorHelpers.h"

#pragma region Color Functions

const uint8_t LedColorHelpers::_gammaR[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 17, 17, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21, 21, 22, 22, 22, 23, 23, 24, 24, 24, 25, 25, 26, 26, 27, 27, 27, 28, 28, 29, 29, 29, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36, 36, 36, 37, 37, 38, 39, 39, 40, 40, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 46, 47, 47, 48, 48, 49, 49, 50, 51, 51, 52, 52, 53, 53, 54, 55, 55, 56, 56, 57, 58, 58, 59, 60, 60, 61, 61, 62, 63, 63, 64, 65, 65, 66, 67, 67, 68, 69, 69, 70, 71, 72, 72, 73, 74, 74, 75, 76, 76, 77, 78, 79, 79, 80, 81, 81, 82, 83, 84, 84, 85, 86, 87, 87, 88, 89, 90, 90, 91, 92, 93, 93, 94, 95, 96, 97, 97 };
const uint8_t LedColorHelpers::_gammaG[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 18, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 22, 22, 22, 23, 23, 23, 24, 24, 24, 25, 25, 25, 26, 26, 26, 27, 27, 27, 28, 28, 28, 29, 29, 30, 30, 30, 31, 31, 31, 32, 32, 33, 33, 33, 34, 34, 35, 35, 35, 36, 36, 37, 37, 38, 38, 38, 39, 39, 40, 40, 41, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 48, 49, 49, 50, 50, 51, 51, 52, 52, 53, 53, 54, 54, 55, 55, 56, 56, 57, 58, 58, 59, 59, 60, 60, 61, 61, 62, 63, 63, 64, 64, 65 };
const uint8_t LedColorHelpers::_gammaB[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 19, 19, 20, 20, 20, 21, 21, 21, 22, 22, 22, 23, 23, 23, 24, 24, 25, 25, 25, 26, 26, 27, 27, 27, 28, 28, 29, 29, 30, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 34, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 40, 40, 41, 41, 42, 42, 43, 43, 44, 45, 45, 46, 46, 47, 47, 48, 49, 49, 50, 50, 51, 51, 52, 53, 53, 54, 54, 55, 56, 56, 57, 58, 58, 59, 59, 60, 61, 61, 62, 63, 63, 64, 65, 65, 66, 67, 67, 68, 69, 69, 70, 71, 72, 72, 73, 74, 75, 75, 76, 77, 78, 78, 79, 80, 81, 81, 82, 83, 84, 84, 85, 86, 87, 88, 88, 89, 90, 91, 92, 93 };

inline void LedColorHelpers::Gamma(CRGB& rgb)
{
	rgb.r = _gammaR[rgb.r];
	rgb.g = _gammaG[rgb.g];
	rgb.b = _gammaB[rgb.b];
}

///<summary>
///  Dims a color by a 0-1 factor (smaller factor means dimmer)
///</summary>
inline int LedColorHelpers::Dim(int color, double factor)
{	
	//int fade = 255 * factor;
	//uint8_t r = (color >> 16) + fade);
	//uint8_t g = (color >> 8 & 0xFF) + fade);
	//uint8_t b = color - +fade);
	//return r << */16 | g << 8 | b;
	return color;
}

///<summary>
///  Creates a sine pulse.  Specify a divisor to get mulitiple sin wave per period
///</summary>
void LedColorHelpers::CreateSinePulse(uint8_t values, uint8_t* wave, int divisor)
{
	for (size_t i = 0; i < values; i++)
	{
		wave[i] = (uint8_t)(255 * (float)(0.5 * (cos(PI * i / divisor) + 1)));
	}
}

///<summary>
///  Creates a linear intensity ramp / sawtooth pattern
///</summary>
void LedColorHelpers::CreateLinearRamp(uint8_t values, uint8_t* wave, int period)
{
	for (size_t i = 0; i < values; i++)
	{
		wave[i] = (uint8_t)(255 * (i % period) / (float)(period - 1));
	}
}

///<summary>
///  Create an HSV color scale
///</summary>
void LedColorHelpers::CreateRainbow(uint8_t values, int* matrix)
{
	CRGB color;
	for (size_t i = 0; i < values; i++)
	{
		uint8_t h = (uint8_t)(255 * i / (float)values);	//much easier to use HSV color model for this
		color.setHSV(h, 255, 255);
		Gamma(color);
		matrix[i] = GetColorCode(color);
	}
}

///<summary>
///  Creates either a red-to-blue, or blue-to-red JPL (a.k.a., "jet") color scale.  This is a default color ramp in MatLab
///</summary>
void LedColorHelpers::CreateJPLRedBlue(uint8_t values, int* matrix, uint8_t redBlue)
{
	int i = 0;
	int j = redBlue == 1 ? 0 : values - 1;
	int increment = (redBlue == 1 ? 1 : -1);

	while (i < values)
	{
		double x = (double)j / (values - 1);

		//R value
		double r;
		if ((x >= 3.0 / 8.0) && (x < 5.0 / 8.0))
		{
			r = (4.0 * x - 3.0 / 2.0);
		}
		else if ((x >= 5.0 / 8.0) && (x < 7.0 / 8.0))
		{
			r = 1;
		}
		else if (x >= 7.0 / 8.0)
		{
			r = -4.0 * x + 9.0 / 2.0;
		}
		else
		{
			r = 0;
		}

		//G value
		double g;
		if ((x >= 1.0 / 8.0) && (x < 3.0 / 8.0))
		{
			g = (4.0 * x - 1.0 / 2.0);
		}
		else if ((x >= 3.0 / 8.0) && (x < 5.0 / 8.0))
		{
			g = 1.0;
		}
		else if ((x >= 5.0 / 8.0) && (x < 7.0 / 8.0))
		{
			g = -4.0 * x + 7.0 / 2.0;
		}
		else
		{
			g = 0.0;
		}

		//B value
		double b;
		if ((x < 1.0 / 8.0))
		{
			b = (4.0 * x + 1.0 / 2.0);
		}
		else if ((x >= 1.0 / 8.0) && (x < 3.0 / 8.0))
		{
			b = 1.0;
		}
		else if ((x >= 3.0 / 8.0) && (x < 5.0 / 8.0))
		{
			b = -4.0 * x + 5.0 / 2.0;
		}
		else
		{
			b = 0.0;
		}

		//Set the 24-bit color
		matrix[j] = (int)(r * 255.0) << 16 | (int)(g * 255.0) << 8 | (int)(b * 255.0);

		j = j + increment;
		i++;
	}
}


#pragma endregion
