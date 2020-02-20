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
    Packet packet = {"0", "DS", "TS", "P", "DT", "DAA", 0, "DE", "TE", 0, 0};
    char Json[820] = {0};
    DWORD BytesWritten = 0; // No of bytes written to the port

    if (!InitializeSerialPort(PORT_NO))
    {
        system("pause");
        return 0;
    }
    InitiliazeELMForJ1939();
    while (1)
    {
        // Set Start Date and Time
        SetDateTimeOfPacket(&packet, StartOfPacket);

        //Turn on the Header
        SetHeaderOnOff(0);

        //Get Parameters
        SetJ1939Params(&packet);
        //Set Battery volatge
        SetBatteryVoltage(&packet);
        // Set End Date and Time
        SetDateTimeOfPacket(&packet, EndOfPacket);
        memset(Json,0,820);
        ConvertStructToPacketJson(packet, Json);
        printf("%s\n", Json);
    }
    return 0;
}
