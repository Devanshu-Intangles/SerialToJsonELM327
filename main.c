#include <Windows.h>
#include <stdio.h>
#include <string.h>
#define READ_TIMEOUT      500      // milliseconds
#define PORT_NO  "COM3"
#define SERIAL_BUFF_SIZE 64

BOOL InitializeSerialPort(HANDLE* hComm,char *portName);
BOOL WriteToSerialPort(HANDLE hComm,char *serialBuffer, DWORD* BytesWritten );
BOOL ReadFromSerialPort(HANDLE hComm, char * serialBuffer);

int main(){
HANDLE hComm;  // Handle to the Serial port
    BOOL   Status; // Status
    char SerialBuffer[SERIAL_BUFF_SIZE] = "ATZ"; //Buffer to send and receive data
    DWORD BytesWritten = 0;          // No of bytes written to the port

    char pszPortName[10] = PORT_NO; //com port id
    

    Status= InitializeSerialPort(&hComm,pszPortName);
    if(Status==FALSE){
        system("pause");
        return 0;
    }


    printf("\n\nEnter your message: ");
    scanf("%s", SerialBuffer);
    strcat(SerialBuffer,"\r");
    printf("Sending %s to COM port ", SerialBuffer);

    Status =WriteToSerialPort(hComm,SerialBuffer, &BytesWritten);
    if (Status == FALSE)
    {
        system("pause");
        return 0;
    }
    
    ReadFromSerialPort(hComm, SerialBuffer);
    
    return 0;
}

BOOL InitializeSerialPort(HANDLE * hComm, char *portName){
     //Open the serial com port
    BOOL   Status; // Status
    DCB dcbSerialParams = { 0 };  // Initializing DCB structure

    *hComm = CreateFile(portName, //friendly name
                       GENERIC_READ | GENERIC_WRITE,      // Read/Write Access
                       0,                                 // No Sharing, ports cant be shared
                       NULL,                              // No Security
                       OPEN_EXISTING,                     // Open existing port only
                       0,                                 // Non Overlapped I/O
                       NULL);                             // Null for Comm Devices

   
   
    if (*hComm == INVALID_HANDLE_VALUE)
    {
        printf("\n Port can't be opened\n\n");
        return FALSE;
    }
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    Status = GetCommState(*hComm, &dcbSerialParams); //retreives  the current settings
    if (Status == FALSE)
    {
        printf("\nError to Get the Com state\n\n");
        CloseHandle(*hComm);
        return FALSE;
    }

    dcbSerialParams.BaudRate = CBR_38400;      //BaudRate = 9600
    dcbSerialParams.ByteSize = 8;             //ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;    //StopBits = 1
    dcbSerialParams.Parity = NOPARITY;      //Parity = None

    Status = SetCommState(*hComm, &dcbSerialParams);
    if (Status == FALSE)
    {
        printf("\nError to Setting DCB Structure\n\n");
        CloseHandle(*hComm);
        return FALSE;
    }
    return TRUE;
}

BOOL WriteToSerialPort(HANDLE hComm,char *serialBuffer, DWORD* BytesWritten ){
    BOOL Status;
    Status = WriteFile(hComm,// Handle to the Serialport
                       serialBuffer,            // Data to be written to the port
                       SERIAL_BUFF_SIZE,   // No of bytes to write into the port
                       BytesWritten,  // No of bytes written to the port
                       NULL);
    // printf("\nSerialBuffer in method = %s and sizeof of buffer=%d \n",serialBuffer, sizeof(serialBuffer));
    if (Status == FALSE)
    {
        CloseHandle(hComm);//Closing the Serial Port
        printf("\nFail to Written");
        return FALSE;
    }
    else
    {
        printf("\nWrite sucess");
    }
    
    //print numbers of byte written to the serial port
    printf("\nNumber of bytes written to the serail port = %d\n\n", *BytesWritten);
    return TRUE;



}

BOOL ReadFromSerialPort(HANDLE hComm, char * serialBuffer){
    DWORD dwEventMask,NoBytesRead;     //  Event mask to trigger // Bytes read by ReadFile();    
    int loop=0,index=0;
    unsigned char tempChar;
    BOOL Status;
    //Setting Receive Mask
    Status = SetCommMask(hComm, EV_RXCHAR);

    if (Status == FALSE)
    {
        printf("\nError to in Setting CommMask\n\n");
        CloseHandle(hComm);//Closing the Serial Port
        return FALSE;
    }


    //Setting WaitComm() Event
    Status = WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received
    if (Status == FALSE)
    {
        printf("\nError! in Setting WaitCommEvent()\n\n");
        CloseHandle(hComm);//Closing the Serial Port
        return FALSE;
    }

    //Read data and store in a buffer
    do
    {
        Status = ReadFile(hComm, &tempChar, sizeof(tempChar), &NoBytesRead, NULL);
        serialBuffer[loop] = tempChar;
        ++loop;
    }
    while (NoBytesRead > 0);

    --loop; //Get Actual length of received data

    printf("\nNumber of bytes received = %d\n\n", loop);

    //print receive data on console
    printf("\n\n");

    index = 0;
    for (index = 0; index < loop; ++index)
    {
        printf("%c", serialBuffer[index]);
    }
    printf("\n\n");
}

