
#include "MavlinkControl.h"

void MavLinkController::Initialize()
{
	_lscm->SetCustomCellDivider(CELL_DIVIDER1, CELL_DIVIDER2, CELL_DIVIDER3, CELL_DIVIDER4);
	_lscm->SetCustomPins(ANALOG1, ANALOG2, ANALOG3, ANALOG4);
	_lscm->SetDebug(DEBUG_LIPO_SINGLE_CELL_MONITOR);
	_leds->Initialize();
	_mavLink->Setup();		
}

void MavLinkController::Process()
{
	_lscm->Process();	
	_mavLink->Receive();                       // receive MavLink communication
	_mavLink->CheckConnection();               // Check MavLink communication
	_leds->Process();
 
	if (_mavLinkConnected == 1)
	{
		if (_frSky->Initialized == 0)  //TODO: might want to send back info that MavLink not connected...
		{
			_frSky->Init();
		}
		else
		{
			_frSky->Process();			
		}
	}

}


