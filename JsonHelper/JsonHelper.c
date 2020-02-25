#include "JsonHelper.h"

void ConvertStructToPacketJson(Packet packet, char *JsonResult){
    
    char * j1939Format="[{\"T\":\"%s\",\"DS\":\"%s\",\"TS\":\"%s\",\"P\":\"%s\",\"DT\":\"%s\",\"DT_UDS\":\"%s\",\"VB\":%f,\"DE\":\"%s\",\"TE\":\"%s\",\"E\":%d,\"DC3\":%d}]";
    sprintf(JsonResult,j1939Format, 
    packet.T, packet.DS, packet.TS,
    packet.P,packet.DT,packet.DT_UDS,
    packet.VB,packet.DE,packet.TE,packet.E,packet.DC3); 
}