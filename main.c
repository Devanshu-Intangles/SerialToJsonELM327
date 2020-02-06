#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "SerialComm.h"

int main()
{
    HANDLE hComm;                              // Handle to the Serial port
    BOOL Status;                               // Status
    char SerialBuffer[SERIAL_BUFF_SIZE] = {0}; //Buffer to send and receive data
    DWORD BytesWritten = 0;                    // No of bytes written to the port

    char *pszPortName = PORT_NO; //com port id

    if (!InitializeSerialPort(&hComm, pszPortName))
    {
        system("pause");
        return 0;
    }

    while(TRUE)
    {
        printf("\n\nEnter your message: ");
        scanf("%s", SerialBuffer);

        if (!WriteToSerialPort(hComm, SerialBuffer, &BytesWritten))
        {
            system("pause");
            return 0;
        }

        if (!ReadFromSerialPort(hComm, SerialBuffer))
        {
            system("pause");
            return 0;
        }
    }
    return 0;
}
