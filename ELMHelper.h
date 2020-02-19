#ifndef ELM327Helper_H_
#define ELM327Helper_H_

#include <stdio.h>
#include <string.h>
#include "SerialComm.h"


#define ATICReset "ATZ"
#define ATSetJ1939 "ATSPA"
#define ATSilentMon "ATCSM0"

BOOL InitiliazeELMForJ1939();



BOOL InitiliazeELMForJ1939(){
    DWORD BytesWritten = 0; 
    char SerialRxBuffer[SERIAL_BUFF_SIZE] = {0};  
    char SerialTxBuffer[SERIAL_BUFF_SIZE] = {0}; 
    
    //Reset ELM327
    strncpy(SerialTxBuffer,ATICReset,SERIAL_BUFF_SIZE);
    if (!WriteToSerialPort(SerialTxBuffer, &BytesWritten))
        {
            system("pause");
            printf("Failed to write\n");
            return 0;
        }
    //Read ELM version
    ReadFromSerialPort(SerialRxBuffer);


    //Set protocol as J1939
    strncpy(SerialTxBuffer,ATSetJ1939,SERIAL_BUFF_SIZE);
    WriteToSerialPort(SerialTxBuffer, &BytesWritten);
    //Read
    ReadFromSerialPort(SerialRxBuffer);


    //Set silent mointoring off
    strncpy(SerialTxBuffer,ATSilentMon,SERIAL_BUFF_SIZE);
    WriteToSerialPort(SerialTxBuffer, &BytesWritten);
    //Read
    ReadFromSerialPort(SerialRxBuffer);

    return TRUE;
}

#endif //
