#include "Configuration.h"
#include "StatusTextParser.h"
#include "MavlinkInterpreter.h"

MavLinkInterpreter::MavLinkInterpreter(MAVLinkData* messages)
{
	_m = messages;
	_connected = false;
}

MavLinkInterpreter::~MavLinkInterpreter()
{
}

/*
 * *******************************************************
 * *** Setup Mavlink:                                  ***
 * *******************************************************
 */
void MavLinkInterpreter::Setup()
{
	
	_MavLinkSerial.begin(58824); //_MavLinkSerialBaud
	_MavLinkSerial.clear();

	debugSerial.begin(debugSerialBaud);

	pinMode(LED_BOARD_P, OUTPUT);

	_lastConnected = 0;
	
	for (int i = 0; i < 10; i++)
	{
		digitalWrite(LED_BOARD_P, 1);
		delay(50);
		digitalWrite(LED_BOARD_P, 0);
		delay(50);
	}

	debugSerial.printf("Serial1 avail for read? %d bytes to read\r\n", _MavLinkSerial.available());
	
}

/*
 * *******************************************************
 * *** Send Mavlink Heartbeat:                         ***
 * *******************************************************
 */
void MavLinkInterpreter::SendHeartbeat()
{
	// Send a heartbeat over the mavlink
#ifdef DEBUG_APM_CONNECTION	
	debugSerial.printf("%d\tSending heartbeat message.\r\n", millis());
#endif

	//mavlink_msg_heartbeat_pack(123, 123, &_msg, MAV_TYPE_ONBOARD_CONTROLLER, MAV_AUTOPILOT_INVALID, MAV_MODE_PREFLIGHT, <CUSTOM_MODE>, MAV_STATE_STANDBY);
	mavlink_msg_heartbeat_pack(_mavlink_system.sysid, _mavlink_system.compid, &_msg, 18, 8, 0, 0, 4);
	_len = mavlink_msg_to_send_buffer(_buf, &_msg);
	_MavLinkSerial.write(_buf, _len);
}

/*
 * *******************************************************
 * *** Config Mavlink connection:                      ***
 * *******************************************************
 */
void MavLinkInterpreter::ConfigureConnection()
{
#ifdef DEBUG_APM_CONNECTION	
	debugSerial.printf("%d\tSetting up Mavlink streams\r\n", millis());
	debugSerial.printf("%d\tSerial available for write? %d bytes avail\r\n", millis(), _MavLinkSerial.availableForWrite());
#endif
	digitalWrite(LED_BOARD_P, HIGH);
	// mavlink_msg_request_data_stream_pack(0xFF,0xBE,&_msg,1,1,MAV_DATA_STREAM_EXTENDED_STATUS, MSG_RATE, START);
	mavlink_msg_request_data_stream_pack(_mavlink_system.sysid, _mavlink_system.compid, &_msg, AP_SYSID, AP_CMPID, MAV_DATA_STREAM_EXTENDED_STATUS, MSG_RATE, START);
	_len = mavlink_msg_to_send_buffer(_buf, &_msg);
	_MavLinkSerial.write(_buf, _len);
#ifdef DEBUG_APM_CONNECTION	
	debugSerial.printf("%d\tWrote MAV_DATA_STREAM_EXTENDED_STATUS request\r\n", millis());
#endif
	delay(10);

	mavlink_msg_request_data_stream_pack(_mavlink_system.sysid, _mavlink_system.compid, &_msg, AP_SYSID, AP_CMPID, MAV_DATA_STREAM_EXTRA2, MSG_RATE, START);
	_len = mavlink_msg_to_send_buffer(_buf, &_msg);
	_MavLinkSerial.write(_buf, _len);
#ifdef DEBUG_APM_CONNECTION	
	debugSerial.printf("%d\tWrote MAV_DATA_STREAM_EXTRA2 request\r\n", millis());
#endif

	delay(10);
	mavlink_msg_request_data_stream_pack(_mavlink_system.sysid, _mavlink_system.compid, &_msg, AP_SYSID, AP_CMPID, MAV_DATA_STREAM_RAW_SENSORS, MSG_RATE, START);
	_len = mavlink_msg_to_send_buffer(_buf, &_msg);
	_MavLinkSerial.write(_buf, _len);
#ifdef DEBUG_APM_CONNECTION	
	debugSerial.printf("%d\tWrote MAV_DATA_STREAM_RAW_SENSORS request\r\n", millis());
#endif

#ifdef USE_RC_CHANNELS
	delay(10);
	mavlink_msg_request_data_stream_pack(_mavlink_system.sysid, _mavlink_system.compid, &_msg, AP_SYSID, AP_CMPID, MAV_DATA_STREAM_RC_CHANNELS, MSG_RATE, START);
	_len = mavlink_msg_to_send_buffer(_buf, &_msg);
	_MavLinkSerial.write(_buf, _len);
#ifdef DEBUG_APM_CONNECTION	
	debugSerial.printf("%d\tWrote MAV_DATA_STREAM_RC_CHANNELS request\r\n", millis());
#endif
#endif

	digitalWrite(LED_BOARD_P, LOW);
	_send_mavlink_connection_config++;
}

