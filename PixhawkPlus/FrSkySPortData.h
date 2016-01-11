#pragma region Copyright
//This file is part of Pixhawk Plus
//
//Jim Rowe Copyright (c) 2015
//https://github.com/jrowe88/PixhawkPlus
//
//  Original concept and big props to Rolf Blomgren - http://diydrones.com/forum/topics/amp-to-frsky-x8r-sport-converter
//  Adapted and modified from numerous other contributors:
//     Jochen Kielkopf - https://github.com/Clooney82/MavLink_FrSkySPort/   
//     Alf Pettersson
//     Christian Swahn
//     Luis Vale
//     Michael Wolkstein
//     Jochen Kielkopf
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

#ifndef _FRSKYSPORTDATA_h
#define _FRSKYSPORTDATA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include <FrSkySportTelemetry.h>
#include <FrSkySportSensorVario.h>
#include <FrSkySportSensorSp2uart.h>
#include <FrSkySportSensorRpm.h>
#include <FrSkySportSensorGps.h>
#include <FrSkySportSensorFuel.h>
#include <FrSkySportSensorFlvss.h>
#include <FrSkySportSensorFas.h>
#include <FrSkySportSensorAcc.h>

#include "MAVLinkData.h"

/// <summary>
/// Write FrSkySPort compatible data packets
/// </summary>
class FrSkySPortData
{
public:
	FrSkySPortData(MAVLinkData *messages);
	~FrSkySPortData();
	void Init();
	void Process();

	uint8_t Initialized = 0;

private:
	MAVLinkData *_m;
	
	/*
	* *******************************************************
	* *** Define FrSkySPortTelemetry Sensors:             ***
	* *******************************************************
	*/
	FrSkySportSensorFas _fas;                               // Create FAS sensor with default ID
	FrSkySportSensorFuel _fuel;                             // Create FUEL sensor with default ID
	FrSkySportSensorFlvss _flvss1;                          // Create FLVSS sensor with default ID
	FrSkySportSensorGps _gps;                               // Create GPS sensor with default ID
	FrSkySportSensorRpm _rpm;                               // Create RPM sensor with default ID
	FrSkySportSensorAcc _acc;                               // Create ACC sensor with default ID
	FrSkySportSensorSp2uart _sp2uart;                       // Create SP2UART Type B sensor with default ID
	FrSkySportSensorVario _vario;                           // Create Variometer sensor with default ID
	FrSkySportTelemetry _telemetry;                         // Create Variometer _telemetry object


	/*
	* *******************************************************
	* *** Define some Variables:                          ***
	* *******************************************************
	*/
	static const int8_t statusRingsize = 30; // Status message rigbuffer vars
	int32_t statusRingHead = 0;
	int32_t statusRingTail = 0;
	int32_t statusRing[statusRingsize];
	uint32_t my_dequeue_status_value = 0;

	float FASCurrent = 0.0;
	float FASVoltage = 0.0;

	unsigned long FAS_timer = 0;
	int8_t transmit = 0;

	// Scale factor for roll/pitch:
	// We need to scale down 360 deg to fit when max value is 256, and 256 equals 362 deg
	float scalefactor = 360.0 / ((362.0 / 360.0)*256.0);

	unsigned long GPS_debug_time = 500;

	/*
	* *******************************************************
	* *** Methods                                         ***
	* *******************************************************
	*/
	void SetPitchAndRoll();
	void SetAcceleration();
	void SetVoltage();
	void SetCellVoltage();
	void SetFuel();
	void SetGPS();
	void SetRPM();
	void SetVariometer();
	uint32_t Handle_A2_A3_value(uint32_t value);
	int32_t StatusDequeue();
	int StatusEqueue(int32_t val);
	int StatusQueuelevel();	
};

inline FrSkySPortData::FrSkySPortData(MAVLinkData *messages)
{
	_m = messages;	
}

inline FrSkySPortData::~FrSkySPortData()
{
}



#endif

