#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <conio.h>

/* seems to not be working..whatever
#pragma comment(lib, "sendrecv.lib")

extern "C"
{
	int __stdcall QuickSend(char* server, int port, char* request, int reqLen, char* response_buffer, int response_buflen, int ms_timeout);
	int __stdcall LastError(char* buffer, int buflen);
}*/

//but typedefs dont show protptype in intellisense? 
typedef	int( __stdcall *QuickSend)(char* request, int reqLen, char* response_buffer, int response_buflen);
typedef int( __stdcall *LastError)(char* buffer, int buflen);	
typedef void(__stdcall *qsConfig)(char* _server, int _port, int _timeout, short partialRespOk);

QuickSend quickSend = NULL;
LastError lastError = NULL;
qsConfig  qsconfig = NULL;

void main(void){

	char* http = "GET /tools.php HTTP/1.0\r\n" 
                "Host: sandsprite.com\r\n" 
                "User-Agent: Mozilla/5.0 (Windows NT 5.1; rv:45.0)\r\n" 
                "Accept-Encoding: none\r\n" 
                "Connection: close\r\n" 
                "\r\n\r\n";

	char buf[100];

	memset(buf,0,sizeof(buf));

	int h = (int)LoadLibrary("sendrecv.dll");
	if(h==0) h = (int)LoadLibrary("./../sendrecv.dll");
	if(h==0) h = (int)LoadLibrary("./../../sendrecv.dll");

	if(h==0){
		MessageBox(0,"Could not locate sendrecv.dll","",0);
		return;
	}

	qsconfig = (qsConfig)GetProcAddress((HMODULE)h,"qsConfig");
	quickSend = (QuickSend)GetProcAddress((HMODULE)h,"QuickSend");
	lastError = (LastError)GetProcAddress((HMODULE)h,"LastError");
	
	if( (int)qsconfig == 0 || (int)quickSend == 0 || (int)lastError == 0){
		MessageBox(0,"Could not locate imports from sendrecv.dll","",0);
		return;
	}

	char* server = "sandsprite.com";
	//char* server = "192.168.0.10";

	(*qsconfig)(server,80,12000,1);
	int sz = (*quickSend)(http,strlen(http), buf, sizeof(buf));
	
	if(sz > 0){
		printf("%s",buf);
	}else{
		printf("Error:");
	}

	printf("\n\n");
	if( (*lastError)(buf, sizeof(buf)) > 0) printf("%s\n",buf);
	printf("press any key to exit...\n");
	getch();

}