/*
 * *******************************************************
 * *** Check Mavlink Connection:                       ***
 * *******************************************************
 */
void MavLinkInterpreter::CheckConnection()
{
	if (millis() > _hb_timer)
	{
		_hb_timer = millis() + 1500;
		if (_connected)
		{
			//Mavlink_send_heartbeat();
		}

		// Sending Mavlink configuration after 10sec, to give FC enough time to boot.
		if (_send_mavlink_connection_config == 0)
		{
			if (millis() > 10000)
			{
				ConfigureConnection();
			}
		}

		if (millis() > MavLink_Connected_timer && ++_hb_count_lost > 5)
		{ // if no HEARTBEAT from APM  in 1.5s then we are not connected			
			_lastConnected = _connected;
			_connected = false;
			_hb_count = 0;				//reset the counter
#ifdef DEBUG_APM_CONNECTION
			if (_lastConnected == 1)
			{
				debugSerial.printf("%d\tLost MavLink connection...\r\n", millis());
			}
			else
			{
				debugSerial.printf("%d\tMavLink not connected.\r\n", millis());
			}
#endif 
		}
		else if (_hb_count >= 10)
		{ // If  received > 10 heartbeats from MavLink then we are connected
			_lastConnected = _connected;
			_connected = 1;
			_hb_count_lost = 0;
#ifdef DEBUG_APM_CONNECTION
			if (_hb_count == 10)
			{
				debugSerial.println("###############################################");
				debugSerial.printf("%d\tMavLink connection etablished. Version: %d\r\n", millis(), _m->MavlinkVersion);
				debugSerial.println("###############################################");
				_send_mavlink_connection_config = 0;
				_connected = true;
			}
#endif
			digitalWrite(LED_BOARD_P, HIGH); // LED will be ON when connected to MavLink, else it will slowly blink
		}
	}
}

/*
 * *******************************************************
 * *** Receive Mavlink Messages:                       ***
 * *******************************************************
 */
void MavLinkInterpreter::Receive()
{
	//debugSerial.printf("%d \Serial1 available: %d\r\n", millis(), _MavLinkSerial.available());

	while (_MavLinkSerial.available())
	{
		uint8_t c = _MavLinkSerial.read();
		if (mavlink_parse_char(MAVLINK_COMM_0, c, &_msg, &_status) && (AP_SYSID == _msg.sysid && AP_CMPID == _msg.compid)) // only proceed with autopilot messages
		{
			/*
			 * *******************************************************
			 * *** Ardupilot MAVLINK Message                       ***
			 * *******************************************************
			 */
#ifdef DEBUG_APM_MAVLINK_MSGS
			debugSerial.printf("%d \tMSG: %d\r\n", millis(), _msg.msgid);
#endif
			switch (_msg.msgid)
			{
			case MAVLINK_MSG_ID_HEARTBEAT: //0
				ProcessHeartbeat();
				break;
			case MAVLINK_MSG_ID_SYS_STATUS: //1
				ProcessSystemStatus();
				break;
			case MAVLINK_MSG_ID_GPS_RAW_INT: //24
				ProcessGps();
				break;
			case MAVLINK_MSG_ID_RAW_IMU: //27
				ProcessImu();
				break;
			case MAVLINK_MSG_ID_ATTITUDE: //30
				ProcessAttitude();
				break;
			case MAVLINK_MSG_ID_GLOBAL_POSITION_INT_COV: // 63
				ProcessGpsTime();
				break;
			case MAVLINK_MSG_ID_RC_CHANNELS: //65
				ProcessRCChannels();
				break;
			case MAVLINK_MSG_ID_VFR_HUD: //74
				ProcessHUD();
				break;				
			case MAVLINK_MSG_ID_STATUSTEXT: //253
				ProcessStatusText();
				break;
			default:
				break;
			}
		}
		else if (GB_SYSID == _msg.sysid && GB_CMPID == _msg.compid) // only proceed with gimbal messages
		{
			/*
			 * *******************************************************
			 * *** GIMBAL MAVLINK Message                          ***
			 * *******************************************************
			 */
			switch (_msg.msgid)
			{
				/*
				 * *****************************************************
				 * *** MAVLINK Message #0 - HEARTBEAT                ***
				 * *****************************************************
				 */
			case MAVLINK_MSG_ID_HEARTBEAT: // 0
#ifdef DEBUG_GIMBAL_HEARTBEAT				
				debugSerial.printf("%d\tGIMBAL MESSAGE: %d, custom_mode: %d\r\n", millis(),
					(mavlink_msg_heartbeat_get_base_mode(&_msg) & 0x80) > 7, mavlink_msg_heartbeat_get_custom_mode(&_msg));
#endif
				break;
			}
		}
		else
		{			
#ifdef DEBUG_OTHER_HEARTBEAT
				debugSerial.printf("%d\tOTHER MESSAGE: sysid: %d, compid: %d, base_mode: %d, custom_mode: %d\r\n",
					millis(),
					_msg.sysid,
					_msg.compid,
					(mavlink_msg_heartbeat_get_base_mode(&_msg) & 0x80) > 7,
					mavlink_msg_heartbeat_get_custom_mode(&_msg));
#endif
		}
	}
}

