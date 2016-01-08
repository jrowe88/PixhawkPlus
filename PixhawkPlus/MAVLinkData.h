// MAVLinkData.h

#ifndef _MAVLINKMESSAGES_h
#define _MAVLINKMESSAGES_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <GCS_MAVLink.h>
#include "BufferedSmoothValue.h"
#include "Coefficients.h"

class MAVLinkData
{
public:
	MAVLinkData();
	~MAVLinkData();

	/*
	* *******************************************************
	* *** Message #0  HEARTBEAT                           ***
	* *******************************************************
	*/
	//uint8_t     _type               =  0;
	//uint8_t     _autopilot          =  0;
	//uint8_t     _system_status      =  0;
	uint8_t BaseMode = 0;
	int32_t CustomMode = -1;
	uint8_t MavlinkVersion = 0;

	/*
	* *******************************************************
	* *** Message #1  SYS_STATUS                          ***
	* *******************************************************
	*/
	uint16_t VoltageBattery = 0; // 1000 = 1V | Battery voltage, in millivolts (1 = 1 millivolt)
	int16_t CurrentBattery = 0;  //   10 = 1A | Battery current, in 10*milliamperes (1 = 10 milliampere), -1: autopilot does not measure the current.
	int8_t BatteryRemaining = 0; // Remaining battery energy: (0%: 0, 100%: 100), -1: autopilot estimate the remaining battery	

	/*
	* *******************************************************
	* *** Message #24  GPS_RAW_INT                        ***
	* *******************************************************
	*/
	uint8_t FixType = 0; // 0 = No GPS, 1 = No Fix, 2 = 2D Fix, 3 = 3D Fix, 4 = DGPS, 5 = RTK.
	// Some applications will not use the value of this field unless it is at least two, so always correctly fill in the fix.
	uint8_t SatVisible = 0; // Number of visible Satelites
	// FrSky Taranis uses the first recieved lat/long as homeposition.
	int32_t Latitude = 0; // Latitude (WGS84), in degrees * 1E7
	int32_t Longitude = 0; // Longitude (WGS84), in degrees * 1E7
	int32_t GpsAltitude = 0; // Altitude (AMSL, NOT WGS84), in meters * 1000 (positive for up).
	// Note that virtually all GPS modules provide the AMSL altitude in addition to the WGS84 altitude.
	int32_t GpsSpeed = 0; // GPS ground speed (m/s * 100)
	uint16_t GpsHdop = 255; // GPS HDOP horizontal dilution of position in cm (m*100). If unknown, set to: 65535
	uint16_t GpsVdop = 0; // GPS VDOP horizontal dilution of position in cm (m*100). If unknown, set to: 65535
	uint32_t Cog = 0; // Course over ground (NOT heading, but direction of movement) in degrees * 100, 0.0..359.99 degrees. If unknown, set to: 65535

	/*
	* *******************************************************
	* *** Message #  Accelerometer                        ***
	* *******************************************************
	*/
	BufferedSmoothValue<int16_t> AccelerationX;
	BufferedSmoothValue<int16_t> AccelerationY;
	BufferedSmoothValue<int16_t> AccelerationZ;
	

	/*
	* *******************************************************
	* *** Message #30  ATTITUDE                           ***
	* *** needed to use current Angles and axis speeds    ***
	* *******************************************************
	*/
	int32_t RollAngle = 0; // Roll angle (deg -180/180)
	int32_t PitchAngle = 0; // Pitch angle (deg -180/180)
	uint32_t YawAngle = 0; // Yaw angle (rad)
	uint32_t RollSpeed = 0; // Roll angular speed (rad/s)
	uint32_t PitchSpeed = 0; // Pitch angular speed (rad/s)
	uint32_t YawSpeed = 0; // Yaw angular speed (rad/s)

	/*
	* *******************************************************
	* *** Message #63  GLOBAL_POSITION_INT_COV            ***
	* *** Needed for Date/Time submission to RC           ***
	* *******************************************************
	*/
	time_t GpsTimeUnixUtc = 0; // Timestamp (microseconds since UNIX epoch) in UTC.
	// 0 for unknown.
	// Commonly filled by the precision time source of a GPS receiver.

