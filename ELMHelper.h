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
#define ATTroubleCodes "ATDM1"

char SerialRxBuffer[SERIAL_BUFF_SIZE] = {0};
char SerialTxBuffer[SERIAL_BUFF_SIZE] = {0};
char ParamResult[820] = {0};
char DT[502] = {0};
DWORD BytesWritten = 0;

BOOL InitiliazeELMForJ1939();
BOOL SetHeaderOn(BOOL val);
BOOL SetBatteryVoltage(Packet *packet);
BOOL SetJ1939Params(Packet *packet);
void ReadLfParam(enum LFreqParams lFreqParam, char *ParamResult);
void ReadHfParam(enum HFreqParams hFreqParam, char *ParamResult);
void removespaces(char *s);
void SetTroubleCodes(Packet *packet);
void ConvertBuffToDM1Format(char * SerialRxBuffer);

BOOL InitiliazeELMForJ1939()
{

    //Reset ELM327
    strncpy(SerialTxBuffer, ATICReset, SERIAL_BUFF_SIZE);
    if (!WriteToSerialPort(SerialTxBuffer, &BytesWritten))
    {
        system("pause");
        printf("Failed to write\n");
        return 0;
    }
    //Read ELM version
    ReadFromSerialPort(SerialRxBuffer);

    //Set protocol as J1939
    strncpy(SerialTxBuffer, ATSetJ1939, SERIAL_BUFF_SIZE);
    WriteToSerialPort(SerialTxBuffer, &BytesWritten);
    //Read
    ReadFromSerialPort(SerialRxBuffer);

    //Set silent mointoring off
    strncpy(SerialTxBuffer, ATSilentMon, SERIAL_BUFF_SIZE);
    WriteToSerialPort(SerialTxBuffer, &BytesWritten);
    //Read
    ReadFromSerialPort(SerialRxBuffer);

    return TRUE;
}

BOOL SetHeaderOn(BOOL val)
{
    val ? strncpy(SerialTxBuffer, ATTurnHeaderOn, SERIAL_BUFF_SIZE) : strncpy(SerialTxBuffer, ATTurnHeaderOff, SERIAL_BUFF_SIZE);
    WriteToSerialPort(SerialTxBuffer, &BytesWritten);
    ReadFromSerialPort(SerialRxBuffer);
}

BOOL SetBatteryVoltage(Packet *packet)
{
    char *eptr;
    strncpy(SerialTxBuffer, ATBatteryVoltage, SERIAL_BUFF_SIZE);
    memset(SerialRxBuffer,0,SERIAL_BUFF_SIZE);
    WriteToSerialPort(SerialTxBuffer, &BytesWritten);
    ReadFromSerialPort(SerialRxBuffer);
    // printf("SerialRxBuffer=%s\n",SerialRxBuffer);
    strtok(SerialRxBuffer, "\r\n");
    //printf("Strtok=%s\n",strtok(SerialRxBuffer, "\n"));
    // printf("VB= %s\n", strtok(NULL, "\n"));
    (*packet).VB = strtod(strtok(NULL, "\n"), &eptr);
    (*packet).E = (*packet).VB > 0 ? 1 : 0;
}

BOOL SetJ1939Params(Packet *packet)
{
    int i = 0, j = 0;
    memset(ParamResult, 0, 820);
    for (i = FEEF; i <= FEEE; i++)
    {
        SetTotalTimeOut(200);
        for (j = F004; j <= FEF6; j++)
        {
            // printf("i=%d\tj=%d\n",i,j);
            ReadHfParam(j, ParamResult);
            strcat(ParamResult, ",");
        }
        SetTotalTimeOut(1000);
        ReadLfParam(i, ParamResult);
        strcat(ParamResult, ";");
    }
    // printf("\nParamResult=%s\n",ParamResult);
    (*packet).P = ParamResult;
}

