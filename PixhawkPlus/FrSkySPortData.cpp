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

#include "FrSkySPortData.h"

	void FrSkySPortData::Init()
	{
		//Configure the _telemetry serial port and sensors (remember to use & to specify a pointer to sensor)
		_telemetry.begin(FrSkySportSingleWireSerial::SERIAL_1, &_fas, &_flvss1, &_gps, &_sp2uart, &_rpm, &_vario, &_fuel, &_acc);
		Initialized = 1;
	}

	void FrSkySPortData::Process()
	{
		_telemetry.send();

		SetVoltage();
		SetCellVoltage();
		SetGPS();
		SetRPM();
		SetPitchAndRoll();
		SetVariometer();
		SetAcceleration();
		SetFuel();
	}

	/*
	* *******************************************************
	* *** Set current/voltage sensor (FAS) data           ***
	* *******************************************************
	* set Voltage source to FAS in menu to use this data for battery voltage,
	* set Current source to FAS in menu to use this data for current readings
	*/
	void FrSkySPortData::SetVoltage() {

		FASVoltage = _m->LipVoltage;
		_fas.setData(_m->CurrentBattery / 10.0, FASVoltage / 10.0);     // Battery voltage in volts
		
#ifdef DEBUG_FrSkySportTelemetry_FAS
			debugSerial.printf("%d\tFrSky voltage: FAS(0x0210): %f, Curr(0x0200): %d", millis(), FASVoltage, _m->CurrentBattery);
#endif

	}

	/*
	* *******************************************************
	* *** Set current/voltage sensor (FAS) data           ***
	* *******************************************************
	* set LiPo voltage sensor (FLVSS) data (we use two sensors to simulate 8S battery)
	* set Voltage source to Cells in menu to use this data for battery voltage
	*/
	void FrSkySPortData::SetCellVoltage() {
		//
		//#ifdef DEBUG_FrSkySportTelemetry_FLVSS
		//	debugSerial.print(millis());
		//	debugSerial.print("\tmaxCells: ");
		//	debugSerial.println(MAXCELLS);
		//	for (int i = 0; i < MAXCELLS; i++) {
		//		debugSerial.print(millis());
		//		debugSerial.print("\tZelle[");
		//		debugSerial.print(i);
		//		debugSerial.print("]: ");
		//		debugSerial.print(lscm.GetCellVoltageAsUint32_T(i) / 1000.0);
		//		debugSerial.println("Volt");
		//
		//	}
		//#endif
		//
		//	switch (lscm.GetCellsInUse()) {
		//	case 1:
		//		_flvss1.setData(lscm.GetCellVoltageAsUint32_T(0) / 1000.0);
		//		break;
		//	case 2:
		//		_flvss1.setData(lscm.GetCellVoltageAsUint32_T(0) / 1000.0, lscm.GetCellVoltageAsUint32_T(1) / 1000.0);
		//		break;
		//	case 3:
		//		_flvss1.setData(lscm.GetCellVoltageAsUint32_T(0) / 1000.0, lscm.GetCellVoltageAsUint32_T(1) / 1000.0, lscm.GetCellVoltageAsUint32_T(2) / 1000.0);
		//		break;
		//	case 4:
		//		_flvss1.setData(lscm.GetCellVoltageAsUint32_T(0) / 1000.0, lscm.GetCellVoltageAsUint32_T(1) / 1000.0, lscm.GetCellVoltageAsUint32_T(2) / 1000.0, lscm.GetCellVoltageAsUint32_T(3) / 1000.0);
		//		break;
		//	case 5:
		//		_flvss1.setData(lscm.GetCellVoltageAsUint32_T(0) / 1000.0, lscm.GetCellVoltageAsUint32_T(1) / 1000.0, lscm.GetCellVoltageAsUint32_T(2) / 1000.0, lscm.GetCellVoltageAsUint32_T(3) / 1000.0, lscm.GetCellVoltageAsUint32_T(4) / 1000.0);
		//		break;
		//	case 6:
		//		_flvss1.setData(lscm.GetCellVoltageAsUint32_T(0) / 1000.0, lscm.GetCellVoltageAsUint32_T(1) / 1000.0, lscm.GetCellVoltageAsUint32_T(2) / 1000.0, lscm.GetCellVoltageAsUint32_T(3) / 1000.0, lscm.GetCellVoltageAsUint32_T(4) / 1000.0, lscm.GetCellVoltageAsUint32_T(5) / 1000.0);
		//		break;
		//	}
		//
		////#elif defined USE_FLVSS_FAKE_SENSOR_DATA
		////	float fake_cell_voltage = (ap_voltage_battery / ap_cell_count) / 1000.0;
		//
		//#ifdef DEBUG_FrSkySportTelemetry_FLVSS
		//	debugSerial.print(millis());
		//	debugSerial.print("\tmaxCells: ");
		//	debugSerial.println(ap_cell_count);
		//	for (int i = 0; i < ap_cell_count; i++) {
		//		debugSerial.print(millis());
		//		debugSerial.print("\tZelle[");
		//		debugSerial.print(i);
		//		debugSerial.print("]: ");
		//		debugSerial.print(fake_cell_voltage);
		//		debugSerial.println("Volt");
		//
		//	}
		//#endif
		//
		//	switch (ap_cell_count) {
		//	case 1:
		//		_flvss1.setData(fake_cell_voltage);
		//		break;
		//	case 2:
		//		_flvss1.setData(fake_cell_voltage, fake_cell_voltage);
		//		break;
		//	case 3:
		//		_flvss1.setData(fake_cell_voltage, fake_cell_voltage, fake_cell_voltage);
		//		break;
		//	case 4:
		//		_flvss1.setData(fake_cell_voltage, fake_cell_voltage, fake_cell_voltage, fake_cell_voltage);
		//		break;
		//	case 5:
		//		_flvss1.setData(fake_cell_voltage, fake_cell_voltage, fake_cell_voltage, fake_cell_voltage, fake_cell_voltage);
		//		break;
		//	case 6:
		//		_flvss1.setData(fake_cell_voltage, fake_cell_voltage, fake_cell_voltage, fake_cell_voltage, fake_cell_voltage, fake_cell_voltage);
		//		break;
		//	}
	}

	/*
	* *******************************************************
	* *** Set GPS data                                    ***
	* *******************************************************
	*/
	void FrSkySPortData::SetGPS()
	{
		if (_m->FixType >= 3)
		{
			/*
			if(ap_longitude < 0)
			longitude=((abs(ap_longitude)/100)*6);//  | 0xC0000000;
			else
			longitude=((abs(ap_longitude)/100)*6);//  | 0x80000000;

			if(ap_latitude < 0 )
			latitude=((abs(ap_latitude)/100)*6);// | 0x40000000;
			else
			latitude=((abs(ap_latitude)/100)*6);
			*/
			_gps.setData(_m->Latitude / 1E7, _m->Longitude / 1E7,    // Latitude and longitude in degrees decimal (positive for N/E, negative for S/W)
				_m->GpsAltitude / 10.0,         // Altitude (AMSL, NOT WGS84), in meters * 1000 (positive for up). Note that virtually all GPS modules provide the AMSL altitude in addition to the WGS84 altitude.
				_m->GpsSpeed * 10.0,// / 100.0,            // GPS ground speed (m/s * 100). If unknown, set to: UINT16_MAX
				_m->Heading,                     // Heading, in degrees * 100, 0.0..359.99 degrees. If unknown, set to: UINT16_MAX
				_m->GpsHdop);                   // GPS HDOP horizontal dilution of position in cm (m*100)
												//              ap_cog,                         // Course over ground (NOT heading, but direction of movement) in degrees * 100, 0.0..359.99 degrees. If unknown, set to: UINT16_MAX

#ifdef DEBUG_FrSkySportTelemetry_GPS
			if (millis() > GPS_debug_time) {
				debugSerial.print(millis());
				debugSerial.print("\tAPM Latitude:\t");
				debugSerial.print(_m->Latitude);
				debugSerial.print("\tAPM Longitude:\t\t");
				debugSerial.println(_m->Longitude);
				debugSerial.print(millis());
				/*debugSerial.print("\tFrSky Latitude:\t");
				debugSerial.print(ap_latitude / 1E7);
				debugSerial.print("\tFrSky Longitude:\t");
				debugSerial.println(ap_longitude / 1E7);*/
				debugSerial.print(millis());
				debugSerial.print("\tGPSAlt: ");
				debugSerial.print(_m->GpsAltitude / 10.0);
				debugSerial.print("cm");
				debugSerial.print("\tGPSSpeed: ");
				debugSerial.print((_m->GpsSpeed / 100.0));
				debugSerial.print("m/s");

				debugSerial.print("\tCog: ");
				debugSerial.print(_m->Cog);
				debugSerial.print("�");

				debugSerial.print("\tHeading: ");
				debugSerial.print(_m->Heading);
				debugSerial.print("�");
				debugSerial.print("\tHDOP (A2): ");
				debugSerial.print(_m->GpsHdop);
				/*
				debugSerial.print("\tDATE: ");
				debugSerial.print(year(ap_gps_time_unix_utc));
				debugSerial.print("-");
				debugSerial.print(month(ap_gps_time_unix_utc));
				debugSerial.print("-");
				debugSerial.print(day(ap_gps_time_unix_utc));
				debugSerial.print("\tTIME: ");
				debugSerial.print(hour(ap_gps_time_unix_utc));
				debugSerial.print(":");
				debugSerial.print(minute(ap_gps_time_unix_utc));
				debugSerial.print(":");
				debugSerial.print(second(ap_gps_time_unix_utc));
				*/
				debugSerial.println();
				debugSerial.println();
				GPS_debug_time = millis() + 500;
			}
#endif
		}
	}

	/*
	* *******************************************************
	* *** Set RPM/Temperature sensor data                 ***
	* *******************************************************
	*
	* 16 bit value:
	* * bit 1     : armed
	* * bit 2 -  5: severity +1 (0 means no message)
	* * bit 6 - 15: number representing a specific text
	*/
	void FrSkySPortData::SetRPM()
	{
		uint32_t ap_status_value = _m->BaseMode & 0x01;

		// If we have a message-text to report (we send it multiple times to make sure it arrives even on _telemetry glitches)
		if (_m->StatusSendCount > 0 && _m->StatusTextId > 0)
		{
			// Add bits 2-15
			ap_status_value |= (((_m->StatusSeverity + 1) & 0x0F) << 1) | ((_m->StatusTextId & 0x3FF) << 5);

			// put message into _status queue if queue is smaller than 15 items
			// ap_status_send_count is set to 1. this happens only ones.
			if (StatusQueuelevel() < 30)
				StatusEqueue(ap_status_value);

			_m->StatusSendCount--;
			if (_m->StatusSendCount == 0)
			{
				// Reset severity and text-message after we have sent the message
				_m->StatusSeverity = 0;
				_m->StatusTextId = 0;
			}
		}

#ifdef DEBUG_FrSkySportTelemetry_RPM
		if (ap_status_value > 0) {
			debugSerial.print(millis());
			debugSerial.print("\tRPM (Throttle/battery_remain): ");
			debugSerial.print(_m->Throttle * 200 + _m->BatteryRemaining * 2);
			debugSerial.print("\tT1 ((ap_sat_visible * 10) + ap_fixtype): ");
			debugSerial.print(_m->GpsStatus);
			debugSerial.print("\tT2 (Armed Status + Severity + Statustext): ");
			debugSerial.print(ap_status_value);
			debugSerial.println();
		}
#endif

		// we dequeue _status every 1400ms T2 sensor sends all 1000ms.

		static long dequeue_request = 0;
		long actualtime = millis();
		if (actualtime > dequeue_request + 1400) {

			//returns 0 if statusDequeue is empty
			my_dequeue_status_value = StatusDequeue();
			// if statusDequeue() returns 0 we set armed _status from ap_status_value
			if (my_dequeue_status_value == 0)
				my_dequeue_status_value = ap_status_value;
			dequeue_request = actualtime;
		}

		_rpm.setData(_m->Throttle * 200 + _m->BatteryRemaining * 2,    // * 2 if number of blades on Taranis is set to 2 + First 4 digits reserved for battery remaining in %
			_m->GpsStatus,         // (ap_sat_visible * 10) + ap_fixtype eg. 83 = 8 sattelites visible, 3D lock
			my_dequeue_status_value);   // Armed Status + Severity + Statustext


	}

	/*
	* *****************************************************
	* *** Set SP2UART sensor data ( A3 & A4 )           ***
	* *****************************************************
	*/
	void FrSkySPortData::SetPitchAndRoll()
	{
#ifdef DEBUG_FrSkySportTelemetry_A3A4
		debugSerial.print(millis());
		debugSerial.print("\tRoll Angle (A3): ");
		debugSerial.print(Handle_A2_A3_value((_m->RollAngle + 180) / scalefactor));
		debugSerial.print("\tPitch Angle (A4): ");
		debugSerial.print(Handle_A2_A3_value((_m->PitchAngle + 180) / scalefactor));
		debugSerial.println();
#endif
		_sp2uart.setData(Handle_A2_A3_value((_m->RollAngle + 180) / scalefactor),     // Roll Angle
			Handle_A2_A3_value((_m->PitchAngle + 180) / scalefactor));   // Pitch Angle

	}

	/*
	* *******************************************************
	* *** Set variometer data                             ***
	* *******************************************************
	* set Variometer source to VSpd in menu to use the vertical speed data from this sensor for variometer.
	*/
	void FrSkySPortData::SetVariometer()
	{
#ifdef DEBUG_FrSkySportTelemetry_VARIO
		debugSerial.print(millis());
		debugSerial.print("\tCurrent altitude: ");
		debugSerial.print(_m->BarAltitude / 100.0);
		debugSerial.print("m\tCurrent climb rate in meters/second: ");
		debugSerial.print(_m->ClimbRate);
		debugSerial.print("m/s");
		debugSerial.println();
#endif
		_vario.setData(_m->BarAltitude, _m->ClimbRate);
	}

	/*
	* *******************************************************
	* *** Set Accelerometer data                          ***
	* *******************************************************
	*/
	void FrSkySPortData::SetAcceleration()
	{
#ifdef DEBUG_FrSkySportTelemetry_ACC		
		debugSerial.printf("%d\tAccx: %d, AccY: %d, AccZ: %d\r\n", 
			millis(), _m->AccelerationX.GetSmoothedValue(), _m->AccelerationY.GetSmoothedValue(), _m->AccelerationZ.GetSmoothedValue());
#endif
		_acc.setData(_m->AccelerationX.GetSmoothedValue(), _m->AccelerationY.GetSmoothedValue(), _m->AccelerationZ.GetSmoothedValue());
	}

	/*
	* *****************************************************
	* *** Set Fuel sensor data                          ***
	* *****************************************************
	* Used for Flight Mode
	*/
	void FrSkySPortData::SetFuel()
	{
#ifdef DEBUG_FrSkySportTelemetry_FLIGHTMODE
		debugSerial.print(millis());
		debugSerial.print("\tFlightmode: ");
		debugSerial.print(_m->CustomMode);
		debugSerial.println();
#endif
		if (_m->CustomMode >= 0) {
			_fuel.setData(_m->CustomMode);
		}
	}


	/*
	* *****************************************************
	* *** Helper function "queue buffer" to queue       ***
	* *** Armed Status, Severity - Statustext messages  ***
	* *****************************************************
	* Used for Flight Mode
	*/

	// Put something into the buffer. Returns 0 when the buffer was full,
	// 1 when the stuff was put sucessfully into the buffer
	int FrSkySPortData::StatusEqueue(int32_t val)
	{
		int32_t newtail = (statusRingTail + 1) % statusRingsize;
		if (newtail == statusRingHead) {
			// Buffer is full, do nothing
			return 0;
		}
		else {
			statusRing[statusRingTail] = val;
			statusRingTail = newtail;
			return 1;
		}
	}

	// Return number of elements in the queue.
	int FrSkySPortData::StatusQueuelevel() {
		return statusRingTail - statusRingHead + (statusRingHead > statusRingTail ? statusRingsize : 0);
	}

	// Get something from the queue. 0 will be returned if the queue
	// is empty
	int32_t FrSkySPortData::StatusDequeue()
	{
		if (statusRingHead == statusRingTail) {
			return 0;
		}
		else {
			uint32_t val = statusRing[statusRingHead];
			statusRingHead = (statusRingHead + 1) % statusRingsize;
			return val;
		}
	}

	uint32_t FrSkySPortData::Handle_A2_A3_value(uint32_t value)
	{
		return (value * 330 - 165) / 0xFF;
	}

