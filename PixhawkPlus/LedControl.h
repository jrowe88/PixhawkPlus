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

#ifndef _LEDCONTROL_h
#define _LEDCONTROL_h

#include <stdint.h>
#include "Configuration.h"
#include "MAVLinkData.h"
#include "LedColorHelpers.h"
#include <OctoWS2811.h>


/// <summary>
/// Control code for the LedLights.
/// </summary>
class LedControl
{
public:
	LedControl(MAVLinkData *messages);
	~LedControl();
	void Initialize();
	void Process();
	void NoMavLink();
	static const uint32_t LEDS_PER_ARM = LEDSPERARM;
	
	enum FLIGHTMODE : int
	{
		Disarmed = 0x1,
		Armed = 0x2,
		Standard = 0x4,
		Stabilize = 0x8,
		Altitude = 0x10,
		Loiter = 0x20,
		Return = 0x40
	};

	enum GPSFIX : uint8_t
	{
		NoGPS = 0,
		NoFix = 1,
		Fix2D = 2,
		Fix3D = 3
	};

private:
	
	MAVLinkData *_messages;
	OctoWS2811 _ledDriver;
	static const LedColorHelpers _ch;

	static const uint8_t CONFIG = WS2811_GRB | WS2811_800kHz;
	static const int NUM_ARMS = 1;	
	uint32_t _frame = 0, _lastFrame = 0;
	static const uint8_t FRAMES_PER_LOOP = 64;     //frame loop length
	static const float MS_PER_FRAME;
	static const uint32_t _numLeds = LEDS_PER_ARM * NUM_ARMS;
	int _display[FRAMES_PER_LOOP][_numLeds];       //array of 24-bit colors, per-frame; per-pixe
	
	//Some basic intensity waves
	uint8_t _sinPulse[FRAMES_PER_LOOP];
	uint8_t _sinDoublePulse[FRAMES_PER_LOOP];
	uint8_t _sinQuadPulse[FRAMES_PER_LOOP];
	uint8_t _rampSingle[FRAMES_PER_LOOP];
	uint8_t _rampDouble[FRAMES_PER_LOOP];
	uint8_t _rampQuad[FRAMES_PER_LOOP];
	int _rainbow[FRAMES_PER_LOOP];
	int _jetRB[FRAMES_PER_LOOP];
	int _jetBR[FRAMES_PER_LOOP];

	//Methods
	void SetPixelsSolid(uint32_t pixel, int color);
	void SetPixelsBlinkPattern(uint32_t pixel, int color, uint64_t pattern);
	void SetPixelsColorPattern(uint32_t pixel, int *colorPattern, int shift);
	void SetPixelsUserFcn(uint32_t pixel, int color, uint8_t *intensityWave);
	void SetPixelBlink(uint64_t pattern, uint32_t arm, uint32_t pixel, int color);
	void SetGPSLights(uint8_t gpsStatus);
	void GpsSeeking(uint32_t found);
	void ShowFrame();
	void AllOff();
};


#endif
