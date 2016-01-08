
#include "Coefficients.h"
#include "BufferedSmoothValue.h"
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
	debugSerial.print("Setup Complete\r\n");
}

void loop()
{
	controller.Process();	
}