void MavLinkInterpreter::ProcessHUD() const
{
	_m->Groundspeed = mavlink_msg_vfr_hud_get_groundspeed(&_msg); // 100 = 1m/s
	_m->Heading = mavlink_msg_vfr_hud_get_heading(&_msg); // 100 = 100 deg
	_m->Throttle = mavlink_msg_vfr_hud_get_throttle(&_msg); //  100 = 100%
	_m->BarAltitude = mavlink_msg_vfr_hud_get_alt(&_msg) * 100; //  m
	_m->ClimbRate = mavlink_msg_vfr_hud_get_climb(&_msg) * 100; //  m/s
#ifdef DEBUG_APM_VFR_HUD				
	debugSerial.printf("\tMAVLINK_MSG_ID_VFR_HUD: groundspeed: %d, heading: %d, throttle: %d, alt: %d, climbrate: %d\r\n",
		_m->Groundspeed, _m->Heading, _m->Throttle, _m->BarAltitude, _m->ClimbRate);
#endif
}

void MavLinkInterpreter::ProcessStatusText()
{
	mavlink_msg_statustext_decode(&_msg, &_statusText);
	_m->StatusSeverity = _statusText.severity;
	_m->StatusSendCount = 1; // since messages are queued we only need one send_count
	if (AC_VERSION == 3.3)
	{
		StatusTextParser::parseStatusText_v3_3(_statusText.severity, _statusText.text, _m);
	}
	else
	{
		StatusTextParser::parseStatusText_v3_2(_statusText.severity, _statusText.text, _m);
	}

#ifdef DEBUG_APM_STATUSTEXT
	debugSerial.print(millis());
	debugSerial.printf("%d\tAC_VERSION: %3.2f, textId: %d, tMAVLINK_MSG_ID_STATUSTEXT-severity: %d, text %s",
		millis(), AC_VERSION, _m->StatusTextId, _statusText.severity, _statusText.text);
#endif	
}

