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



#ifndef _MAVLINKINTERPRETER_h
#define _MAVLINKINTERPRETER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include <GCS_MAVLink.h>
#include "Configuration.h"
#include "MAVLinkData.h"


/// <summary>
/// Primary processor of MavLink messages of interest.  Read and does minimal conversion/scaling to
/// put the data in teh MavLinkData structure
/// </summary>
class MavLinkInterpreter
{
public:
	MavLinkInterpreter(MAVLinkData* messages);
	~MavLinkInterpreter();
	void Setup();
	void Receive();
	bool CheckConnection();

private:
	MAVLinkData *_m;

	unsigned long RC_DEBUG_TIMEOUT = 3000;
	bool _connected = false;
	bool _lastConnected = false;
	unsigned long _mavLinkConnectedTimer = 0;
	uint8_t _hb_count = 0;
	uint8_t _hb_count_lost = 0;
	unsigned long _hb_timer = 0;
	
	mavlink_message_t _msg;
	mavlink_system_t _mavlink_system = {MY_SYSID , MY_CMPID};
	mavlink_status_t _status;
	mavlink_statustext_t _statusText;	
	uint16_t _len = 0;
	uint8_t _buf[MAVLINK_MAX_PACKET_LEN];

	unsigned long _send_mavlink_connection_config = 0;

	void ProcessBattery();
	void ProcessStatusText();
	void ProcessHUD() const;
	void ProcessRCChannels() const;
	void ProcessGpsTime() const;
	void ProcessGpsOrigin() const;
	void ProcessAttitude() const;
	void ProcessImu() const;
	void ProcessGpsIntegrated() const;
	void ProcessGpsRaw() const;
	void ProcessHeartbeat();
	void ProcessSystemStatus() const;
	void SendHeartbeat();
	void ConfigureConnection();
};

#endif

