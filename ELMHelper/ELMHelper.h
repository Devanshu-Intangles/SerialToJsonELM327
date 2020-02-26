#ifndef ELM327Helper_H_
#define ELM327Helper_H_

#include <stdio.h>
#include <string.h>
#include "../SerialComm/SerialComm.h"
#include "../Model.h"

#define ATICReset "ATZ"
#define ATSetJ1939 "ATSPA"
#define ATSet15765 "ATSP0"
#define ATSilentMon "ATCSM0"
#define ATTurnHeaderOff "ATH0"
#define ATTurnHeaderOn "ATH1"
#define ATBatteryVoltage "ATRV"
#define ATTroubleCodes "ATDM1"
#define SupportedPIDs "0100"


BOOL InitiliazeELMForJ1939();
BOOL InitiliazeELMFor15765();
BOOL SetHeaderOn(BOOL val);
BOOL SetBatteryVoltage(Packet *packet);
BOOL SetJ1939Params(Packet *packet);
void ReadLfParam(enum LFreqParams lFreqParam, char *ParamResult);
void ReadHfParam(enum HFreqParams hFreqParam, char *ParamResult);
void removespaces(char *s);
void SetTroubleCodes(Packet *packet);
void SetTroubleCodes15765(Packet *packet);
void ConvertBuffToDM1Format(char * SerialRxBuffer);
BOOL Set15765Params(Packet *packet);
int FindSupportedPIDs(int pidChunkCounter);
BOOL checkValuePresentInArray(int val,int Arr[],int lengthOfArr);
BOOL FindSuppHighAndLowFreqPIDS(int * highPriorityParmAvailable,int * highPriorityParamCount,int * lowPriorityParamAvailable, int * lowPriorityParamCount);
#endif //
