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

extern byte* PingLocation;
extern byte* LoggedInLocation;
extern byte* NameLocation;
extern byte* EmailLocation;
extern byte* DeadLocation;
extern byte* BasePointerLocation;
extern byte* MapIdLocation;
extern byte* RegionLocation;
extern byte* LanguageLocation;
extern byte* ConnectionLocation;

struct Bag;
struct Item;
struct ItemExtra;

struct MapOverlay;
struct Quest;

long GetTimeStamp();
template <typename T> T ReadPtrChain(
									 dword pBase = 0,
									 long pOffset1 = 0,
									 long pOffset2 = 0,
									 long pOffset3 = 0,
									 long pOffset4 = 0);

#define maxAgent *(unsigned int*)AgentArrayMaxPtr
#define mapLoading *(long*)(CurrentTarget+0x410)
#define myId *(long*)(AgentArrayPtr-0x54)
#define buildNumber *(long*)BuildNumber

#include "Agent.h"

extern CSectionA* MySectionA;
extern AgentArray Agents;
extern AgentMovementArray AgentMovements;

#include "MapOverlay.h"
#include "Item.h"
#include "CPacket.h"
#include "Skillbar.h"
#include "GuildWarsFunctions.h"

void SendPacket(CPacket* pak);
void HandleCommands();
void ReloadSkillbar();
void SellItem(long itemId);
void BuyItem(long id, long quantity, long value);
long GetItemValue(long itemId);
void RequestQuote(long id);
void RequestQuoteSell(long id);
bool TraderBuy();
bool TraderSell();
void OpenStorage();
void SendPartyInfo(HWND hwndReceiver, long teamId, long teamSize = 8);
void WriteWhisper(const wchar_t* chatMsg, const wchar_t* chatName);
void TargetNearestFoe();
void TargetNearestAlly();
void TargetNearestItem();
void TargetCalledTarget();
void TargetNextPartyMember();
void TargetNextFoe();
void GoAgent(long agentId);
void UseHeroSkill(long HeroId, long SkillNumber, long Target);
void UseSkillNew(long SkillId, long Target, long Event);
void UseSkillSuperNew(long SkillSlot, long Target);
void Move(float X, float Y);
void UpdateAgentPosition(long agentId);
void ChangeMaxZoom(float fZoom);
wchar_t* GetAgentName(int agentId);
void SetEngineHook(int Enable);
void WriteJMP(byte* location, byte* newFunction);

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
	long TargetId;
};

struct UpdateAgentPositionStruct {
	long Header; // = 0x21
	long AgentId;
	float X;
	float Y;
	float Zero1;
	float Zero2;
};

struct PartyPlayerInfo {
	long AgentId;
	float X;
	float Y;
	float HP;
	long Effects;
	byte Hex;
	wchar_t* NamePtr;
	byte Primary;
	byte Secondary;
	long Target;
	word Skill;
	word Weapon;
};

struct PartyInfo {
	HWND HwndReceiver;
	long TeamSize;
	long TeamId;
	PartyPlayerInfo Players[8];
};

#endif