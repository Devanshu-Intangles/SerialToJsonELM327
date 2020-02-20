#ifndef ELM327Helper_H_
#define ELM327Helper_H_

#include <stdio.h>
#include <string.h>
#include "SerialComm.h"
#include "Model.h"


#define ATICReset "ATZ"
#define ATSetJ1939 "ATSPA"
#define ATSilentMon "ATCSM0"
#define ATTurnHeaderOff "ATH0"
#define ATTurnHeaderOn "ATH1"
#define ATBatteryVoltage "ATRV"

char SerialRxBuffer[SERIAL_BUFF_SIZE] = {0};  
char SerialTxBuffer[SERIAL_BUFF_SIZE] = {0}; 
DWORD BytesWritten = 0; 

BOOL InitiliazeELMForJ1939();
BOOL SetHeaderOnOff(BOOL val);
BOOL SetBatteryVoltage(Packet * packet);


BOOL InitiliazeELMForJ1939(){
     
    
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

BOOL SetHeaderOnOff(BOOL val){
    val ?  strncpy(SerialTxBuffer,ATTurnHeaderOn,SERIAL_BUFF_SIZE):
           strncpy(SerialTxBuffer,ATTurnHeaderOff,SERIAL_BUFF_SIZE);
    WriteToSerialPort(SerialTxBuffer, &BytesWritten);
    ReadFromSerialPort(SerialRxBuffer);
}

BOOL SetBatteryVoltage(Packet * packet){
    strncpy(SerialTxBuffer,ATBatteryVoltage,SERIAL_BUFF_SIZE);
    WriteToSerialPort(SerialTxBuffer, &BytesWritten);
    ReadFromSerialPort(SerialRxBuffer);
    // (*packet).VB=SerialRxBuffer;
}

#endif //
