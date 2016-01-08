#include "LedColorHelpers.h"


#pragma region Color Functions

///<summary>
///  Creates a sine pulse.  Specify a divisor to get mulitiple sin wave per period
///</summary>
void LedColorHelpers::CreateSinePulse(uint8_t values, uint8_t* wave, int divisor)
{
	for (size_t i = 0; i < values; i++)
	{
		wave[i] = (uint8_t)(255 * (float)(0.5 * (cos(PI * i / divisor) + 1)));
	}
}

///<summary>
///  Creates a linear intensity ramp / sawtooth pattern
///</summary>
void LedColorHelpers::CreateLinearRamp(uint8_t values, uint8_t* wave, int period)
{
	for (size_t i = 0; i < values; i++)
	{
		wave[i] = (uint8_t)(255 * (i % period) / (float)(period - 1));
	}
}

///<summary>
///  Create an HSV color scale
///</summary>
void LedColorHelpers::CreateRainbow(uint8_t values, int* matrix)
{
	CRGB color;
	for (size_t i = 0; i < values; i++)
	{
		uint8_t h = (uint8_t)(255 * i / (float)values);	//much easier to use HSV color model for this	
		matrix[i] = GetColorCode(color.setHSV(h, 255, 255));
	}
}

///<summary>
///  Creates either a red-to-blue, or blue-to-red JPL (a.k.a., "jet") color scale.  This is a default color ramp in MatLab
///</summary>
void LedColorHelpers::CreateJPLRedBlue(uint8_t values, int* matrix, uint8_t redBlue)
{
	int i = 0;
	int j = redBlue == 1 ? 0 : values - 1;
	int increment = (redBlue == 1 ? 1 : -1);

	while (i < values)
	{
		double x = (double)j / (values - 1);

		//R value
		double r;
		if ((x >= 3.0 / 8.0) && (x < 5.0 / 8.0))
		{
			r = (4.0 * x - 3.0 / 2.0);
		}
		else if ((x >= 5.0 / 8.0) && (x < 7.0 / 8.0))
		{
			r = 1;
		}
		else if (x >= 7.0 / 8.0)
		{
			r = -4.0 * x + 9.0 / 2.0;
		}
		else
		{
			r = 0;
		}

		//G value
		double g;
		if ((x >= 1.0 / 8.0) && (x < 3.0 / 8.0))
		{
			g = (4.0 * x - 1.0 / 2.0);
		}
		else if ((x >= 3.0 / 8.0) && (x < 5.0 / 8.0))
		{
			g = 1.0;
		}
		else if ((x >= 5.0 / 8.0) && (x < 7.0 / 8.0))
		{
			g = -4.0 * x + 7.0 / 2.0;
		}
		else
		{
			g = 0.0;
		}

		//B value
		double b;
		if ((x < 1.0 / 8.0))
		{
			b = (4.0 * x + 1.0 / 2.0);
		}
		else if ((x >= 1.0 / 8.0) && (x < 3.0 / 8.0))
		{
			b = 1.0;
		}
		else if ((x >= 3.0 / 8.0) && (x < 5.0 / 8.0))
		{
			b = -4.0 * x + 5.0 / 2.0;
		}
		else
		{
			b = 0.0;
		}

		//Set the 24-bit color
		matrix[j] = (int)(r * 255.0) << 16 | (int)(g * 255.0) << 8 | (int)(b * 255.0);

		j = j + increment;
		i++;
	}
}


#pragma endregion
