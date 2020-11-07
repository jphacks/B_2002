#ifndef COBS_H
#define	COBS_H

extern void CobsEncode(char data[], char length, char send_data[]);
extern void CobsDecode(char receive_data[], char length, char data[]);

#endif