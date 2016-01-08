// MavlinkControl.h

#ifndef _MAVLINKCONTROL_h
#define _MAVLINKCONTROL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Configuration.h"

#include "LipoSingleCellMonitor.h"
#include "LedControl.h"
#include "FrSkySPortData.h"
#include "MAVLinkData.h"
#include "MavlinkInterpreter.h"	

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
	uint8_t _mavLinkConnected = 0;	
};

inline MavLinkController::MavLinkController()
{
	_messages = new MAVLinkData();	
	_lscm = new LipoSingleCellMonitor(MAXCELLS, 13,_messages);
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

