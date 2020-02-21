#ifndef Model_H_
#define Model_H_

#include <stdio.h>

BOOL logg=FALSE;

typedef struct
{
    char * T ;
    char * DS ;
    char * TS ;
    char * P ;
    char * DT ;
    char * DT_UDS ;
    double VB ;
    char * DE ;
    char * TE ;
    int E ;
    int DC3 ;
}Packet;


    char Json[820] = {0};

enum HFreqParams{
    F004,
    F003,
    FEF2,
    FEF6
};

enum LFreqParams{
    FEEF,
    FEE9,
    FEE5,
    FEBF,
    FECA,
    FEEE
};

enum PacketPosition{
    StartOfPacket,
    EndOfPacket
};

#endif //