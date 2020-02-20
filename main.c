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
    BOOL Status;
    char SerialRxBuffer[SERIAL_BUFF_SIZE] = {0}; //Buffer to receive data
    char SerialTxBuffer[SERIAL_BUFF_SIZE] = {0}; //Buffer to send data
    char Json[256]={0};
    DWORD BytesWritten = 0;                    // No of bytes written to the port

    char *pszPortName = PORT_NO; //com port id

    if (!InitializeSerialPort(pszPortName))
    {
        system("pause");
        return 0;
    }
    InitiliazeELMForJ1939();

    SetDateTimeOfPacket(&packet,StartOfPacket);
    SetDateTimeOfPacket(&packet,EndOfPacket);
    ConvertStructToPacketJson(packet, Json);
    return 0;
}
