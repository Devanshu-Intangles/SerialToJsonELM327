#include <Windows.h>
#include <stdio.h>
#include "SerialComm.h"

int main()
{
    HANDLE hComm;                              // Handle to the Serial port
    BOOL Status;                               // Status
    char SerialRxBuffer[SERIAL_BUFF_SIZE] = {0}; //Buffer to receive data
    char SerialTxBuffer[SERIAL_BUFF_SIZE] = {0}; //Buffer to send data
    DWORD BytesWritten = 0;                    // No of bytes written to the port

    char *pszPortName = PORT_NO; //com port id

    if (!InitializeSerialPort(&hComm, pszPortName))
    {
        system("pause");
        return 0;
    }
    // printf("Hello World!");
    while(TRUE)
    {
        memset(SerialTxBuffer,0,sizeof(SerialTxBuffer));
        printf("\n\nEnter your message: ");
        scanf("%s", SerialTxBuffer);
        if (!WriteToSerialPort(hComm, SerialTxBuffer, &BytesWritten))
        {
            system("pause");
            return 0;
        }

        if (!ReadFromSerialPort(hComm, SerialRxBuffer))
        {
            system("pause");
            return 0;
        }
        
    }
    return 0;
}
