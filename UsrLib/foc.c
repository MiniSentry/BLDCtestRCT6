#include "foc.h"
#include "motorDrv.h"
#include "speedCalc.h"
#include <math.h>   //replace with lookup table later

#define _SQRT3  1.73205080757f
#define _PI_3   1.0471975512f

void setPhaseVoltage(runStateStruct* runState)
{
    /*
    * SVPWM implementation according to https://docs.simplefoc.com/foc_theory
    */
    float Uq = 0;       //target voltage
    float Upsu = 0;     //PSU voltage
    float angleEl = 0;  // TODO:add angle calculation implementation to speedCalc.c
                        // do normalize between 0 and 2pi
    // add these above to runState
    uint8_t sector = floor(angleEl / _PI_3) + 1;
    float T1 = _SQRT3*sin(sector*_PI_3 - angleEl) * Uq/Upsu;
    float T2 = _SQRT3*sin(angleEl - (sector - 1)*_PI_3) * Uq/Upsu;
    float T0 = 1 - T1 - T2;

    float Ta, Tb, Tc;
    switch(sector)
    {
        case 1:
            Ta = T1 + T2 + T0/2;
            Tb = T2 + T0/2;
            Tc = T0/2;
            break;
        case 2:
            Ta = T1 +  T0/2;
            Tb = T1 + T2 + T0/2;
            Tc = T0/2;
            break;
        case 3:
            Ta = T0/2;
            Tb = T1 + T2 + T0/2;
            Tc = T2 + T0/2;
            break;
        case 4:
            Ta = T0/2;
            Tb = T1+ T0/2;
            Tc = T1 + T2 + T0/2;
            break;
        case 5:
            Ta = T2 + T0/2;
            Tb = T0/2;
            Tc = T1 + T2 + T0/2;
            break;
        case 6:
            Ta = T1 + T2 + T0/2;
            Tb = T0/2;
            Tc = T1 + T0/2;
            break;
        default:
            // possible error state
            Ta = 0;
            Tb = 0;
            Tc = 0;
    }
    float Ua = Ta*Upsu;
    float Ub = Tb*Upsu;
    float Uc = Tc*Upsu;

}