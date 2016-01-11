#pragma region Copyright
//This file is part of Pixhawk Plus
//
//Jim Rowe Copyright (c) 2015
//https://github.com/jrowe88/PixhawkPlus
//
//  Original concept and big props to Rolf Blomgren - http://diydrones.com/forum/topics/amp-to-frsky-x8r-sport-converter
//  Adapted and modified from numerous other contributors - https://github.com/Clooney82/MavLink_FrSkySPort/   
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

#include <FastLED.h>
#include <OctoWS2811.h>
#include <GCS_MAVLink.h>
#include <FrSkySportSingleWireSerial.h>
#include <FrSkySportSensorAss.h>
#include <FrSkySportSensor.h>
#include <FrSkySportTelemetry.h>
#include <FrSkySportSensorVario.h>
#include <FrSkySportSensorSp2uart.h>
#include <FrSkySportSensorRpm.h>
#include <FrSkySportSensorGps.h>
#include <FrSkySportSensorFuel.h>
#include <FrSkySportSensorFlvss.h>
#include <FrSkySportSensorFas.h>
#include <FrSkySportSensorAcc.h>
#include "MavlinkControl.h"

MavLinkController controller;


void setup()
{			
	controller.Initialize();	
}

void loop()
{
	controller.Process();	
}
