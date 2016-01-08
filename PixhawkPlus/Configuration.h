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
#define _MavLinkSerial      Serial1
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
#define MAXCELLS 4

// 4S network with customer resistors:
// A    B     =Volt Range
// 993  3320  3.2330
//1960  1240  3.2550
//3600  1240  3.2281
//5110  1240  3.2806
#define CELL_DIVIDER1 1910.652081
#define CELL_DIVIDER2 961.8219697
#define CELL_DIVIDER3 635.9153519
#define CELL_DIVIDER4 484.6976855

#define ANALOG1 7
#define ANALOG2 6
#define ANALOG3 5
#define ANALOG4 4

//LED Control
#define LEDSPERARM 8

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
#define DEBUG_OTHER_HEARTBEAT

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
#define DEBUG_LIPO_SINGLE_CELL_MONITOR 0  //SET TO 1 TO TURN ON

#endif