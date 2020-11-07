#include "COBS.h"
// COBS
// length は変換前のデータ長。実際に送受信する+2されたデータ長ではない。

void CobsEncode(char data[], char length, char send_data[])
{
    char last_zero = 0;
    for (char i = 0; i < length; i++)
    {
        if (data[i] == 0)
        {
            send_data[last_zero] = i - last_zero + 1;
            last_zero = i + 1;
        }
        else
        {
            send_data[i + 1] = data[i];
        }
    }
    send_data[last_zero] = length - last_zero + 1;
    send_data[length + 1] = 0;
}

void CobsDecode(char receive_data[], char length, char data[])
{
    char next_zero;
    next_zero = receive_data[0] - 1;
    for (int i = 0; i < length; i++)
    {
        data[i] = receive_data[i + 1];
    }
    while (next_zero < length)
    {
        data[next_zero] = 0;
        next_zero += receive_data[next_zero + 1];
    }
}
