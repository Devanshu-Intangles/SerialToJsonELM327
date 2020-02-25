#include <Windows.h>
#include <stdio.h>
#include "SerialComm/SerialComm.h"
#include "ELMHelper/ELMHelper.h"
#include <time.h>
#include "Model.h"
#include "Timehelper/timehelper.h"
#include "JsonHelper/JsonHelper.h"

#define J1939PROTOCOL
// #define CAN15765PROTOCOL

// extern BOOL InitiliazeELMForJ1939();
Packet packet = {"0", "DS", "TS", "P", "DT", "DAA", 0, "DE", "TE", 0, 0};

int main()
{
    char Json[1921] = {0};
    DWORD BytesWritten = 0; // No of bytes written to the port

    #ifdef J1939PROTOCOL
    InitiliazeELMForJ1939();
    #endif
    #ifdef CAN15765PROTOCOL
    InitiliazeELMFor15765();
    #endif

    while (1)
    {
        // Set Start Date and Time
        SetDateTimeOfPacket(&packet, StartOfPacket);

        //Turn off Header
        SetHeaderOn(0);

        //Set Parameters and Fault codes
        #ifdef J1939PROTOCOL
        SetJ1939Params(&packet);
        SetTroubleCodes(&packet);
        #endif
        #ifdef CAN15765PROTOCOL
        Set15765Params(&packet);
        SetTroubleCodes15765(&packet);
        #endif
       
        //Set Battery volatge
        SetBatteryVoltage(&packet);
        
        // Set End Date and Time
        SetDateTimeOfPacket(&packet, EndOfPacket);

        ConvertStructToPacketJson(packet, Json);

        printf("%s\n\n", Json);
    }
    return 0;
}
