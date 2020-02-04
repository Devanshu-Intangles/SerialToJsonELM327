#include <Windows.h>
#include <stdio.h>
#include <string.h>
#define READ_TIMEOUT      500      // milliseconds

int main(){
HANDLE hComm;  // Handle to the Serial port
    BOOL   Status; // Status
    DCB dcbSerialParams = { 0 };  // Initializing DCB structure
    COMMTIMEOUTS timeouts = { 0 };  //Initializing timeouts structure
    char SerialBuffer[64] = { 0 }; //Buffer to send and receive data
    DWORD BytesWritten = 0;          // No of bytes written to the port
    DWORD dwEventMask;     // Event mask to trigger
    char  ReadData;        //temperory Character
    DWORD NoBytesRead;     // Bytes read by ReadFile()
    unsigned char loop = 0;

    char pszPortName[10] = { 0 }; //com port id



    //Enter the com port id
    printf("Enter the Com Port: ");
    scanf("%s", pszPortName);

    //Open the serial com port
    hComm = CreateFile(pszPortName, //friendly name
                       GENERIC_READ | GENERIC_WRITE,      // Read/Write Access
                       0,                                 // No Sharing, ports cant be shared
                       NULL,                              // No Security
                       OPEN_EXISTING,                     // Open existing port only
                       0,                                 // Non Overlapped I/O
                       NULL);                             // Null for Comm Devices

    if (hComm == INVALID_HANDLE_VALUE)
    {
        printf("\n Port can't be opened\n\n");
        goto Exit2;
    }

    //Setting the Parameters for the SerialPort
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    Status = GetCommState(hComm, &dcbSerialParams); //retreives  the current settings
    if (Status == FALSE)
    {
        printf("\nError to Get the Com state\n\n");
        goto Exit1;
    }

    dcbSerialParams.BaudRate = CBR_9600;      //BaudRate = 9600
    dcbSerialParams.ByteSize = 8;             //ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;    //StopBits = 1
    dcbSerialParams.Parity = NOPARITY;      //Parity = None

    Status = SetCommState(hComm, &dcbSerialParams);
    if (Status == FALSE)
    {
        printf("\nError to Setting DCB Structure\n\n");
        goto Exit1;
    }

    //Setting Timeouts
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (SetCommTimeouts(hComm, &timeouts) == FALSE)
    {
        printf("\nError to Setting Time outs");
        goto Exit1;
    }


    printf("\n\nEnter your message: ");
    scanf("%s", SerialBuffer);


    //Writing data to Serial Port
    Status = WriteFile(hComm,// Handle to the Serialport
                       SerialBuffer,            // Data to be written to the port
                       sizeof(SerialBuffer),   // No of bytes to write into the port
                       &BytesWritten,  // No of bytes written to the port
                       NULL);
    if (Status == FALSE)
    {
        printf("\nFail to Written");
        goto Exit1;
    }

    //print numbers of byte written to the serial port
    printf("\nNumber of bytes written to the serail port = %d\n\n", BytesWritten);

    //Setting Receive Mask
    Status = SetCommMask(hComm, EV_RXCHAR);

    if (Status == FALSE)
    {
        printf("\nError to in Setting CommMask\n\n");
        goto Exit1;
    }


    //Setting WaitComm() Event
    Status = WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received
    if (Status == FALSE)
    {
        printf("\nError! in Setting WaitCommEvent()\n\n");
        goto Exit1;
    }

    //Read data and store in a buffer
    do
    {
        Status = ReadFile(hComm, &ReadData, sizeof(ReadData), &NoBytesRead, NULL);
        SerialBuffer[loop] = ReadData;
        ++loop;
    }
    while (NoBytesRead > 0);

    --loop; //Get Actual length of received data

    printf("\nNumber of bytes received = %d\n\n", loop);

    //print receive data on console
    printf("\n\n");
    int index = 0;
    for (index = 0; index < loop; ++index)
    {
        printf("%c", SerialBuffer[index]);
    }

    printf("\n\n");

Exit1:
    CloseHandle(hComm);//Closing the Serial Port
Exit2:
    system("pause");
    return 0;
}

