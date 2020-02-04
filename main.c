#include <Windows.h>
#include <stdio.h>

int main(){
HANDLE hComm;
HANDLE hComm;
DWORD dwRead;
BOOL fWaitingOnRead = FALSE;
OVERLAPPED osReader = {0};
hComm = CreateFile( gszPort,  
                    GENERIC_READ | GENERIC_WRITE, 
                    0, 
                    0, 
                    OPEN_EXISTING,
                    FILE_FLAG_OVERLAPPED,
                    0);
if (hComm == INVALID_HANDLE_VALUE){
    printf("Error: Invalid Handle Value\n");
}


// Create the overlapped event. Must be closed before exiting
// to avoid a handle leak.
osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

if (osReader.hEvent == NULL)
   // Error creating overlapped event; abort.

if (!fWaitingOnRead) {
   // Issue read operation.
   if (!ReadFile(hComm, lpBuf, READ_BUF_SIZE, &dwRead, &osReader)) {
      if (GetLastError() != ERROR_IO_PENDING)     // read not delayed?
         // Error in communications; report it.
      else
         fWaitingOnRead = TRUE;
   }
   else {    
      // read completed immediately
      HandleASuccessfulRead(lpBuf, dwRead);
    }
}
return 0;
}