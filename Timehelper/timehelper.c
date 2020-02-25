#include "timehelper.h"
#include <string.h>

void SetDateTimeOfPacket(Packet * packet, enum PacketPosition packetPos){
    
    time_t t;
    time(&t);
    
    switch(packetPos){
        case StartOfPacket:
            strftime((*packet).DS,9, "%d%m%Y\0",localtime(&t));
            strftime((*packet).TS,9, "%H%M%S\0",localtime(&t));
            break;
        case EndOfPacket:
            strftime((*packet).DE,9, "%d%m%Y\0",localtime(&t));
            strftime((*packet).TE,9, "%H%M%S\0",localtime(&t));
            break;
    }
}