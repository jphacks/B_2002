#ifndef DPS310_H
#define	DPS310_H

// DPS310

#define PRS_B2 0x00
#define PRS_B1 0x01
#define PRS_B0 0x02
#define TMP_B2 0x03
#define TMP_B1 0x04
#define TMP_B0 0x05
#define PRS_CFG 0x06
#define TMP_CFG 0x07
#define MEAS_CFG 0x08
#define CFG_REG 0x09
#define INT_STS 0x0a
#define FIFO_STS 0x0b
// #define RESET 0x0c
#define ProductIO 0x0d
#define COEF 0x10
// c0 - c30 (0x10 - 0x21)
#define COEF_SRCE 0x28

#define ProductID_RSP 0x10

extern void DPS310_Init();
extern signed long DPS310_PRS();
extern signed long DPS310_TMP();
extern double DPS310_Pressure();

extern long _c0, _c1, _c00, _c10, _c01, _c11, _c20, _c21, _c30;

#endif