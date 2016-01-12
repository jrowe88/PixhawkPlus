#pragma region Copyright
//This file is part of Pixhawk Plus
//
//Jim Rowe Copyright (c) 2015
//https://github.com/jrowe88/PixhawkPlus
//
//  Original concept and big props to Rolf Blomgren - http://diydrones.com/forum/topics/amp-to-frsky-x8r-sport-converter
//  Adapted and modified from numerous other contributors:
//     Michael Wolkstein - https://github.com/Clooney82/MavLink_FrSkySPort/   
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

#ifndef _LIPOSINGLECELLMONITOR_H_
#define _LIPOSINGLECELLMONITOR_H_

#include "Arduino.h"
#include "MAVLinkData.h"

#define NOPIN -1

/// <summary>
/// Calculates individual cell voltage using data from balance plug inputs.
/// supports up to 6S batteries.
/// 
/// Assumes input from hardware that scales the voltage to within the range of the
/// analog monitors using a voltage divider pair of resistors.  Specify the scaling factors in terms of cell divider
/// values based on the R1 and R2 resistor values.
/// https://en.wikipedia.org/wiki/Voltage_divider
/// </summary>
class LipoSingleCellMonitor {

public:
    
    /// <summary>
    /// Standard constructor
    /// </summary>
    /// <param name="cells">Number of cells in the battery</param>
    /// <param name="analogReadResolution">Analog resolution.  Default is 13</param>
    /// <param name="data">MAVLink data packats</param>
	LipoSingleCellMonitor(uint8_t cells, uint8_t analogReadResolution, MAVLinkData *data, float analogRefVoltage = 3.32);
    
    /// <summary>
    /// Process the cell voltage.  Timing is subject to _minSamplePeriod
    /// </summary>
    void Process();
    
	/// <summary>
	/// Individual cell voltage in volts
	/// </summary>
	/// <param name="cell"></param>
	/// <returns></returns>
    float GetCellVoltage(uint8_t cell);

    /// <summary>
    /// Total voltage of all cells
    /// </summary>
    /// <returns></returns>
    float GetAllLipoCells();
    
    /// <summary>
    /// Cells that are connected
    /// </summary>
    /// <returns></returns>
    uint8_t GetCellsInUse();
    
    /// <summary>
    /// Number of cells specified
    /// </summary>
    /// <returns></returns>
    uint8_t GetMaxCells();
    
    /// <summary>
    /// Enable serial debugging.  By default it is not enabled; use this method to toggle.
    /// </summary>
    /// <param name="debug"></param>
    void SetDebug(bool debug);

    /// <summary>
    /// Set the voltage ratio/transfer function values.  Values are calculated as follows:
    ///   Vin = Vout * H
	///   H = R2 / (R2 + R1)
	/// </summary>
	/// <param name="a"></param>    
    void SetVoltageRatio(double H1, double H2=0, double H3=0, double H4=0, double H5=0, double H6=0);       

	/// <summary>
	/// Set the pins used for analog reading.  Can be any pins from your board that are not used
	/// by other modules and can read analog signals.
	/// </summary>
	/// <param name="a"></param>    
    void SetCustomPins(int32_t a, int32_t b = NOPIN, int32_t c = NOPIN, int32_t d = NOPIN, int32_t e = NOPIN, int32_t f = NOPIN);
    
private:
	static const uint8_t _MAXCELLS = 6;
	const uint8_t _analogreadMinThreshold = 100;         // threshold for connected zelldetection in mV
	const float _maxVoltsPerCell = 4.2;
	const uint32_t _minSamplePeriod = 50; //min ms between samples

	float _referenceVoltage = 3.32; //as measured from Teensy3.2
	uint32_t _sampleLimitTimer = 20;
	uint8_t _resolutionBits = 13;
	MAVLinkData *_messages;      
	int32_t *_pins; //pin assignments
    double _individualCellDivisor[_MAXCELLS] = { 1899.14213, 956.02786, 632.08454, 482.51706, 380.50860, 313.29126 };  //default values
    		
	bool _debug;    
	uint8_t _expectedCells = _MAXCELLS;
    uint8_t _cellsInUse;    
    BufferedSmoothValue<float> _cellVoltages[_MAXCELLS];
	BufferedSmoothValue<float> _totalVoltage;

    //Methods
	void Initialize(uint8_t cells, uint8_t analogReadReso);
	void SetCellDivider(double a, double b, double c, double d, double e, double f);
	void SetPins(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
	void SampleVoltage();
};
#endif
 

