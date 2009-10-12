#ifndef _MAIN_H
#define _MAIN_H

#include <windows.h>
#include <vector>
#include <math.h>

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;

extern byte* AgentArrayPtr;
extern byte* AgentArrayMaxPtr;

#define maxAgent *(unsigned int*)AgentArrayMaxPtr
#define mapLoading *(long*)(CurrentTarget+0x410)
#define myId *(long*)(AgentArrayPtr-0x54)
#define buildNumber *(long*)BuildNumber

#include "Agent.h"
#include "CPacket.h"
#include "Skillbar.h"
#include "GuildWarsFunctions.h"

extern CSectionA* MySectionA;
extern AgentArray Agents;

void SendPacket(CPacket* pak);
void ReloadSkillbar();
void WriteWhisper(const wchar_t* chatMsg, const wchar_t* chatName);
void TargetNearestFoe();
void TargetNearestAlly();
void TargetNearestItem();
void TargetCalledTarget();
void UseHero1Skill(long SkillNumber, long Target);
void UseHero2Skill(long SkillNumber, long Target);
void UseHero3Skill(long SkillNumber, long Target);
void ChangeMaxZoom(float fZoom);
wchar_t* GetAgentName(int agentId);

#define RESPONSE_INVALID	break //PostMessage((HWND)MsgLParam, 0x500, -1, -1);

struct LoggedSkillStruct {
	long AgentId;
	long MyId;
	long SkillId;
	float Activation;
	byte TeamId;
	word Allegiance;
	float Distance;
	long Ping;
};

#endif