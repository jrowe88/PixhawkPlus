/**
 *
 * Class LSCM, part of MavLink_FrSkySPort
 *
 * Copyright (C) 2015 Michael Wolkstein
 * https://github.com/Clooney82/MavLink_FrSkySPort
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 *
 *
 * Class of Wolke lipo-single-cell-monitor
 * *******************************************
 * This will monitor 1 - 8 cells of your lipo.
 * It display the low cell, the high cell and the difference between this cells
 * this will give you a quick overview about your lipo _status and if the lipo is well balanced
 *
 * detailed informations and schematics here
 *
 */

#include "LipoSingleCellMonitor.h"

// ----- Initialization and Default Values -----
LipoSingleCellMonitor::LipoSingleCellMonitor(uint8_t cells, uint8_t analogReadReso, MAVLinkData* data)
{
	_messages = data;
	Initialize(cells, analogReadReso, 0.99);
}

// public functions
void LipoSingleCellMonitor::Process()
{
	double aread[_maxcells + 1];
	for (int i = 0; i < _maxcells; i++)
	{
		aread[i] = analogRead(_pins[i]);
		if (aread[i] < _analogread_threshold)
		{
			_cells_in_use = i;
			break;
		}
		else
		{
			_cells_in_use = _maxcells;
		}
		// USE Low Pass filter
		_smoothedVal[i] = (aread[i] * (1 - _lp_filter_val)) + (_smoothedVal[i] * _lp_filter_val);
		aread[i] = round(_smoothedVal[i])
		;
		_cell[i] = double(aread[i] / _individualcelldivider[i]) * 1000;
		if (i == 0)
		{
			_zelle[i] = round(_cell[i])
			;
		}
		else
		{
			_zelle[i] = round(_cell[i] - _cell[i - 1])
			;
		}
	}
	_alllipocells = _cell[_cells_in_use - 1];

	if (_debug)
	{
		//TODO
	}
}

uint32_t LipoSingleCellMonitor::GetCellVoltageAsUint32_T(uint8_t cell)
{
	return _zelle[cell];
}

uint8_t LipoSingleCellMonitor::GetCellsInUse()
{
	return _cells_in_use;
}

int32_t LipoSingleCellMonitor::GetAllLipoCells()
{
	return _alllipocells;
}


uint8_t LipoSingleCellMonitor::GetMaxCells()
{
	return _maxcells;
}

void LipoSingleCellMonitor::SetDebug(bool debug)
{
	_debug = debug;
}

void LipoSingleCellMonitor::SetCustomCellDivider(double a)
{
	SetCellDivider(a, 0.0, 0.0, 0.0, 0.0, 0.0);
}

void LipoSingleCellMonitor::SetCustomCellDivider(double a, double b)
{
	SetCellDivider(a, b, 0.0, 0.0, 0.0, 0.0);
}

void LipoSingleCellMonitor::SetCustomCellDivider(double a, double b, double c)
{
	SetCellDivider(a, b, c, 0.0, 0.0, 0.0);
}

void LipoSingleCellMonitor::SetCustomCellDivider(double a, double b, double c, double d)
{
	SetCellDivider(a, b, c, d, 0.0, 0.0);
}

void LipoSingleCellMonitor::SetCustomCellDivider(double a, double b, double c, double d, double e)
{
	SetCellDivider(a, b, c, d, e, 0.0);
}

void LipoSingleCellMonitor::SetCustomCellDivider(double a, double b, double c, double d, double e, double f)
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


// private functions

void LipoSingleCellMonitor::Initialize(uint8_t cells, uint8_t analogReadReso, float smoothness)
{
	_debug = false;
	_maxcells = cells;
	_cells_in_use = cells;
	_lp_filter_val = smoothness;
	_analogread_threshold = 100;
	_alllipocells = 0;
	analogReadResolution(analogReadReso);
	analogReference(DEFAULT);

	_individualcelldivider = new double[cells + 1];
	_zelle = new int32_t[cells + 1];
	_cell = new double[cells + 1];
	_smoothedVal = new double[cells + 1];

	_pins = new int32_t[cells];

	for (int i = 0; i < cells; i++)
	{
		_zelle[i] = 0;
		_cell[i] = 0.0;
		_pins[i] = i;
		_individualcelldivider[i] = _LIPOCELL_1TO8[i];
		_smoothedVal[i] = 900.0;
	}
}

void LipoSingleCellMonitor::SetCellDivider(double a, double b, double c, double d, double e, double f)
{
	if (a == 0.0)
		return;
	_individualcelldivider[0] = a;

	if (b == 0.0)
		return;
	_individualcelldivider[1] = b;

	if (c == 0.0)
		return;
	_individualcelldivider[2] = c;

	if (d == 0.0)
		return;
	_individualcelldivider[3] = d;

	if (e == 0.0)
		return;
	_individualcelldivider[4] = e;

	if (f == 0.0)
		return;
	_individualcelldivider[5] = f;
}

void LipoSingleCellMonitor::SetPins(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f)
{
	_pins[0] = a;
	if (_maxcells > 1) _pins[1] = b;
	if (_maxcells > 2) _pins[2] = c;
	if (_maxcells > 3) _pins[3] = d;
	if (_maxcells > 4) _pins[4] = e;
	if (_maxcells > 5) _pins[5] = f;
}

