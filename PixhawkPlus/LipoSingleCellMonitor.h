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
 * Class definition of Wolke lipo-single-cell-monitor
 * *******************************************
 * This will monitor 1 - 8 cells of your lipo.
 * It display the low cell, the high cell and the difference between this cells
 * this will give you a quick overview about your lipo _status and if the lipo is well balanced
 *
 * detailed informations and schematics here
 *
 */

#ifndef _LSCM_H_
#define _LSCM_H_

#include "Arduino.h"
#include "MAVLinkData.h"

class LipoSingleCellMonitor {

public:
    
    /**
    \ingroup MavLink_FrSkySPort
    \brief Overloaded Constructor of LSCM Object
    \descriptions construct LSCM minimal with cells count.
    \example LSCM lscm(3, 13, 0.99);
    \args
    \cells how many cells connected
    \analogReadResolution on teensy3.x 13 bit as default    
    */	
	LipoSingleCellMonitor(uint8_t cells, uint8_t analogReadResolution, MAVLinkData *data);
    
    /**
    \brief main-process-call of LSCM. 
    \descriptions: must called in main loop.
    */
    void Process();
    
    /**
    \brief returns disered Cell voltage 0-n based on lipocells. 
    \descriptions returned an uint32_t of called lipocell.
    */   
    uint32_t GetCellVoltageAsUint32_T(uint8_t cell);

    /**
    \brief returns Cell voltage sum from al Cells. 
    \descriptions returned an int32_t Cell sum.
    */ 
    int32_t GetAllLipoCells();
    
    /**
    \brief returns quantity of really connected cells. 
    \descriptions returned an uint8_t of real connected cells.
    */ 
    uint8_t GetCellsInUse();
    
    /**
    \brief quantity of cells at init. 
    \descriptions returned an uint8_t of constructer maxcells.
    */
    uint8_t GetMaxCells();
    
    /**
    \brief debug. 
    \descriptions enable disable debug.
    */
    void SetDebug(bool debug);

    /**
    \brief Overloaded Function of LSCM setCustomCelldivider
    */
    void SetCustomCellDivider(double a);
    void SetCustomCellDivider(double a, double b);
    void SetCustomCellDivider(double a, double b, double c);
    void SetCustomCellDivider(double a, double b, double c, double d);
    void SetCustomCellDivider(double a, double b, double c, double d, double e);
    void SetCustomCellDivider(double a, double b, double c, double d, double e, double f);       

    void SetCustomPins(int32_t a);
    void SetCustomPins(int32_t a, int32_t b);
    void SetCustomPins(int32_t a, int32_t b, int32_t c);   
    void SetCustomPins(int32_t a, int32_t b, int32_t c, int32_t d);
    void SetCustomPins(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e);
    void SetCustomPins(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
    
private:
    
	MAVLinkData *_messages;

    /**
    \descriptions init LSCM object variables and arrays. automatically called by constructor
    */
    void Initialize(uint8_t cells, uint8_t analogReadReso, float smoothness);

    /**
    \descriptions set Celldivider
    */
    void SetCellDivider(double a, double b, double c, double d, double e, double f);    
    
    void SetPins(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f);
    
    /**
    \brief main Array which holds software deviders. 
    \descriptions this 13bit default divider are used if LSCM object will init. do not change this!
    */
    double _LIPOCELL_1TO8[13] =
    {
      1913.528953519,
      933.688035297,
      622.955076603,
      473.787040052,
      373.105567418,
      317.423580786      
    };
    
    bool _debug;
    double *_individualcelldivider;
    uint8_t _analogread_threshold;         // threshold for connected zelldetection in mV
    uint8_t _cells_in_use;
    int32_t *_zelle;
    double *_cell;
    int32_t _alllipocells;
    float _lp_filter_val; // this determines smoothness  - .0001 is max  0.99 is off (no smoothing)
    double *_smoothedVal; // this holds the last loop value
    uint8_t _maxcells;
    int32_t *_pins; //pin assignments
    
};
#endif /* _LSCM_H_ */
 
