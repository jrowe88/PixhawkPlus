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
#ifndef _CONFIGURATION_h
#define _CONFIGURATION_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

/*
* *******************************************************
* *** Basic Configuration:                            ***
* *******************************************************
*/
#define debugSerial         Serial
#define debugSerialBaud     57600
#define _MavLinkSerial      Serial2
#define _MavLinkSerialBaud  57600
#define MAVLINK_P_TX        1
#define MAVLINK_P_RX        0
#define LED_BOARD_P         13
#define START               1
#define MSG_RATE            10      // Hertz
#define AP_SYSID            1       // autopilot system id
#define AP_CMPID            1       // autopilot component id
#define MY_SYSID            123     // teensy system id
#define MY_CMPID            123     // teensy component id
#define GB_SYSID            71      // gimbal system id
#define GB_CMPID            67      // gimbal component id

#define AC_VERSION          3.3     // 3.2 backwards compatibility

/*
* *******************************************************
* *** Enable Addons:                                  ***
* *******************************************************
*/
#define USE_SINGLE_CELL_MONITOR                         // Enable/disable single cell monitoring
#define USE_TEENSY_LED_SUPPORT                          // Enable LED-Controller functionality

/*
* *******************************************************
* *** Variables Definitions:                          ***
* *******************************************************
*/
//Single Cell LiPO monitor
#define MAXCELLS 6  //no more than 6 are currently supported
#define ANALOG_SAMPLE_BITS 13

#define CELL_DIVIDER1 1910.652081
#define CELL_DIVIDER2 961.8219697
#define CELL_DIVIDER3 635.9153519
#define CELL_DIVIDER4 484.6976855

#define ANALOG1 7
#define ANALOG2 6
#define ANALOG3 5
#define ANALOG4 4

//LED Control
#define LEDSPERARM 27

/*
* *******************************************************
* *** Debug Options:                                  ***
* *******************************************************
*/
// *** DEBUG MAVLink Messages:
#define DEBUG_APM_MAVLINK_MSGS              // Show all messages received from APM
#define DEBUG_APM_CONNECTION
#define DEBUG_APM_HEARTBEAT                 // MSG #0
#define DEBUG_APM_SYS_STATUS                // MSG #1   - not used -> use: DEBUG_APM_BAT
#define DEBUG_APM_BAT                       // Debug Voltage and Current received from APM
#define DEBUG_APM_GPS_RAW                   // MSG #24
#define DEBUG_APM_RAW_IMU                   // MSG #27  - not used -> use: DEBUG_APM_ACC
#define DEBUG_APM_ACC                       // Debug Accelerometer
#define DEBUG_APM_ATTITUDE                  // MSG #30
#define DEBUG_APM_GLOBAL_POSITION_INT_COV   // MSG #63  - planned - currently not implemented - not supported by APM
#define DEBUG_APM_RC_CHANNELS               // MSG #65
#define DEBUG_APM_VFR_HUD                   // MSG #74
#define DEBUG_APM_STATUSTEXT                // MSG #254 -
#define DEBUG_APM_PARSE_STATUSTEXT
#define DEBUG_GIMBAL_HEARTBEAT
//#define DEBUG_PROCESSING_STATE

// *** DEBUG FrSkySPort Telemetry:
#define DEBUG_FrSkySportTelemetry
#define DEBUG_FrSkySportTelemetry_FAS
#define DEBUG_FrSkySportTelemetry_FLVSS
#define DEBUG_FrSkySportTelemetry_GPS
#define DEBUG_FrSkySportTelemetry_RPM
#define DEBUG_FrSkySportTelemetry_A3A4
#define DEBUG_FrSkySportTelemetry_VARIO
#define DEBUG_FrSkySportTelemetry_ACC
#define DEBUG_FrSkySportTelemetry_FLIGHTMODE

// *** DEBUG other things:
#define DEBUG_AVERAGE_VOLTAGE
#define DEBUG_LIPO_SINGLE_CELL_MONITOR 1  //SET TO 1 TO TURN ON

#endif