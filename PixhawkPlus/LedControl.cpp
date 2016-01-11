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

#include "LedControl.h"

//Some standard color definitions
static const int C_DISARMED = CRGB::Red;
static const int C_ARMED = CRGB::DarkCyan;
static const int C_NOGPS = CRGB::Orange;
static const int C_GPSSEEK = CRGB::Green;
static const int C_FIX2D = CRGB::LawnGreen;
static const int C_FIX3D = CRGB::Green;
static const int C_BLACK = CRGB::Black;

//High speed WS2812B driver configuration
DMAMEM int _ledDisplayMemory[LEDSPERARM * 6];
DMAMEM int _ledDrawingMemory[LEDSPERARM * 6];

/*
	Global variables used:
		ap_voltage_battery
		ap_cell_count
		ap_fixtype  // 0 = No GPS, 1 =No Fix, 2 = 2D Fix, 3 = 3D Fix, 4 = DGPS, 5 = RTK
		ap_sat_visible
*/

const float LedControl::MS_PER_FRAME = 31.25;  // = ~32fps, so our 64-bit periods will be two seconds

LedControl::LedControl(MAVLinkData *messages) :
	_ledDriver(LEDS_PER_ARM, _ledDisplayMemory, _ledDrawingMemory, CONFIG)
{
	_messages = messages;
			
	_ch.CreateSinePulse(FRAMES_PER_LOOP, _sinPulse, 32);
	_ch.CreateSinePulse(FRAMES_PER_LOOP, _sinDoublePulse, 16);
	_ch.CreateSinePulse(FRAMES_PER_LOOP, _sinQuadPulse, 8);
	_ch.CreateLinearRamp(FRAMES_PER_LOOP, _rampSingle, 64);
	_ch.CreateLinearRamp(FRAMES_PER_LOOP, _rampDouble, 32);
	_ch.CreateLinearRamp(FRAMES_PER_LOOP, _rampQuad, 16);
	_ch.CreateRainbow(FRAMES_PER_LOOP, _rainbow);
	_ch.CreateJPLRedBlue(FRAMES_PER_LOOP, _jetRB, 1);
	_ch.CreateJPLRedBlue(FRAMES_PER_LOOP, _jetBR, 0);
}

LedControl::~LedControl()
{	
}

//Called once to initialize
void LedControl::Initialize()
{
	_ledDriver.begin();	
	AllOff();
	_ledDriver.show();	
}

//Called once in the main Processing Loop.
// This is a constant frame rate loop that will excute once per frame, which is approximately
//  at 30 frames per second.  Assumption is that calculations are trivial enough that there are
//  plenty of tics to complete the work within a frame.  If not, it will just run at a slower
//  frame rate and blink patterns and functions may be erratic.
void LedControl::Process()
{
	//Only process this once per frame
	_frame = millis() / MS_PER_FRAME;
	
	if (_frame != _lastFrame)
	{						
		//Armed/Disarmed Status
		///*SetPixelsSolid(1, C_ARMED);
		//SetPixelsSolid(3, C_ARMED);
		//SetPixelsSolid(5, C_ARMED);
		//*/SetPixelsSolid(7, C_ARMED);		

		//GPS Fix		
		SetGPSLights(Fix2D);

		//Nav Lights


		//Breaking


		//Mode


		//Swith Response


		//Set all the pixels and show them
		ShowFrame();	
		_lastFrame = _frame;
	}
}

void LedControl::NoMavLink()
{
}

void LedControl::AllOff()
{
	for (int i = 0; i < LEDS_PER_ARM * NUM_ARMS; i++)
	{
		_ledDriver.setPixel(i, 0);
	}
}

void LedControl::SetPixelsSolid(uint32_t pixel, int color)
{	
	SetPixelsBlinkPattern(pixel, color, _ch.ALL_FRAMES);
}

