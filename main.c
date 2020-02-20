#include <Windows.h>
#include <stdio.h>
#include "SerialComm.h"
#include "ELMHelper.h"
#include <time.h>
#include "Model.h"
#include "timehelper.h"
#include "JsonHelper.h"

// extern BOOL InitiliazeELMForJ1939();


int main()
{
    Packet packet={"0","DS","TS","P","DT","DT_UDS",0,"DE","TE",0,0};
    char Json[256]={0};
    DWORD BytesWritten = 0;                    // No of bytes written to the port

    if (!InitializeSerialPort(PORT_NO))
    {
        system("pause");
        return 0;
    }
    InitiliazeELMForJ1939();

    SetDateTimeOfPacket(&packet,StartOfPacket);
    SetHeaderOnOff(0);
    SetBatteryVoltage(&packet);
    SetDateTimeOfPacket(&packet,EndOfPacket);
    ConvertStructToPacketJson(packet, Json);
    printf("%s\n",Json);
    return 0;
}
