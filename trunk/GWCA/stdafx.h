#pragma once
#ifndef _STDAFX_H
#define _STDAFX_H
/* Standard definitions and includes */

//Deactivate fucking warnings :X
#pragma warning(disable:4996)
#pragma warning(disable:4800)
#pragma warning(disable:4244)

#include <windows.h>
#include <vector>
#include <math.h>
#include <string>

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;

#include "Memory.h"

#define maxAgent *(unsigned int*)GWMemory.AgentArrayMaxPtr
#define mapLoading *(long*)(GWMemory.CurrentTarget+0x410)
#define myId *(long*)(GWMemory.AgentArrayPtr-0x54)
#define buildNumber *(long*)GWMemory.BuildNumber

#define RESPONSE_INVALID	break //PostMessage((HWND)MsgLParam, 0x500, -1, -1);

void InjectErr(const char* lpzText);


#endif