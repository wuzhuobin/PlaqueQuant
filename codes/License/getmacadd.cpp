#include "getmacadd.h"

void getMacAdd(char* mac)
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	ULONG ulOutBufLen;
	pAdapterInfo=(PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO));
	ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	// 第一次调用GetAdapterInfo获取ulOutBufLen大小
	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen); 
	}

	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		if (pAdapter) 
		{

			sprintf(mac, "%02x-%02x-%02x-%02x-%02x-%02x", 
				pAdapter->Address[0],
				pAdapter->Address[1],
				pAdapter->Address[2],
				pAdapter->Address[3],
				pAdapter->Address[4],
				pAdapter->Address[5]);
		}
	}
	else
	{
		fprintf(stderr, "Call to GetAdaptersInfo failed.\n");
	}
	return;
}