	// Message #65 RC_CHANNELS
	uint8_t ChannelCount = 0; // Total number of RC channels being received.
	// This can be larger than 18, indicating that more channels are available but
	// not given in this message. This value should be 0 when no RC channels are available.
	uint16_t ChannelRaw[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // RC channel x input value, in microseconds.
	
	/*
	* *******************************************************
	* *** Message #74  VFR_HUD                            ***
	* *******************************************************
	*/
	int32_t  Airspeed = 0;    // Current airspeed in m/s
	uint32_t Groundspeed = 0; // Current ground speed in m/s
	uint32_t Heading = 0; // Current heading in degrees, in compass units (0..360, 0=north)
	uint16_t Throttle = 0; // Current throttle setting in integer percent, 0 to 100
	// FrSky Taranis uses the first recieved value after 'PowerOn' or  'Telemetry Reset'  as zero altitude
	int32_t BarAltitude = 0; // 100 = 1m
	int32_t ClimbRate = 0; // 100 = 1m/s
	
	/*
	* *******************************************************
	* *** Message #253  MAVLINK_MSG_ID_STATUSTEXT         ***
	* *******************************************************
	*/
	uint16_t StatusSeverity = 255;
	uint16_t StatusSendCount = 0;
	uint16_t StatusTextId = 0;
	mavlink_statustext_t StatusText;

	/*
	MAV_SEVERITY_EMERGENCY=0,     System is unusable. This is a "panic" condition.
	MAV_SEVERITY_ALERT=1,         Action should be taken immediately. Indicates error in non-critical systems.
	MAV_SEVERITY_CRITICAL=2,      Action must be taken immediately. Indicates failure in a primary system.
	MAV_SEVERITY_ERROR=3,         Indicates an error in secondary/redundant systems.
	MAV_SEVERITY_WARNING=4,       Indicates about a possible future error if this is not resolved within a given timeframe. Example would be a low battery warning.
	MAV_SEVERITY_NOTICE=5,        An unusual event has occured, though not an error condition. This should be investigated for the root cause.
	MAV_SEVERITY_INFO=6,          Normal operational messages. Useful for logging. No action is required for these messages.
	MAV_SEVERITY_DEBUG=7,         Useful non-operational messages that can assist in debugging. These should not occur during normal operation.
	MAV_SEVERITY_ENUM_END=8,
	*/

	/*
	* *******************************************************
	* *** These are special for FrSky:                    ***
	* *******************************************************
	*/
	int32_t GpsStatus = 0; // (_sat_visible * 10) + _fixtype
	// ex. 83 = 8 sattelites visible, 3D lock
	
	/*
	* *******************************************************
	* *** Lipo Cell Monitoring                            ***
	* *******************************************************
	*/
	uint8_t CellCount = 0;
	//BufferedSmoothValue<float> CellVoltage[MAXCELLS];
	
	BufferedSmoothValue<float> LipoVoltage;

	/*
	* *******************************************************
	* *** Variables needed for Mavlink Connection Status  ***
	* *** and starting FrSkySPort Telemetry               ***
	* *******************************************************
	*/
	bool MavLink_Connected = 0; // Connected or Not
	unsigned long StartTelemetry = 30000; // Start Telemetry after 30s (or 5s after init)
	bool TelemetryInitialized = 0; // Is FrSkySPort Telemetry initialized
};


inline MAVLinkData::MAVLinkData()
{	
	const int accBuffSize = 128;
	BufferedSmoothValue<int16_t> AccelerationX(accBuffSize, Coefficients::BlackmannHarris, 21);
	BufferedSmoothValue<int16_t> AccelerationY(accBuffSize, Coefficients::BlackmannHarris, 21);
	BufferedSmoothValue<int16_t> AccelerationZ(accBuffSize, Coefficients::BlackmannHarris, 21);
	BufferedSmoothValue<float> LipoVoltage(accBuffSize, Coefficients::Hamming, 21);
}

inline MAVLinkData::~MAVLinkData()
{
}

#endif

