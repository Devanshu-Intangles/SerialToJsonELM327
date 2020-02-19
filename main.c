#include <Windows.h>
#include <stdio.h>
#include "SerialComm.h"
#include "ELMHelper.h"
// extern BOOL InitiliazeELMForJ1939();

int main()
{
    BOOL Status;                               // Status
    char SerialRxBuffer[SERIAL_BUFF_SIZE] = {0}; //Buffer to receive data
    char SerialTxBuffer[SERIAL_BUFF_SIZE] = {0}; //Buffer to send data
    DWORD BytesWritten = 0;                    // No of bytes written to the port

    char *pszPortName = PORT_NO; //com port id

    if (!InitializeSerialPort(pszPortName))
    {
        system("pause");
        return 0;
    }
    printf("Initialization done sucefully\n");
    InitiliazeELMForJ1939();
    // printf("Hello World!");
    while(TRUE)
    {
        memset(SerialTxBuffer,0,sizeof(SerialTxBuffer));
        printf("\n\nEnter your message: ");
        scanf("%s", SerialTxBuffer);
        // if (!WriteToSerialPort(SerialTxBuffer, &BytesWritten))
        // {
        //     system("pause");
        //     return 0;
        // }

        // if (!ReadFromSerialPort(SerialRxBuffer))
        // {
        //     system("pause");
        //     return 0;
        // }
        
    }
    return 0;
}
