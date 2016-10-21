#pragma once
#include<winsock2.h>
#include<Iphlpapi.h>
#include<string>
#pragma comment(lib,"Iphlpapi.lib")

void getMacAdd(char* mac);