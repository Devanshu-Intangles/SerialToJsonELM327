#ifndef SerialComm_H
#define SerialComm_H
#include <stdio.h>
#include <windows.h>

#define READ_TIMEOUT 500 // milliseconds
#define PORT_NO "\\\\.\\COM10"
#define SERIAL_BUFF_SIZE 512

HANDLE hComm; 

BOOL InitializeSerialPort(char *portName);
BOOL WriteToSerialPort(char *serialBuffer, DWORD *BytesWritten);
BOOL ReadFromSerialPort(char *serialBuffer);
BOOL SetTotalTimeOut(int timeout);


#endif