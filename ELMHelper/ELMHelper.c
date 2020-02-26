#include "ELMHelper.h"

char SerialRxBuffer[SERIAL_BUFF_SIZE] = {0};
char SerialTxBuffer[SERIAL_BUFF_SIZE] = {0};
char ParamResult[820] = {0};
char DT[502] = {0};
DWORD BytesWritten = 0;

int priorityParam15765[] = {0x0C, 0x04, 0x0B, 0x5E, 0x06, 0x07, 0x10};

BOOL InitiliazeELMForJ1939()
{
    // Initialize Serial Port
    if (!InitializeSerialPort(PORT_NO))
    {
        system("pause");
        return 0;
    }
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

BOOL InitiliazeELMFor15765()
{
    // Initialize Serial Port
    if (!InitializeSerialPort(PORT_NO))
    {
        system("pause");
        return 0;
    }
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
    //Set protocol as 15765
    strncpy(SerialTxBuffer, ATSet15765, SERIAL_BUFF_SIZE);
    WriteToSerialPort(SerialTxBuffer, &BytesWritten);
    Sleep(10);
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
    memset(SerialRxBuffer, 0, SERIAL_BUFF_SIZE);
    WriteToSerialPort(SerialTxBuffer, &BytesWritten);
    ReadFromSerialPort(SerialRxBuffer);
    strtok(SerialRxBuffer, "\r\n");
    (*packet).VB = strtod(strtok(NULL, "\n"), &eptr);
    (*packet).E = (*packet).VB > 0 ? 1 : 0;
}

BOOL SetJ1939Params(Packet *packet)
{
    int i = 0, j = 0;
    memset(ParamResult, 0, 820);
    for (i = FEEF; i <= FEEE; i++)
    {
        // Decrease timeout for High frequency parameter
        SetTotalTimeOut(200);
        for (j = F004; j <= FEF6; j++)
        {
            ReadHfParam(j, ParamResult);
            strcat(ParamResult, ",");
        }
        // Increase timeout for low frequency parameter
        SetTotalTimeOut(1000);
        ReadLfParam(i, ParamResult);
        strcat(ParamResult, ";");
    }
    // printf("\nParamResult=%s\n",ParamResult);
    (*packet).P = ParamResult;
}
BOOL Set15765Params(Packet *packet)
{
    int i = 0, j = 0,
        highPriorityParmAvailable[7] = {0}, priorityCount = 0,
        lowPriorityParamCount = 0, lowpriorityParamAvailable[100] = {0};
    char paramPacket[1200] = {0}, tempArr[20] = {0};

    FindSuppHighAndLowFreqPIDS(highPriorityParmAvailable, &priorityCount, lowpriorityParamAvailable, &lowPriorityParamCount);
    
   
    for (i = 0; i < lowPriorityParamCount; i++)
    {
        for (j = 0; j < priorityCount; j++)
        {
            memset(tempArr, 0, 20);
            // printf("%02X\t",priorityParmAvailable[j]);
            sprintf(SerialTxBuffer, "01%02X", highPriorityParmAvailable[j]);
            WriteToSerialPort(SerialTxBuffer, &BytesWritten);
            ReadFromSerialPort(SerialRxBuffer);
            strtok(SerialRxBuffer, "\n");
            strtok(NULL, " ");
            strtok(NULL, " ");
            sprintf(tempArr, "%02X=%s,", highPriorityParmAvailable[j], strtok(NULL, "\r\n"));
            removespaces(tempArr);
            strcat(paramPacket, tempArr);
            // printf("%s,",tempArr);
        }
        memset(tempArr, 0, 20);
        // printf("%02X\n",lowpriorityParamAvailable[i]);
        sprintf(SerialTxBuffer, "01%02X", lowpriorityParamAvailable[i]);
        WriteToSerialPort(SerialTxBuffer, &BytesWritten);
        ReadFromSerialPort(SerialRxBuffer);
        strtok(SerialRxBuffer, "\n");
        strtok(NULL, " ");
        strtok(NULL, " ");
        sprintf(tempArr, "%02X=%s;", lowpriorityParamAvailable[i], strtok(NULL, "\r\n"));
        removespaces(tempArr);
        strcat(paramPacket, tempArr);
        // printf("%s",tempArr);
    }
    // printf("\n\nResult===%s\n\n",paramPacket);
    (*packet).P = paramPacket;
}

BOOL FindSuppHighAndLowFreqPIDS(int *highPriorityParmAvailable, int *highPriorityParamCount, int *lowPriorityParamAvailable, int *lowPriorityParamCount)
{
    int suppPIDs = 1, pidChunkCounter = 0, i = 0;
    *highPriorityParamCount=0;
    *lowPriorityParamCount=0;
    while (suppPIDs & 1 != 0)
    {
        suppPIDs = FindSupportedPIDs(pidChunkCounter);
        // printf("Supported PIDs==int=%d, hex=%X\n ",suppPIDs,suppPIDs);

        for (i = 1; i < 31; i++)
        {
            if (1 << (pidChunkCounter - i) & suppPIDs)
            {
                // printf("Command given to ELM327==01%02X\n",i+pidChunkCounter);

                if (checkValuePresentInArray((i + pidChunkCounter), priorityParam15765,7))
                {
                    highPriorityParmAvailable[*highPriorityParamCount] = i + pidChunkCounter;
                    // printf("HP=%02X\n", highPriorityParmAvailable[*highPriorityParamCount]);
                    *highPriorityParamCount=*highPriorityParamCount+1;
                }
                else
                {
                    lowPriorityParamAvailable[*lowPriorityParamCount] = i + pidChunkCounter;
                    // printf("LP=%02X\n", lowPriorityParamAvailable[*lowPriorityParamCount]);
                    *lowPriorityParamCount=*lowPriorityParamCount+1;
                }
            }
        }
        pidChunkCounter += 32;
    }
}

int FindSupportedPIDs(int pidChunkCounter)
{
    char suppPIDsArr[11] = {0};
    int suppPIDs = 0;
    sprintf(SerialTxBuffer, "01%02X", pidChunkCounter);
    SetTotalTimeOut(1000);
    WriteToSerialPort(SerialTxBuffer, &BytesWritten);
    ReadFromSerialPort(SerialRxBuffer);
    strtok(SerialRxBuffer, "\n");
    strtok(NULL, " ");
    strtok(NULL, " ");
    strcat(suppPIDsArr, strtok(NULL, " "));
    strcat(suppPIDsArr, strtok(NULL, " "));
    strcat(suppPIDsArr, strtok(NULL, " "));
    strcat(suppPIDsArr, strtok(NULL, " "));
    sscanf(suppPIDsArr, "%X", &suppPIDs);
    //suppPIDs=(int)strtol("D83A0011",NULL,16);

    return suppPIDs;
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
    removespaces(Result);
    // printf("%s\n",Result);
    strcat(ParamResult, Param);
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
    removespaces(Result);
    // printf("%s\n",Result);
    strcat(ParamResult, Param);
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

BOOL checkValuePresentInArray(int val, int Arr[], int lengthOfArr)
{
    int i = 0;
    for (i = 0; i < lengthOfArr; i++)
    {
        
        if (val == Arr[i])
        {
            return TRUE;
        }
    }
    return FALSE;
}
void SetTroubleCodes(Packet *packet)
{
    SetHeaderOn(1);

    char *eptr, i = 0, *res = 0, *x = "-";
    char delim[] = "\r\n";
    memset(DT, 0, 502);
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
        if (res != NULL)
        {
            strcat(DT, res);
            // printf( "%s", res ); //printing each res
            strcat(DT, "-");
            // printf("-");
        }
    }

    // printf("DT=%s\n",DT);
    (*packet).DT = DT;
    ReadFromSerialPort(SerialRxBuffer);
}

void SetTroubleCodes15765(Packet *packet)
{

    char *eptr, i = 0, *res = 0, *x = "-";
    char delim[] = "\r\n";
    memset(DT, 0, 502);
    strncpy(SerialTxBuffer, "03", SERIAL_BUFF_SIZE);

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
        if (res != NULL && strcmp(res, ">>"))
        {
            strcat(DT, res);
            // printf( "%s", res ); //printing each res
            // strcat(DT, "-");
            // printf("-");
        }
    }

    // printf("DT=%s\n",DT);
    (*packet).DT = DT;
    ReadFromSerialPort(SerialRxBuffer);
}
void ConvertBuffToDM1Format(char *SerialRxBuffer)
{
    int count = 0;
    printf("Before%s\n", SerialRxBuffer);
    while (SerialRxBuffer[count] != '\0')
    {
        if (SerialRxBuffer[count] == '\n')
        {
            SerialRxBuffer[count] = '-';
        }
        printf("count=%d\tchar=%c\thexvalue=%02X\n", count, SerialRxBuffer[count], SerialRxBuffer[count]);
        count++;
    }
    printf("After%s\n", SerialRxBuffer);
}