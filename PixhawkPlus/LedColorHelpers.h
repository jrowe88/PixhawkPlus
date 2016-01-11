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

#ifndef _LEDCOLORFUNCTIONS_h
#define _LEDCOLORFUNCTIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <FastLED.h>
#include <wiring.h>

/// <summary>
/// Functions to create patterns and perform basic color manipulations of RGB colors.
/// Functions can be used to create data that fits into "n" number of frames, so
/// effects like pulsing, color changing, etc. can be created in a small stream
/// of frames.
/// </summary>
class LedColorHelpers
{
public:
	//Blink patterns.  48-bits equal approximately 2 seconds.
	//Set the LSB bits with your pattern.  Bits 49-64 will be ignored.
	static const uint64_t BLINK_VERYSLOW_A = 0x00000000FFFFFFFF;        //every 1 second
	static const uint64_t BLINK_VERYSLOW_B = 0xFFFFFFFF00000000; //every 1 second
	static const uint64_t BLINK_SLOW_A = 0x0000FFFF0000FFFF;  //every 1/2 second
	static const uint64_t BLINK_SLOW_B = 0xFFFF0000FFFF0000;    //every 1/2 second
	static const uint64_t BLINK_MEDIUM_A = 0xFF00FF00FF00FF00;   //every 1/3 second
	static const uint64_t BLINK_MEDIUM_B = 0x00FF00FF00FF00FF; //every 1/3 second
	static const uint64_t BLINK_MEDIUMFAST_A = 0xF0F0F0F0F0F0F0F0;  //every 1/4 second
	static const uint64_t BLINK_MEDIUMFAST_B = 0x0F0F0F0F0F0F0F0F;  //every 1/4 second
	static const uint64_t BLINK_FAST_A = 0xCCCCCCCCCCCCCCCC;  //every 1/6 second
	static const uint64_t BLINK_FAST_B = 0x3333333333333333;  //every 1/6 second
	static const uint64_t ALL_FRAMES = 0xFFFFFFFFFFFFFFFF;

	static void CreateSinePulse(uint8_t values, uint8_t* wave, int divisor);
	static void CreateLinearRamp(uint8_t values, uint8_t* wave, int period);
	static void CreateRainbow(uint8_t values, int* matrix);
	static void CreateJPLRedBlue(uint8_t values, int* matrix, uint8_t redBlue);
	static void Gamma(CRGB& rgb);
	static int Dim(int color, double factor);

	///<summary>
	///  Get the 24-bit color code from a CRGB structure
	///</summary>
	static inline int GetColorCode(CRGB color)
	{
		return color.r << 16 | color.g << 8 | color.b;
	}	

private:
	
	static const uint8_t _gammaR[256];
	static const uint8_t _gammaG[256];
	static const uint8_t _gammaB[256];
	
};

#endif

