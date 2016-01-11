#pragma region Copyright
//This file is part of Pixhawk Plus
//
//Jim Rowe Copyright (c) 2015
//https://github.com/jrowe88/PixhawkPlus
//
//  Original concept and big props to Rolf Blomgren - http://diydrones.com/forum/topics/amp-to-frsky-x8r-sport-converter
//  Adapted and modified from numerous other contributors:
//     Michael Wolkstein - https://github.com/Clooney82/MavLink_FrSkySPort/   

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


#include "LipoSingleCellMonitor.h"

LipoSingleCellMonitor::LipoSingleCellMonitor(uint8_t expectedCells, uint8_t analogReadReso, MAVLinkData* data, float analogRefVoltage)
	: _totalVoltage(Coefficients::Hamming, 21)
{
	_messages = data;
	_referenceVoltage = analogRefVoltage;
	Initialize(expectedCells, analogReadReso);
}

// public functions
void LipoSingleCellMonitor::Process()
{
	if (millis() > _sampleLimitTimer)
	{
		SampleVoltage();
		_sampleLimitTimer = millis() + _minSamplePeriod;
	}
}

uint8_t LipoSingleCellMonitor::GetCellsInUse()
{
	return _cellsInUse;
}

float LipoSingleCellMonitor::GetCellVoltage(uint8_t cell)
{
	return 0;// _cellVoltages[cell].GetSmoothedValue();
}

float LipoSingleCellMonitor::GetAllLipoCells()
{
	return _totalVoltage.GetSmoothedValue();
}

uint8_t LipoSingleCellMonitor::GetMaxCells()
{
	return _MAXCELLS;
}

void LipoSingleCellMonitor::SetDebug(bool debug)
{
	_debug = debug;
}

void LipoSingleCellMonitor::SetVoltageRatio(double a)
{
	SetCellDivider(a, 0.0, 0.0, 0.0, 0.0, 0.0);
}

void LipoSingleCellMonitor::SetVoltageRatio(double a, double b)
{
	SetCellDivider(a, b, 0.0, 0.0, 0.0, 0.0);
}

void LipoSingleCellMonitor::SetVoltageRatio(double a, double b, double c)
{
	SetCellDivider(a, b, c, 0.0, 0.0, 0.0);
}

void LipoSingleCellMonitor::SetVoltageRatio(double a, double b, double c, double d)
{
	SetCellDivider(a, b, c, d, 0.0, 0.0);
}

void LipoSingleCellMonitor::SetVoltageRatio(double a, double b, double c, double d, double e)
{
	SetCellDivider(a, b, c, d, e, 0.0);
}

void LipoSingleCellMonitor::SetVoltageRatio(double a, double b, double c, double d, double e, double f)
{
	SetCellDivider(a, b, c, d, e, f);
}

void LipoSingleCellMonitor::SetCustomPins(int32_t a)
{
	SetPins(a, 0, 0, 0, 0, 0);
}

void LipoSingleCellMonitor::SetCustomPins(int32_t a, int32_t b)
{
	SetPins(a, b, 0, 0, 0, 0);
}

void LipoSingleCellMonitor::SetCustomPins(int32_t a, int32_t b, int32_t c)
{
	SetPins(a, b, c, 0, 0, 0);
}

void LipoSingleCellMonitor::SetCustomPins(int32_t a, int32_t b, int32_t c, int32_t d)
{
	SetPins(a, b, c, d, 0, 0);
}

void LipoSingleCellMonitor::SetCustomPins(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e)
{
	SetPins(a, b, c, d, e, 0);
}

void LipoSingleCellMonitor::SetCustomPins(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f)
{
	SetPins(a, b, c, d, e, f);
}

#pragma region Private Members
void LipoSingleCellMonitor::SampleVoltage()
{
	int zeroCellCount = 0;
	uint16_t aread[_MAXCELLS];
	double cumVoltage = 0;

	for (int i = 0; i < _MAXCELLS; i++)
	{
		aread[i] = analogRead(_pins[i]);
		if (aread[i] < _analogreadMinThreshold)
		{
			zeroCellCount++;
			aread[i] = 0;
		}

		//Smooth in a FIR filter
		float voltage = aread[i] / _individualCellDivisor[i];		
		_cellVoltages[i].SetValue(voltage);
		cumVoltage += voltage;
	}
	
	_totalVoltage.SetValue(cumVoltage);
	_cellsInUse = _MAXCELLS - zeroCellCount;

	if(_debug)
	{
		debugSerial.printf("%d\tCell| Raw  |  V     | Vsmooth\r\n", millis());;
		debugSerial.printf("%d\t----|------|--------|---------\r\n", millis());
		for (size_t j = 0; j < _MAXCELLS; j++)
		{
			debugSerial.printf("%d\t %d  | %4d | %5.3f | %5.3f\r\n", 
				millis(), j+1, aread[j], _cellVoltages[j].GetValue(), _cellVoltages[j].GetSmoothedValue());
		}
		debugSerial.printf("%d\tTotal: %5.2f / %5.2f v, Cells in use: %d\r\n", millis(),
			cumVoltage, _totalVoltage.GetSmoothedValue(), _cellsInUse);
	}

}

void LipoSingleCellMonitor::Initialize(uint8_t expectedCells, uint8_t analogReadReso)
{
	_debug = false;
	_cellsInUse = expectedCells;
	_expectedCells = expectedCells;

	analogReadResolution(analogReadReso);
	analogReference(DEFAULT);

	_pins = new int32_t[_MAXCELLS];
	
	_totalVoltage = BufferedSmoothValue<float>(Coefficients::Hamming, 21);
	for (int i = 0; i < _MAXCELLS; i++)
	{
		_pins[i] = i;	
		_cellVoltages[i] = BufferedSmoothValue<float>(Coefficients::Hamming, 21);
	}
}

void LipoSingleCellMonitor::SetCellDivider(double h1, double h2, double h3, double h4, double h5, double h6)
{
	double* ptrs[6] = { &h1, &h2, &h3, &h4, &h5, &h6 };
	uint16_t resBits = (2 ^ _resolutionBits) - 1;

	//Convert the ratios into divsors to simplify the per-reading processing
	for (size_t i = 0; i < 6; i++)
	{
		if (*ptrs[i] > 0.0)
		{			
			_individualCellDivisor[i] = *ptrs[i] / _referenceVoltage * resBits;
			if(_debug)
			{
				debugSerial.printf("%d\tCell Divisor %d: %3.5f", millis(), i, _individualCellDivisor[i]);
			}			
		}
	}

}

void LipoSingleCellMonitor::SetPins(int32_t p1, int32_t p2, int32_t p3, int32_t p4, int32_t p5, int32_t p6)
{
	_pins[0] = p1;
	if (_MAXCELLS > 1) _pins[1] = p2;
	if (_MAXCELLS > 2) _pins[2] = p3;
	if (_MAXCELLS > 3) _pins[3] = p4;
	if (_MAXCELLS > 4) _pins[4] = p5;
	if (_MAXCELLS > 5) _pins[5] = p6;
}

#pragma endregion
