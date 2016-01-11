#pragma region Copyright
//This file is part of Pixhawk Plus
//This file is part of Pixhawk Plus
//
//Jim Rowe Copyright (c) 2015
//https://github.com/jrowe88/PixhawkPlus
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
#pragma once

#ifndef _COEFFICIENTS_h
#define _COEFFICIENTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/// <summary>
/// Coefficients to be used in a smoothing filter.  These were generated from Octave and are standard
///  signal processing coefficients.
/// </summary>
namespace Coefficients
{
	static double Hamming[21] = {
		0.00735294117647059,
		0.00942224264705882,
		0.0154275735294118,
		0.0247811580882353,
		0.0365672794117647,
		0.0496323529411765,
		0.0626974264705882,
		0.0744835477941177,
		0.0838371323529412,
		0.0898424632352941,
		0.0919117647058824,
		0.0898424632352941,
		0.0838371323529412,
		0.0744835477941177,
		0.0626974264705882,
		0.0496323529411765,
		0.0365672794117647,
		0.0247811580882353,
		0.0154275735294118,
		0.00942224264705882,
		0.00735294117647059
	};

	static double BlackmannHarris[21] =
	{
		8.36233623827886E-06,
		0.000249643611166752,
		0.00153058627614631,
		0.00546213865857311,
		0.0143567375984184,
		0.0303092876956417,
		0.0537823655164905,
		0.0823676182243404,
		0.110637889600548,
		0.131609235163445,
		0.139372270637981,
		0.131609235163445,
		0.110637889600548,
		0.0823676182243404,
		0.0537823655164905,
		0.0303092876956417,
		0.0143567375984184,
		0.00546213865857311,
		0.00153058627614631,
		0.000249643611166752,
		8.36233623827886E-06
	};

	static double Guassian[21] =
	{
		0.00133164299740864,
		0.00313114112299136,
		0.00672893866770491,
		0.0132158585027725,
		0.0237222808123298,
		0.0389162447020172,
		0.058347276287213,
		0.079950963316466,
		0.100124353807319,
		0.114595977189096,
		0.119870645189363,
		0.114595977189096,
		0.100124353807319,
		0.079950963316466,
		0.058347276287213,
		0.0389162447020172,
		0.0237222808123298,
		0.0132158585027725,
		0.00672893866770491,
		0.00313114112299136,
		0.00133164299740864
	};
}

#endif

