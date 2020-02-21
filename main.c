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
        SetHeaderOn(0);

        //Set Parameters
        SetJ1939Params(&packet);
        //Set Fault codes
        SetHeaderOn(1);
        logg=FALSE;
        SetTroubleCodes(&packet);
        logg=FALSE;
        //Set Battery volatge
        SetBatteryVoltage(&packet);
        // Set End Date and Time
        SetDateTimeOfPacket(&packet, EndOfPacket);
        // printf("\npacket.p=%spacket.dt=%s\n",packet.P,packet.DT);
        memset(Json,0,820);
        // printf("%s\n\n", Json);
        ConvertStructToPacketJson(packet, Json);
        printf("%s\n\n", Json);
    }
    return 0;
}
