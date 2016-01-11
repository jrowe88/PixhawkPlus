#pragma region Copyright
//This file is part of Pixhawk Plus
//
//Jim Rowe Copyright (c) 2015
//https://github.com/jrowe88/PixhawkPlus
//
//  Original concept and big props to Rolf Blomgren - http://diydrones.com/forum/topics/amp-to-frsky-x8r-sport-converter
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

#ifndef _MAVLINKCONTROL_h
#define _MAVLINKCONTROL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Configuration.h"
#include "Coefficients.h"
#include "BufferedSmoothValue.h"
#include "LipoSingleCellMonitor.h"
#include "LedControl.h"
#include "FrSkySPortData.h"
#include "MAVLinkData.h"
#include "MavlinkInterpreter.h"	


/// <summary>
/// Master "engine" for sampling the Mavlink, voltage, and led processing.  Performs setup
/// and makes calls to the supporting classes.
/// </summary>
class MavLinkController
{
public:
	MavLinkController();
	~MavLinkController();
	void Initialize();
	void Process();

private:
	MAVLinkData *_messages;	
	MavLinkInterpreter *_mavLink;
	FrSkySPortData *_frSky;
	LedControl *_leds;
	LipoSingleCellMonitor *_lscm;
};

inline MavLinkController::MavLinkController()
{
	_messages = new MAVLinkData();	
	_lscm = new LipoSingleCellMonitor(MAXCELLS, ANALOG_SAMPLE_BITS, _messages);
	_leds = new LedControl(_messages);
	_frSky = new FrSkySPortData(_messages);
	_mavLink = new MavLinkInterpreter(_messages);
}

inline MavLinkController::~MavLinkController()
{
	if (_messages != nullptr) delete _messages;	
	if (_lscm != nullptr) delete _lscm;
	if (_leds != nullptr) delete _leds;
	if (_frSky != nullptr) delete _frSky;
	if (_mavLink != nullptr) delete _mavLink;		
}


#endif

