#include "JsonHelper.h"

void ConvertStructToPacketJson(Packet packet, char *JsonResult){
    
    char * j1939Format="[{\"T\":\"%s\",\"DS\":\"%s\",\"TS\":\"%s\",\"P\":\"%s\",\"DT\":\"%s\",\"DT_UDS\":\"%s\",\"VB\":%f,\"DE\":\"%s\",\"TE\":\"%s\",\"E\":%d,\"DC3\":%d}]";
    // printf("%s\tstrlen=%d\n",packet.DT,strlen(packet.DT)+strlen(packet.P)+4+strlen(packet.DE)+strlen(packet.DS)+strlen(packet.DT_UDS)+4+strlen(packet.T)+1+strlen(packet.TE)+strlen(packet.TS)+packet.VB);
    // printf("%s",packet.P);
    // printf("%d",packet.DC3);
    // printf("%s",packet.DE);
    // printf("%s",packet.DS);
    // printf("%s",packet.DT_UDS);
    // printf("%d",packet.E);
    // printf("%s",packet.T);
    // printf("%s",packet.TE);
    // printf("%s",packet.TS);
    // printf("%f",packet.VB);
    sprintf(JsonResult,j1939Format, 
    packet.T, packet.DS, packet.TS,
    packet.P,packet.DT,packet.DT_UDS,
    packet.VB,packet.DE,packet.TE,packet.E,packet.DC3); 
}