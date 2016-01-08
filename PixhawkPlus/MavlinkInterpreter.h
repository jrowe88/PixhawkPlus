// MavlinkInterpreter.h

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

class MavLinkInterpreter
{
public:
	MavLinkInterpreter(MAVLinkData* messages);
	~MavLinkInterpreter();
	void Setup();
	void Receive();
	void CheckConnection();

private:
	MAVLinkData *_m;

	unsigned long RC_DEBUG_TIMEOUT = 3000;
	bool _connected;
	bool _lastConnected = 0;
	unsigned long MavLink_Connected_timer = 0;
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

	void ProcessStatusText();
	void ProcessHUD() const;
	void ProcessRCChannels() const;
	void ProcessGpsTime() const;
	void ProcessAttitude() const;
	void ProcessImu() const;
	void ProcessGps() const;
	void ProcessHeartbeat();
	void ProcessSystemStatus() const;
	void SendHeartbeat();
	void ConfigureConnection();
};

#endif