void ReadHfParam(enum HFreqParams hFreqParam, char *ParamResult)
{
    char Result[23] = {0};
    char Param[6] = {0};
    switch (hFreqParam)
    {
    case F004:
        strncpy(SerialTxBuffer, "ATMPF0041", SERIAL_BUFF_SIZE);
        strncpy(Param, "F004=", 6);
        break;
    case F003:
        strncpy(SerialTxBuffer, "ATMPF0031", SERIAL_BUFF_SIZE);
        strncpy(Param, "F003=", 6);
        break;
    case FEF2:
        strncpy(SerialTxBuffer, "ATMPFEF21", SERIAL_BUFF_SIZE);
        strncpy(Param, "FEF2=", 6);
        break;
    case FEF6:
        strncpy(SerialTxBuffer, "ATMPFEF61", SERIAL_BUFF_SIZE);
        strncpy(Param, "FEF6=", 6);
        break;
    }
    char *eptr;
    WriteToSerialPort(SerialTxBuffer, &BytesWritten);
    ReadFromSerialPort(SerialRxBuffer);
    strtok(SerialRxBuffer, "\n");
    strncpy(Result, strtok(NULL, "\n"), 23);
    //printf("Before removing spaces %s\n", Result);
    removespaces(Result);
    //printf("%s\n",Result);
    strcat(ParamResult, Param);
    //memcpy(ParamResult,Result,16);
    strcat(ParamResult, Result);
}

void ReadLfParam(enum LFreqParams lFreqParam, char *ParamResult)
{
    char Result[23] = {0};
    char Param[6] = {0};
    switch (lFreqParam)
    {
    case FEEF:
        strncpy(SerialTxBuffer, "ATMPFEEF1", SERIAL_BUFF_SIZE);
        strncpy(Param, "FEEF=", 6);
        break;
    case FEE9:
        strncpy(SerialTxBuffer, "ATMPFEE91", SERIAL_BUFF_SIZE);
        strncpy(Param, "FEE9=", 6);
        break;
    case FEE5:
        strncpy(SerialTxBuffer, "ATMPFEE51", SERIAL_BUFF_SIZE);
        strncpy(Param, "FEE5=", 5);
        break;
    case FEBF:
        strncpy(SerialTxBuffer, "ATMPFEBF1", SERIAL_BUFF_SIZE);
        strncpy(Param, "FEBF=", 6);
        break;
    case FECA:
        strncpy(SerialTxBuffer, "ATMPFECA1", SERIAL_BUFF_SIZE);
        strncpy(Param, "FECA=", 6);
        break;
    case FEEE:
        strncpy(SerialTxBuffer, "ATMPFEEE1", SERIAL_BUFF_SIZE);
        strncpy(Param, "FEEE=", 6);
        break;
    }
    char *eptr;
    WriteToSerialPort(SerialTxBuffer, &BytesWritten);
    ReadFromSerialPort(SerialRxBuffer);
    strtok(SerialRxBuffer, "\n");
    strncpy(Result, strtok(NULL, "\n"), 23);
    //printf("Before removing spaces %s\n", Result);
    removespaces(Result);
    //printf("%s\n",Result);
    strcat(ParamResult, Param);
    //memcpy(ParamResult,Result,16);
    strcat(ParamResult, Result);
}

void removespaces(char *s)
{
    const char *d = s;
    do
    {
        while (*d == ' ')
        {
            ++d;
        }
    } while (*s++ = *d++);
}
void SetTroubleCodes(Packet *packet)
{
    char *eptr, i = 0, *res = 0,*x="-";
    char delim[] = "\r\n";
    memset(DT,0,502);
    strncpy(SerialTxBuffer, ATTroubleCodes, SERIAL_BUFF_SIZE);
    
    WriteToSerialPort(SerialTxBuffer, &BytesWritten);
    SetTotalTimeOut(1000);
    ReadFromSerialPort(SerialRxBuffer);
    removespaces(SerialRxBuffer);
    // printf("%s",SerialRxBuffer);
    //ConvertBuffToDM1Format(SerialRxBuffer);
     
    res = strtok(SerialRxBuffer, delim);
    while (res != NULL)
    {
        res = strtok(NULL, delim);
        if(res!=NULL){
            strcat(DT,res);
            // printf( "%s", res ); //printing each res
            strcat(DT,"-");
            // printf("-");
        }
    }
     
    // printf("DT=%s\n",DT);
    (*packet).DT = DT;
}

void ConvertBuffToDM1Format(char * SerialRxBuffer){
    int count=0;
    printf("Before%s\n",SerialRxBuffer);
    while(SerialRxBuffer[count]!='\0'){
        if(SerialRxBuffer[count]=='\n'){
            SerialRxBuffer[count]='-';
        }
        printf("count=%d\tchar=%c\thexvalue=%02X\n",count,SerialRxBuffer[count],SerialRxBuffer[count]);
        count++;
    }
    printf("After%s\n",SerialRxBuffer);
}
#endif //
