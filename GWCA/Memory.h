#pragma once
#ifndef _MEMORY_H
#define _MEMORY_H
#include "stdafx.h"
/*  Class to work with GW memory. 
	Contains the memory scanner and a few memory reading commands */

struct Bag;
struct Item;
struct ItemExtra;

class Memory
{
public:
	byte* BaseOffset;
	byte* PacketSendFunction;
	byte* CurrentTarget;
	byte* AgentArrayPtr;
	byte* AgentArrayMaxPtr;
	byte* MessageHandlerStart;
	byte* MessageHandlerReturn;
	byte* SkillLogStart;
	byte* SkillLogReturn;
	byte* SkillTypeBase;
	byte* WriteWhisperStart;
	byte* TargetFunctions;
	byte* HeroSkillFunction;
	byte* ClickToMoveFix;
	byte* BuildNumber;
	byte* ChangeTargetFunction;
	byte* MaxZoomStill;
	byte* MaxZoomMobile;
	byte* SkillCancelStart;
	byte* SkillCancelReturn;
	byte* AgentNameFunction;
	byte* SellSessionStart;
	byte* SellSessionReturn;
	byte* SellItemFunction;
	byte* BuyItemFunction;
	byte* PingLocation;
	byte* LoggedInLocation;
	byte* NameLocation;
	byte* EmailLocation;
	byte* DeadLocation;
	byte* BasePointerLocation;
	byte* MapIdLocation;
	byte* DialogStart;
	byte* DialogReturn;
	byte* EngineStart;

	byte EngineHookSave[32];

	Memory(void);
	~Memory(void);
	template <typename T> T ReadPtrChain(dword pBase, long pOffset1 = 0, long pOffset2 = 0, long pOffset3 = 0, long pOffset4 = 0);
	void WriteJMP(byte* location, byte* newFunction);
	unsigned long FindPattern(unsigned char* bMask, char* szMask, unsigned long dw_Address, unsigned long dw_Len);
	bool bDataCompare(const unsigned char* pData, const unsigned char* bMask, const char* szMask);

	long MapId();
	long Ping();
	long LoggedIn();
	wchar_t* Name();
	wchar_t* Email();
	long Dead();
	dword BasePointer();
	dword SkillbarPtr();
	long CurrentBalthFaction();
	long MaxBalthFaction();
	long CurrentKurzickFaction();
	long MaxKurzickFaction();
	long CurrentLuxonFaction();
	long MaxLuxonFaction();
	long MoneySelf();
	long MoneyStorage();
	Bag** BagArrayPointer();
	long TitleTreasure();
	long TitleLucky();
	long TitleUnlucky();
	long TitleWisdom();
	long TitleDrunkard();
    long TitleSunspear();
    long TitleVanguard();
    long TitleNorn();
    long TitleAsura();
    long TitleDeldrimor();
    long TitleNorthMastery();
    long TitleLightbringer();
	long TitleSweet();
    long TitleParty();
    long TitleCommander();
    long TitleTrackLuxon();
    long TitleTrackKurzick();
	dword MerchantItems();
	dword HeroesStruct();
	dword GHStruct();
};
extern Memory GWMemory;
#endif