#include "timehelper.h"

void SetDateTimeOfPacket(Packet * packet, enum PacketPosition packetPos){
    
    time_t t;
    time(&t);
    strftime(Date,9, "%d%m%Y\0",localtime(&t));
    strftime(Time,9, "%H%M%S\0",localtime(&t));
    switch(packetPos){
        case StartOfPacket:
            (*packet).DS=Date;
            (*packet).TS=Time;
            break;
        case EndOfPacket:
            (*packet).DE=Date;
            (*packet).TE=Time;
            break;
    }
}