void MavLinkInterpreter::ProcessRCChannels() const
{
    _m->ChannelCount = mavlink_msg_rc_channels_get_chancount(&_msg);    // Number of RC Channels used
	_m->ChannelRaw[0] = mavlink_msg_rc_channels_get_chan1_raw(&_msg);   // The PPM values of the RC channels received.
	_m->ChannelRaw[1] = mavlink_msg_rc_channels_get_chan2_raw(&_msg);   // The standard PPM modulation is as follows:
	_m->ChannelRaw[2] = mavlink_msg_rc_channels_get_chan3_raw(&_msg);   // 1000 microseconds: 0%, 2000 microseconds: 100%.
	_m->ChannelRaw[3] = mavlink_msg_rc_channels_get_chan4_raw(&_msg);   // Individual receivers/transmitters might violate this specification.
	_m->ChannelRaw[4] = mavlink_msg_rc_channels_get_chan5_raw(&_msg);
	_m->ChannelRaw[5] = mavlink_msg_rc_channels_get_chan6_raw(&_msg);
	_m->ChannelRaw[6] = mavlink_msg_rc_channels_get_chan7_raw(&_msg);
	_m->ChannelRaw[7] = mavlink_msg_rc_channels_get_chan8_raw(&_msg);
	_m->ChannelRaw[8] = mavlink_msg_rc_channels_get_chan9_raw(&_msg);
	_m->ChannelRaw[9] = mavlink_msg_rc_channels_get_chan10_raw(&_msg);
	_m->ChannelRaw[10] = mavlink_msg_rc_channels_get_chan11_raw(&_msg);
	_m->ChannelRaw[11] = mavlink_msg_rc_channels_get_chan12_raw(&_msg);
	_m->ChannelRaw[12] = mavlink_msg_rc_channels_get_chan13_raw(&_msg);
	_m->ChannelRaw[13] = mavlink_msg_rc_channels_get_chan14_raw(&_msg);
	_m->ChannelRaw[14] = mavlink_msg_rc_channels_get_chan15_raw(&_msg);
	_m->ChannelRaw[15] = mavlink_msg_rc_channels_get_chan16_raw(&_msg);
	_m->ChannelRaw[16] = mavlink_msg_rc_channels_get_chan17_raw(&_msg);
	_m->ChannelRaw[17] = mavlink_msg_rc_channels_get_chan18_raw(&_msg);

//
#ifdef DEBUG_APM_RC_CHANNELS				
		debugSerial.printf("%d\tMAVLINK_MSG_ID_RC_CHANNELS: chancount: %d\r\n", millis(), _m->ChannelCount);
		
		for (size_t i = 0; i < 3; i++)
		{
			int j = i * 6;
			debugSerial.printf("\tRC%d: %d, RD%d: %d, RC%d: %d, RC%d: %d, RC%d: %d, RC%d: %d\r\n",
				j + 1, _m->ChannelRaw[j], j + 2, _m->ChannelRaw[j+1], j + 3, _m->ChannelRaw[j+2],
				j + 4, _m->ChannelRaw[j+3], j + 5, _m->ChannelRaw[j+4], j + 6, _m->ChannelRaw[j+5] );
		}
#endif

}

void MavLinkInterpreter::ProcessGpsTime() const
{
	_m->GpsTimeUnixUtc = mavlink_msg_global_position_int_cov_get_time_utc(&_msg);
#ifdef DEBUG_APM_GLOBAL_POSITION_INT_COV				
	debugSerial.printf("%d\tap_gps_time_unix_utc: %lld\r\n", millis(), _m->GpsTimeUnixUtc);
#endif
}

void MavLinkInterpreter::ProcessAttitude() const
{
	_m->RollAngle = mavlink_msg_attitude_get_roll(&_msg) * 180 / PI; //value comes in rads, convert to deg
																		 // Not upside down
	if (fabs(_m->RollAngle) <= 90)
	{
		_m->PitchAngle = mavlink_msg_attitude_get_pitch(&_msg) * 180 / PI; //value comes in rads, convert to deg
	}
	// Upside down
	else
	{
		_m->PitchAngle = 180 - mavlink_msg_attitude_get_pitch(&_msg) * 180 / PI; //value comes in rads, convert to deg
	}
	_m->YawAngle = (mavlink_msg_attitude_get_yaw(&_msg) + 3.1416) * 162.9747; //value comes in rads, add pi and scale to 0 to 1024

#ifdef DEBUG_APM_ATTITUDE				
	debugSerial.printf("%d\tMAVLINK Roll Angle: %d, Pitch: %d, Yaw: %d\r\n", millis(), _m->RollAngle, _m->PitchAngle, _m->YawAngle);
#endif

}

void MavLinkInterpreter::ProcessImu() const
{
	_m->AccelerationX.SetValue(mavlink_msg_raw_imu_get_xacc(&_msg) / 10);
	_m->AccelerationY.SetValue(mavlink_msg_raw_imu_get_yacc(&_msg) / 10);
	_m->AccelerationZ.SetValue(mavlink_msg_raw_imu_get_zacc(&_msg) / 10);

#ifdef DEBUG_APM_ACC
	debugSerial.printf("\tMAVLINK_MSG_ID_RAW_IMU: xacc: %d, yacc: %d, zacc: %d\r\n",
		mavlink_msg_raw_imu_get_xacc(&_msg), mavlink_msg_raw_imu_get_yacc(&_msg), mavlink_msg_raw_imu_get_zacc(&_msg));
#endif
}

