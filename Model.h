#ifndef Model_H_
#define Model_H_

#include <stdio.h>


typedef struct
{
    char * T ;
    char DS[10] ;
    char TS[10] ;
    char * P ;
    char * DT ;
    char * DT_UDS ;
    double VB ;
    char DE[10] ;
    char TE[10] ;
    int E ;
    int DC3 ;
}Packet;

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