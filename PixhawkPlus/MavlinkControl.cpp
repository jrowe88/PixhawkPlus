#pragma region Copyright
//This file is part of Pixhawk Plus
//
//Jim Rowe Copyright (c) 2015
//https://github.com/jrowe88/PixhawkPlus
//
//  Original concept and big props to Rolf Blomgren - http://diydrones.com/forum/topics/amp-to-frsky-x8r-sport-converter
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

#include "MavlinkControl.h"

void MavLinkController::Initialize()
{
	debugSerial.begin(debugSerialBaud);
	_lscm->SetVoltageRatio(CELL_DIVIDER1, CELL_DIVIDER2, CELL_DIVIDER3, CELL_DIVIDER4);
	_lscm->SetCustomPins(ANALOG1, ANALOG2, ANALOG3, ANALOG4);
	_lscm->SetDebug(DEBUG_LIPO_SINGLE_CELL_MONITOR);
	_leds->Initialize();
	_mavLink->Setup();		
}

void MavLinkController::Process()
{
	_mavLink->Receive();
	if (_mavLink->CheckConnection())
	{
		
		//if (_frSky->Initialized == 0)  //TODO: might want to send back info that MavLink not connected...
		//{
		//	_frSky->Init();
		//}
		//else
		//{
		//	_frSky->Process();			
		//}
	}
	_lscm->Process();
	_leds->Process();
}