void MavLinkInterpreter::ProcessGps() const
{
	_m->FixType = mavlink_msg_gps_raw_int_get_fix_type(&_msg); // 0 = No GPS, 1 =No Fix, 2 = 2D Fix, 3 = 3D Fix, 4 = DGPS, 5 = RTK
	_m->SatVisible = mavlink_msg_gps_raw_int_get_satellites_visible(&_msg); // numbers of visible satelites
	_m->GpsStatus = (_m->SatVisible * 10) + _m->FixType;
	_m->GpsHdop = mavlink_msg_gps_raw_int_get_eph(&_msg);
	// Max 8 bit
	if (_m->GpsHdop == 0 || _m->GpsHdop > 255)
	{
		_m->GpsHdop = 255;
	}
	if (_m->FixType >= 3)
	{
		_m->Latitude = mavlink_msg_gps_raw_int_get_lat(&_msg);
		_m->Longitude = mavlink_msg_gps_raw_int_get_lon(&_msg);
		_m->GpsAltitude = mavlink_msg_gps_raw_int_get_alt(&_msg); // 1m = 1000
		_m->GpsSpeed = mavlink_msg_gps_raw_int_get_vel(&_msg); // 100 = 1m/s
		_m->Cog = mavlink_msg_gps_raw_int_get_cog(&_msg) / 100;
	}
	else
	{
		_m->GpsSpeed = 0;
	}
#ifdef DEBUG_APM_GPS_RAW
	debugSerial.printf("%d\tMAVLINK_MSG_ID_GPS_RAW_INT: fixtype: %d, visiblesats: %d, _status: %d, gpsspeed: %d, hdop: %d, alt: %d\r\n",
		millis(), _m->FixType, _m->SatVisible, _m->GpsStatus, _m->GpsSpeed, _m->GpsHdop, _m->GpsAltitude);
#endif
}

void MavLinkInterpreter::ProcessSystemStatus() const
{

	//				ap_voltage_battery = lscm.GetAllLipoCells();
	//no lipo to network connected use reported mavlink_msg voltage
	//				if (_lscm.GetCellsInUse() == 0) ap_voltage_battery = mavlink_msg_sys_status_get_voltage_battery(&_msg);

	_m->VoltageBattery = mavlink_msg_sys_status_get_voltage_battery(&_msg); // 1 = 1mV
	_m->CurrentBattery = mavlink_msg_sys_status_get_current_battery(&_msg); // 1=10mA
	_m->BatteryRemaining = mavlink_msg_sys_status_get_battery_remaining(&_msg); //battery capacity reported in %
	
//	_averager->StoreVoltageReading(_m->VoltageBattery);
//	_averager->StoreCurrentReading(_m->CurrentBattery);

#ifdef DEBUG_APM_BAT
	debugSerial.printf("%d \tMAVLINK_MSG_ID_SYS_STATUS: voltage_battery: %d, current_battery: %d\r\n", millis(), _m->VoltageBattery, _m->CurrentBattery);
#endif
	uint8_t temp_cell_count;

	///*_m->CellCount = lscm.GetCellsInUse();
	//if (lscm.GetCellsInUse() == 0) {
	//	if (ap_voltage_battery > 21000) temp_cell_count = 6;
	//	else if (ap_voltage_battery > 17500) temp_cell_count = 5;
	//	else if (ap_voltage_battery > 12750) temp_cell_count = 4;
	//	else if (ap_voltage_battery > 8500)  temp_cell_count = 3;
	//	else if (ap_voltage_battery > 4250)  temp_cell_count = 2;
	//	else temp_cell_count = 0;
	//	if (temp_cell_count > ap_cell_count)
	//		ap_cell_count = temp_cell_count;
	//*/}

	//				/*if (ap_voltage_battery > 21000) temp_cell_count = 6;
	//				else if (ap_voltage_battery > 17500) temp_cell_count = 5;
	//				else if (ap_voltage_battery > 12750) temp_cell_count = 4;
	//				else if (ap_voltage_battery > 8500)  temp_cell_count = 3;
	//				else if (ap_voltage_battery > 4250)  temp_cell_count = 2;
	//				else temp_cell_count = 0;
	//				if (temp_cell_count > ap_cell_count)
	//					ap_cell_count = temp_cell_count;
	//*/
}

void MavLinkInterpreter::ProcessHeartbeat()
{
	_hb_count++;
	_m->BaseMode = (mavlink_msg_heartbeat_get_base_mode(&_msg) & 0x80) > 7;
	_m->CustomMode = mavlink_msg_heartbeat_get_custom_mode(&_msg);
	_m->MavlinkVersion = mavlink_msg_heartbeat_get_mavlink_version(&_msg);
	
	#ifdef DEBUG_APM_HEARTBEAT
		debugSerial.printf("%d \tHeartbeat Count: %d\r\n", millis(), _hb_count);
		debugSerial.printf("%d \tMAVLINK_MSG_ID_SYS_STATUS: base_mode: %d, custom: %d\r\n", millis(), _m->BaseMode, _m->CustomMode);
	#endif
			
	MavLink_Connected_timer = millis() + 1500;

}


