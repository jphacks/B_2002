#include <xc.h>
#include "DPS310.h"
#include "i2c.h"

const char pressure_address = 0x77;

signed long _c0, _c1, _c00, _c10, _c01, _c11, _c20, _c21, _c30;
double c0, c1, c00, c10, c01, c11, c20, c21, c30;

void DPS310_Init()
{
    char _cc[18];
    signed long cc[18];
    char i;

    I2cWriteRegister(pressure_address, PRS_CFG, 0x44);
    I2cWriteRegister(pressure_address, TMP_CFG, 0x44);
    I2cWriteRegister(pressure_address, MEAS_CFG, 0x07);
    I2cWriteRegister(pressure_address, CFG_REG, 0x0c);

    I2cReadMultiRegisters(pressure_address, COEF, _cc, 18);
    for (i = 0; i < 18; i++)
    {
        cc[i] = (signed long)_cc[i];
    }
//    _c0 = cc[0] << 4 | (cc[1] >> 4);
//    if (_c0 & ((long)1 << 11))
//        _c0 -= (long)1 << 12;
//    _c1 = (cc[1] & 0x0f) << 8 | cc[2];
//    if (_c1 & ((long)1 << 11))
//        _c1 -= (long)1 << 12;
//    _c00 = cc[3] << 12 | cc[4] << 4 | cc[5] >> 4;
//    if (_c00 & ((long)1 << 19))
//        _c0 -= (long)1 << 20;
//    _c10 = (cc[5] & 0x0f) << 16 | cc[6] << 8 | cc[7];
//    if (_c10 & ((long)1 << 19))
//        _c10 -= (long)1 << 20;
//    _c01 = cc[8] << 8 | cc[9];
//    if (_c01 & ((long)1 << 15))
//        _c01 -= (long)1 << 16;
//    _c11 = cc[10] << 8 | cc[11];
//    if (_c11 & (1 << 15))
//        _c11 -= (long)1 << 16;
//    _c20 = cc[12] << 8 | cc[13];
//    if (_c20 & (1 << 15))
//        _c20 -= (long)1 << 16;
//    _c21 = cc[14] << 8 | cc[15];
//    if (_c21 & (1 << 15))
//        _c21 -= (long)1 << 16;
//    _c30 = cc[16] << 8 | cc[17];
//    if (_c30 & (1 << 15))
//        _c30 -= (long)1 << 16;

    
    _c0 = cc[0] << 4 | (cc[1] >> 4);
    if (cc[0] & 0x80)
        _c0 |= 0xfffff000;

    _c1 = (cc[1] & 0x0f) << 8 | cc[2];
    if (cc[1] & 0x08)
        _c1 |= 0xfffff000;

    _c00 = cc[3] << 12 | cc[4] << 4 | cc[5] >> 4;
    if (cc[3] & 0x80)
        _c00 |= 0xfff00000;

    _c10 = (cc[5] & 0x0f) << 16 | cc[6] << 8 | cc[7];
    if (cc[5] & 0x08)
        _c10 |= 0xfff00000;

    _c01 = cc[8] << 8 | cc[9];
    if (cc[8] & 0x80)
        _c01 |= 0xffff0000;

    _c11 = cc[10] << 8 | cc[11];
    if (cc[10] & 0x80)
        _c11 |= 0xffff0000;

    _c20 = cc[12] << 8 | cc[13];
    if (cc[12] & 0x80)
        _c20 |= 0xffff0000;

    _c21 = cc[14] << 8 | cc[15];
    if (cc[14] & 0x80)
        _c21 |= 0xffff0000;

    _c30 = cc[16] << 8 | cc[17];
    if (cc[16] & 0x80)
        _c30 |= 0xffff0000;
    
    
    c0 = (double)_c0;
    c1 = (double)_c1;
    c00 = (double)_c00;
    c10 = (double)_c10;
    c01 = (double)_c01;
    c11 = (double)_c11;
    c20 = (double)_c20;
    c21 = (double)_c21;
}

signed long DPS310_PRS()
{
    char v[3];
    signed long value;
    I2cReadMultiRegisters(pressure_address, PRS_B2, v, 3);
    value = (signed long)v[0] << 16 | (signed long)v[1] << 8 | (signed long)v[2];
    if (v[0] & 0x80)
        value |= 0xff000000;
    return value;
}

signed long DPS310_TMP()
{
    char v[3];
    signed long value;
    I2cReadMultiRegisters(pressure_address, TMP_B2, v, 3);
    value = (signed long)v[0] << 16 | (signed long)v[1] << 8 | (signed long)v[2];
    if (v[0] & 0x80)
        value |= 0xff000000;
    return value;
}

double DPS310_Pressure()
{
    signed short long p, t;
    double kt = 253952;
    double kp = 253952;
    double p_raw, t_raw, p_sc, t_sc, pressure;
    unsigned long value;

    p = DPS310_PRS();
    t = DPS310_TMP();
    p_raw = (double)p;
    t_raw = (double)t;
    t_sc = t_raw / kt;
    p_sc = p_raw / kp;
    pressure = c00 + p_sc*(c10+p_sc*(c20+p_sc*c30)) + t_sc*c01 + t_sc*p_sc*(c11+p_sc*c21);
    return pressure;
}
