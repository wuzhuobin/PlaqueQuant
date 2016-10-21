#include<winsock2.h>
#include<Iphlpapi.h>
#include<stdio.h>

#pragma comment(lib,"Iphlpapi.lib")

int main()
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
		while (pAdapter) 
		{
			printf("Adapter Name: \t%s\n", pAdapter->AdapterName);
			printf("Adapter Desc: \t%s\n", pAdapter->Description);
			printf("MAC Addr: \t%02x-%02x-%02x-%02x-%02x-%02x\n", 
				pAdapter->Address[0],
				pAdapter->Address[1],
				pAdapter->Address[2],
				pAdapter->Address[3],
				pAdapter->Address[4],
				pAdapter->Address[5]);
			printf("IP Address: \t%s\n", pAdapter->IpAddressList.IpAddress.String);
			printf("IP Mask: \t%s\n", pAdapter->IpAddressList.IpMask.String); 
			printf("Gateway: \t%s\n", pAdapter->GatewayList.IpAddress.String);
			pAdapter = pAdapter->Next;
		}
	}
	else
	{
		printf("Call to GetAdaptersInfo failed.\n");
	}
}