//Blinks a pixel based on the binary pattern
void LedControl::SetPixelsBlinkPattern(uint32_t pixel, int color, uint64_t pattern)
{	
	for (size_t f = 0; f < FRAMES_PER_LOOP; f++)
	{
		int colordisp = color * (pattern >> f & 0x1);
		for (size_t i = 0; i < NUM_ARMS; i++)
		{
			_display[f][i * LEDS_PER_ARM + pixel] = colordisp;
		}
	}
}

//Sets color pattern.  Expects a int[FRAMES_PER_LOOP] sized array
void LedControl::SetPixelsColorPattern(uint32_t pixel, int *colorPattern, int shift = 0)
{
	for (size_t f = 0; f < FRAMES_PER_LOOP; f++)
	{		
		for (size_t i = 0; i < NUM_ARMS; i++)
		{
			int c = (f + shift) % FRAMES_PER_LOOP;
			_display[f][i * LEDS_PER_ARM + pixel] = colorPattern[c];
		}
	}
}

//Blinks a pixel baesd on a user supplied wave with 0-to-1 scale factor.
void LedControl::SetPixelsUserFcn(uint32_t pixel, int color, uint8_t *intensityWave)
{
	for (size_t f = 0; f < FRAMES_PER_LOOP; f++)
	{		
		int c = _ch.Dim(color, intensityWave[f]);
		for (size_t i = 0; i < NUM_ARMS; i++)
		{			
			_display[f][i * LEDS_PER_ARM + pixel] = c;
		}
	}
}

void LedControl::SetPixelBlink(uint64_t pattern, uint32_t arm, uint32_t pixel, int color)
{
	for (size_t f = 0; f < FRAMES_PER_LOOP; f++)
	{
		int colordisp = color * (pattern >> f & 0x1);
		_display[f][arm * LEDS_PER_ARM + pixel] = colordisp;
	}
}

void LedControl::SetGPSLights(uint8_t gpsStatus)
{
	switch(gpsStatus)
	{
	case NoGPS: 
		SetPixelsBlinkPattern(0, C_NOGPS, _ch.BLINK_MEDIUM_A);
		SetPixelsBlinkPattern(2, C_NOGPS, _ch.BLINK_MEDIUM_A);
		SetPixelsBlinkPattern(4, C_NOGPS, _ch.BLINK_MEDIUM_A);
		SetPixelsBlinkPattern(6, C_NOGPS, _ch.BLINK_MEDIUM_A);
		break;
	case NoFix:
		//GpsSeeking(0);
		for (size_t i = 0; i < 8; i++)
		{
			SetPixelsSolid(i, 0x808080);
		}		
		break;
	case Fix2D:
		for (size_t i = 0; i < LEDSPERARM; i++)
		{
			SetPixelsColorPattern(i, _rainbow);
			//SetPixelsColorPattern(i, _rainbow, 255 * i/LEDSPERARM );
		}		
		break;
	case Fix3D:
		SetPixelsBlinkPattern(6, C_FIX3D, _ch.BLINK_FAST_A);
		break;
	default: break;
	}

}

void LedControl::GpsSeeking(uint32_t numFound)
{
	CRGB c = C_GPSSEEK;
	int armOffset = 0;
	float dim = .45;

	for (size_t i = 0; i < LEDS_PER_ARM - 1; i++)
	{		
		for (size_t j = 0; j < LEDS_PER_ARM - 1; j++)
		{
			if (j <= i)
			{
				_ledDriver.setPixel(armOffset + j, c.r * dim, c.g * dim, c.b * dim);
			}
		}
	}
}

//Sets the pixels for the current frame and render the pixels.
void LedControl::ShowFrame()
{
	int frameSequence = _frame % FRAMES_PER_LOOP;
	for (size_t i = 0; i < _numLeds; i++)
	{
		_ledDriver.setPixel(i, _display[frameSequence][i]);
	}
	_ledDriver.show();	
}



