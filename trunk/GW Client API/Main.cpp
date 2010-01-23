#include "Main.h"

/*	Guild Wars Client API - GWCA
	This is a large set of functions that makes interfacing with Guild Wars a lot easier.
	Uses Window Messages for communicating by hooking the WndProc of GW, allowing you
	to SendMessage directly to the Guild Wars window.
	Protocol rules are, if your Message expects a reply, then you have to provide the
	window handle of your application in lParam. The reply is sent with code 0x500. */

byte* BaseOffset = NULL;
byte* PacketSendFunction = NULL;
byte* CurrentTarget = NULL;
byte* AgentArrayPtr = NULL;
byte* AgentArrayMaxPtr = NULL;
byte* MessageHandlerStart = NULL;
byte* MessageHandlerReturn = NULL;
byte* SkillLogStart = NULL;
byte* SkillLogReturn = NULL;
byte* WriteWhisperStart = NULL;
byte* TargetFunctions = NULL;
byte* HeroSkillFunction = NULL;
byte* ClickToMoveFix = NULL;
byte* BuildNumber = NULL;
byte* ChangeTargetFunction = NULL;
byte* MaxZoomStill = NULL;
byte* MaxZoomMobile = NULL;
byte* SkillCancelStart = NULL;
byte* SkillCancelReturn = NULL;
//byte* AgentNameFunction = NULL;
byte* SellSessionStart = NULL;
byte* SellSessionReturn = NULL;
byte* SellItemFunction = NULL;
byte* BuyItemFunction = NULL;
byte* PingLocation = NULL;
byte* LoggedInLocation = NULL;
byte* NameLocation = NULL;
byte* EmailLocation = NULL;
byte* DeadLocation = NULL;
byte* BasePointerLocation = NULL;
byte* MapIdLocation = NULL;
byte* DialogStart = NULL;
byte* DialogReturn = NULL;
byte* EngineStart = NULL;
byte* SkillTypeBase = NULL;
byte* WinHandle = NULL;
byte* LoadFinished = NULL;
byte* TargetLogStart = NULL;
byte* TargetLogReturn = NULL;
byte* VirtualHeroFlagFunction = NULL;
byte* SetAttrisFunc = NULL;

dword FlagLocation = 0;
dword PacketLocation = 0;

AgentArray Agents;

bool LogSkills = false;
HWND ScriptHwnd = NULL;
wchar_t* pName;
long MoveItemId = NULL;
long TmpVariable = NULL;
long CurrentBag = 1;

long SellSessionId = NULL;
long LastDialogId = 0;
byte EngineHookSave[32];

bool FinishedLoading = false;

long* AgentTargets = new long[2560];

long PartyTeamSize = 8;

Skillbar* MySkillbar = NULL;
CSectionA* MySectionA = new CSectionA();
ItemManager* MyItemManager = new ItemManager();

unsigned int MsgUInt = NULL;
WPARAM MsgWParam = NULL;
LPARAM MsgLParam = NULL;
HWND MsgHwnd = NULL;
int MsgInt;
int MsgInt2;
int MsgEvent = 0;
float MsgFloat;
float MsgFloat2;
int MsgLoop = 0;

HANDLE PacketMutex;
HANDLE PartyMutex;
std::vector<CPacket*> PacketQueue;
std::vector<SkillLogSkill> SkillLogQueue;
std::vector<SkillLogSkill> SkillCancelQueue;
std::vector<PartyInfo*> PartyInfoQueue;
std::vector<long> TeamAgents;

void _declspec(naked) SkillLogHook(){
	SkillLogSkill* skillPtr;

	_asm {
		POP EDI
		MOV skillPtr,EDI
		MOV EAX,DWORD PTR DS:[ESI+0x10]
		INC EAX
		MOV DWORD PTR DS:[ESI+0x10],EAX
	}

	SkillLogQueue.push_back(*skillPtr);

	_asm {
		JMP SkillLogReturn
	}
}

void _declspec(naked) SkillCancelHook(){
	SkillLogSkill* cancelSkillPtr;

	_asm {
		MOV ESI,ECX
		MOV EAX,DWORD PTR DS:[EDI]
		MOV ECX,DWORD PTR DS:[ESI+4]
		PUSHAD
		MOV cancelSkillPtr,EDI
		CMP EAX,ECX
		JNZ SkillCancelSkip
	}

	SkillCancelQueue.push_back(*cancelSkillPtr);

	_asm {
SkillCancelSkip:
		POPAD
		JMP SkillCancelReturn
	}
}

void _declspec(naked) SellSessionHook(){
	_asm {
		PUSH ESI
		MOV ESI,ECX
		PUSH EDI

		MOV EDX,DWORD PTR DS:[ESI+4]
		MOV SellSessionId,EDX

		MOV EDX,2

		JMP SellSessionReturn
	}
}

void _declspec(naked) DialogHook(){
	_asm {
		PUSH EBP
		MOV EBP,ESP
		
		MOV EAX,DWORD PTR SS:[EBP+8]
		MOV LastDialogId,EAX

		MOV EAX,DWORD PTR DS:[ECX+8]
		TEST AL,1
		JMP DialogReturn
	}
}

void _declspec(naked) EngineHook(){
	Sleep(100);
	_asm RET
}

void _declspec(naked) LoadHook(){
	for(int i = 1;i < 2560;i++){ AgentTargets[i] = 0; }
	FinishedLoading = true;

	_asm {
		MOV ESP,EBP
		POP EBP
		RETN 0x10
	}
}

void _declspec(naked) TargetLogHook(){
	long agentTarget, agentCaster, actionType;

	_asm {
		PUSHAD
		MOV EAX,DWORD PTR DS:[EBP+8]
		MOV agentTarget,EAX
		MOV agentCaster,EDX
		MOV actionType,ECX
	}

	if(	actionType == 0x39 ||
		actionType == 0x03){
		AgentTargets[agentCaster] = agentTarget;
	}

	_asm {
		POPAD
		PUSH EBX
		MOV DWORD PTR SS:[EBP-0xC],ECX
		PUSH ESI
		JMP TargetLogReturn
	}
}
void SetAttribute(dword atr,dword val){
	dword Attribute = atr;
	dword Value = val;
	long id = myId;
	_asm{
		MOV EDX,1  //attri count
		LEA ECX,Value//value
		PUSH ECX
		LEA EAX,Attribute//attribute
		PUSH EAX
		MOV ECX,id //AgentId to change attris
		CALL SetAttrisFunc
	}
}
void _declspec(naked) CustomMsgHandler(){
	_asm {
		MOV EAX,DWORD PTR DS:[EBP+0x8]
		MOV MsgHwnd,EAX
		MOV EAX,DWORD PTR DS:[EBP+0xC]
		MOV MsgUInt,EAX
		MOV EAX,DWORD PTR DS:[EBP+0x10]
		MOV MsgWParam,EAX
		MOV EAX,DWORD PTR DS:[EBP+0x14]
		MOV MsgLParam,EAX
	}

	if(MsgUInt < 0x400){
		_asm {
			SUB ESP,0x20
			PUSH EBX
			PUSH ESI
			PUSH EDI
			MOV EDI,DWORD PTR SS:[EBP+8]
			JMP MessageHandlerReturn
		}
	}

	switch(MsgUInt){
		//Stuff related to you
		case 0x401: //Current Target : No return
			PostMessage((HWND)MsgLParam, 0x500, *(long*)CurrentTarget, 0);
			break;
		case 0x402: //Get your own agent ID : Return int
			PostMessage((HWND)MsgLParam, 0x500, myId, 0);
			break;
		case 0x403: //Check if you're casting : Return int/bool
			ReloadSkillbar();
			if(MySkillbar==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, MySkillbar->Casting, 0);
			break;
		case 0x404: //Check if skill is recharging : Return int/long
			ReloadSkillbar();
			if(MySkillbar==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, (MySkillbar->Skill[MsgWParam-1].Recharge), 0);
			break;
		case 0x405: //Check adrenaline points of a skill : Return int/long
			ReloadSkillbar();
			if(MySkillbar==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, MySkillbar->Skill[MsgWParam-1].Adrenaline, 0);
		case 0x406: //Put stuff in Msg variables : No return
			switch(MsgWParam){
				case 1:
					MsgInt = MsgLParam;
					break;
				case 2:
					MsgInt2 = MsgLParam;
					break;
				case 3:
					memcpy(&MsgFloat, &MsgLParam, sizeof(int));
					break;
				case 4:
					memcpy(&MsgFloat2, &MsgLParam, sizeof(int));
					break;
			}
		case 0x407: //Set SkillLog and Script hWnd : No return
			LogSkills = (bool)MsgWParam;
			ScriptHwnd = (HWND)MsgLParam;
			break;
		case 0x408: //Get base Agent array pointer and Current target pointer : Return ptr & ptr
			PostMessage((HWND)MsgLParam, 0x500, (WPARAM)AgentArrayPtr, (LPARAM)CurrentTarget);
			break;
		case 0x409: //Get skill id of skills on your Skillbar : Return int/dword
			ReloadSkillbar();
			if(MySkillbar==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, MySkillbar->Skill[MsgWParam-1].Id, 0);
			break;
		case 0x40A: //Get your own max health (and current health): Return int/long & float
			if(Agents[myId]==NULL){RESPONSE_INVALID;}
			MsgInt = (int)(floor(Agents[myId]->MaxHP * Agents[myId]->HP));
			PostMessage((HWND)MsgLParam, 0x500, Agents[myId]->MaxHP, MsgInt);
			break;
		case 0x40B: //Get your own max energy (and current energy): Return int/long & float
			if(Agents[myId]==NULL){RESPONSE_INVALID;}
			MsgInt = (int)(floor(Agents[myId]->MaxEnergy * Agents[myId]->Energy));
			PostMessage((HWND)MsgLParam, 0x500, Agents[myId]->MaxEnergy, MsgInt);
			break;
		case 0x40C: //Get build number of GW : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, buildNumber, 0);
			break;
		case 0x40D: //Change max zoom of GW : No return
			MsgFloat = (float)MsgWParam;
			if(MsgFloat < 0 || MsgFloat > 10000){RESPONSE_INVALID;}
			ChangeMaxZoom(MsgFloat);
			break;
		case 0x40E: //Get last dialog id : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, LastDialogId, 0);
			break;
		case 0x40F: //Enable or disable graphics rendering : No return
			SetEngineHook(MsgWParam);
			break;
			
		//Packet Related Commands
		case 0x410: //Attack : No return
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			AttackTarget(MsgWParam);
			break;
		case 0x411: //Move to x, y : No return
			memcpy(&MsgFloat, &MsgWParam, sizeof(int));
			memcpy(&MsgFloat2, &MsgLParam, sizeof(int));
			MovePlayer(MsgFloat, MsgFloat2);
			break;
		case 0x412: //Use skill : No return
			ReloadSkillbar();
			if(MySkillbar == NULL){break;}
			if(MsgLParam == -1){MsgLParam = *(long*)CurrentTarget;}
			if(MsgLParam == -2){MsgLParam = myId;}
			MsgInt = MySkillbar->Skill[MsgWParam-1].Id;
			UseSkillNew(MsgInt, MsgLParam, MySkillbar->Skill[MsgWParam-1].Event);
			break;
		case 0x413: //Change weapon set : No return
			ChangeWeaponSet(MsgWParam-1);
			break;
		case 0x414: //Zone map : No return
			if(MsgLParam!=NULL)
				MoveMap(MsgWParam, 2, MsgLParam);
			else
				MoveMap(MsgWParam);
			break;
		case 0x415: //Drop gold : No return
			DropGold(MsgWParam);
			break;
		case 0x416: //Go to NPC : No return
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			GoNPC(MsgWParam);
			break;
		case 0x417: //Go to player : No return
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			GoPlayer(MsgWParam);
			break;
		case 0x418: //Go to signpost : No return
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			GoSignpost(MsgWParam);
			break;
		case 0x419: //Use attack skill : No return
			ReloadSkillbar();
			if(MySkillbar == NULL){break;}
			if(MsgLParam == -1){MsgLParam = *(long*)CurrentTarget;}
			if(MsgLParam == -2){MsgLParam = myId;}
			MsgInt = MySkillbar->Skill[MsgWParam-1].Id;
			UseSkillNew(MsgInt, MsgLParam, MySkillbar->Skill[MsgWParam-1].Event);
			break;
		case 0x41A: //Enter challenge mission : No return
			EnterChallenge();
			break;
		case 0x41B: //Open chest : No return
			OpenChest();
			break;
		case 0x41C: //Set event skill mode : No return
			MsgEvent = (int)MsgWParam;
			break;
		case 0x41D: //Use skillbar skill : No return
			ReloadSkillbar();
			if(MySkillbar == NULL){break;}
			if(MsgLParam == -1){MsgLParam = *(long*)CurrentTarget;}
			if(MsgLParam == -2){MsgLParam = myId;}
			MsgInt = MySkillbar->Skill[MsgWParam-1].Id;
			UseSkillNew(MsgInt, MsgLParam, MySkillbar->Skill[MsgWParam-1].Event);
			break;
		case 0x41E: //Pick up item : No return
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			PickUpItem(MsgWParam);
			break;
		case 0x41F: //Use skillbar attack skill : No return
			ReloadSkillbar();
			if(MySkillbar == NULL){break;}
			if(MsgLParam == -1){MsgLParam = *(long*)CurrentTarget;}
			if(MsgLParam == -2){MsgLParam = myId;}
			MsgInt = MySkillbar->Skill[MsgWParam-1].Id;
			UseSkillNew(MsgInt, MsgLParam, MySkillbar->Skill[MsgWParam-1].Event);
			break;
		case 0x420: //Dialog packet : No return
			Dialog(MsgWParam);
			break;
		case 0x421: //Change target : No return
			if(Agents[MsgWParam]==NULL){break;}
			_asm MOV ECX,MsgWParam
			_asm MOV EDX,0
			_asm CALL ChangeTargetFunction
			break;
		case 0x422: //Write status about Bot (and/or Lock) : No return (wparam=bot, lparam=lock, 0=nothing, 1=on, 2=off)
			switch(MsgWParam){
				case 1:
					WriteWhisper(L"Interrupts turned on", L"GWCA");
					break;
				case 2:
					WriteWhisper(L"Interrupts turned off", L"GWCA");
					break;
			}
			switch(MsgLParam){
				case 1:
					WriteWhisper(L"Locked on target", L"Lock On");
					break;
				case 2:
					WriteWhisper(L"No longer locked on target", L"Lock Off");
					break;
			}
			break;
		case 0x423: //Target nearest foe : No return
			TargetNearestFoe();
			break;
		case 0x424: //Target nearest ally : No return
			TargetNearestAlly();
			break;
		case 0x425: //Target nearest item : No return
			TargetNearestItem();
			break;
		case 0x426: //Write status about Bot Delay : No return
			switch(MsgWParam){
				case 0:
					WriteWhisper(L"Interrupt delay is off", L"Delay Off");
					break;
				case 1:
					WriteWhisper(L"Interrupt delay is on", L"Delay On");
					break;
			}
			break;
		case 0x427: //Target called target : No return
			TargetCalledTarget();
			break;
		case 0x428: //Use hero 1 skill : No return
			if(MsgLParam == -1){MsgLParam = *(long*)CurrentTarget;}
			if(MsgLParam == -2){MsgLParam = myId;}
			if(Agents[MsgLParam]==NULL && MsgLParam!=0){RESPONSE_INVALID;}
			UseHeroSkill(*(long*)(MySectionA->HeroesStruct() + 0x4), MsgWParam-1, MsgLParam);
			break;
		case 0x429: //Use hero 2 skill : No return
			if(MsgLParam == -1){MsgLParam = *(long*)CurrentTarget;}
			if(MsgLParam == -2){MsgLParam = myId;}
			if(Agents[MsgLParam]==NULL && MsgLParam!=0){RESPONSE_INVALID;}
			UseHeroSkill(*(long*)(MySectionA->HeroesStruct() + 0x28), MsgWParam-1, MsgLParam);
			break;
		case 0x42A: //Use hero 3 skill : No return
			if(MsgLParam == -1){MsgLParam = *(long*)CurrentTarget;}
			if(MsgLParam == -2){MsgLParam = myId;}
			if(Agents[MsgLParam]==NULL && MsgLParam!=0){RESPONSE_INVALID;}
			UseHeroSkill(*(long*)(MySectionA->HeroesStruct() + 0x4C), MsgWParam-1, MsgLParam);
			break;
		case 0x42B: //Write status about Bot Miss : No return
			switch(MsgWParam){
				case 0:
					WriteWhisper(L"Random interrupt miss is off", L"Miss Off");
					break;
				case 1:
					WriteWhisper(L"Random interrupt miss is on", L"Miss On");
					break;
			}
			break;
		case 0x42C: //Cancel movement : No return
			CancelAction();
			break;
		case 0x42D: //Write status about current tab : No return
			switch(MsgWParam){
				case 1:
					WriteWhisper(L"Switched to tab 1", L"Tab 1");
					break;
				case 2:
					WriteWhisper(L"Switched to tab 2", L"Tab 2");
					break;
			}
			break;
		case 0x42E: //Get ptr to name of agent : Return wchar_t*
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			pName = GetAgentName(MsgWParam);
			PostMessage((HWND)MsgLParam, 0x500, (WPARAM)pName, 0);
			break;
		case 0x42F: //Command hero 1 to location : No return
			memcpy(&MsgFloat, &MsgWParam, sizeof(float));
			memcpy(&MsgFloat2, &MsgLParam, sizeof(float));
			CommandHero(*(long*)(MySectionA->HeroesStruct() + 0x4), MsgFloat, MsgFloat2);
			break;
		case 0x430: //Command hero 2 to location : No return
			memcpy(&MsgFloat, &MsgWParam, sizeof(float));
			memcpy(&MsgFloat2, &MsgLParam, sizeof(float));
			CommandHero(*(long*)(MySectionA->HeroesStruct() + 0x28), MsgFloat, MsgFloat2);
			break;
		case 0x431: //Command hero 3 to location : No return
			memcpy(&MsgFloat, &MsgWParam, sizeof(float));
			memcpy(&MsgFloat2, &MsgLParam, sizeof(float));
			CommandHero(*(long*)(MySectionA->HeroesStruct() + 0x4C), MsgFloat, MsgFloat2);
			break;
		case 0x432: //Command all to location : No return
			memcpy(&MsgFloat, &MsgWParam, sizeof(float));
			memcpy(&MsgFloat2, &MsgLParam, sizeof(float));
			CommandAll(MsgFloat, MsgFloat2);
			break;
		case 0x433: //Change region and language : No return
			ChangeDistrict(MsgWParam, MsgLParam);
			break;
		case 0x434: //Send /resign to chat, effectively resigning : No return
			SendChat('/',"resign");
			break;
		case 0x435: //Send "Return to Outpost" packet : No return
			ReturnToOutpost();
			break;
		case 0x436: //Go to target : No return
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			GoAgent(MsgWParam);
			break;
		case 0x437: //Donate faction : No return
			DonateFaction(MsgWParam);
			break;
		case 0x438: //Set skillbar skill : No return
			SetSkillbarSkill(MsgWParam, MsgLParam);
			break;
		case 0x439: //Change second profession : No return
			if(MsgWParam < 1 || MsgWParam > 10){break;}
			ChangeSecondProfession(MsgWParam);
			break;
		case 0x43A: //Target next party member : No return
			TargetNextPartyMember();
			break;
		case 0x43B: //Target next foe : No return
			TargetNextFoe();
			break;
		case 0x43C: //Skip cinematic : No return
			SkipCinematic();
			break;

		//SectionA related commands
		case 0x440: //Check if map is loading : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, mapLoading, 0);
			break;
		case 0x441: //Get map id : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->MapId(), 0);
			break;
		case 0x442: //Get ping : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->Ping(), 0);
			break;
		case 0x443: //Check if logged in : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->LoggedIn(), 0);
			break;
		case 0x444: //Check if you're dead : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->Dead(), 0);
			break;
		case 0x445: //Get current and max balthazar faction : Return int/long & int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->CurrentBalthFaction(), MySectionA->MaxBalthFaction());
			break;
		case 0x446: //Get current and max kurzick faction : Return int/long & int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->CurrentKurzickFaction(), MySectionA->MaxKurzickFaction());
			break;
		case 0x447: //Get current and max luxon faction : Return int/long & int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->CurrentLuxonFaction(), MySectionA->MaxLuxonFaction());
			break;
		case 0x448: //Get current Treasure Title (credits to ddarek): Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->TitleTreasure(), 0);
			break;
		case 0x449: //Get current Lucky Title (credits to ddarek): Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->TitleLucky(), 0);
			break;
		case 0x44A: //Get current Unlucky Title (credits to ddarek): Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->TitleUnlucky(), 0);
			break;
		case 0x44B: //Get current Wisdom Title (credits to ddarek): Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->TitleWisdom(), 0);
			break;

		//Agent Related Commands
		case 0x450: //Check for agent existency : Return int/bool
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){
				PostMessage((HWND)MsgLParam, 0x500, 0, 0);
			}else{
				PostMessage((HWND)MsgLParam, 0x500, 1, 1);
			}
			break;
		case 0x451: //Get agent's primary and secondary profession : Return byte & byte
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->Primary, Agents[MsgWParam]->Secondary);
			break;
		case 0x452: //Get player number of agent : Return word
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->PlayerNumber, 0);
			break;
		case 0x453: //Get HP of agent : Return float
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			memcpy(&MsgInt, &Agents[MsgWParam]->HP, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x454: //Get rotation of agent in degrees : Return float
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgFloat = (float)(Agents[MsgWParam]->Rotation * 180 / 3.14159265358979323846);
			memcpy(&MsgInt, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x455: //Get agent's current skill : Return word
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->Skill, 0);
			break;
		case 0x456: //Get X,Y coords of agent : Return float & float
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			memcpy(&MsgInt, &Agents[MsgWParam]->X, sizeof(float));
			memcpy(&MsgInt2, &Agents[MsgWParam]->Y, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x457: //Get weapon speeds of agent (weapon attack speed, attack speed modifier) : Return float & float
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			memcpy(&MsgInt, &Agents[MsgWParam]->WeaponAttackSpeed, sizeof(float));
			memcpy(&MsgInt2, &Agents[MsgWParam]->AttackSpeedModifier, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x458: //Is agent in spirit range of me : Return int/long
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->InSpiritRange, 0);
			break;
		case 0x459: //Get team ID of agent (0 = none) : Return byte
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->TeamId, 0);
			break;
		case 0x45A: //Get agent's combat mode : Return byte
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->CombatMode, 0);
			break;
		case 0x45B: //Get agent's model mode : Return float
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			memcpy(&MsgInt, &Agents[MsgWParam]->ModelMode, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x45C: //Get agent's health pips : Return int
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			//memcpy(&MsgInt, &Agents[MsgWParam]->HPPips, sizeof(float));
			MsgInt = ((Agents[MsgWParam]->HPPips / 0.0038) > 0.0) ? floor((Agents[MsgWParam]->HPPips / 0.0038) + 0.5) : ceil((Agents[MsgWParam]->HPPips / 0.0038) - 0.5);//floor((Agents[MsgWParam]->HPPips / 0.0038) + 0.5);
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x45D: //Get agent's effect bit map : Return int/long
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->Effects, 0);
			break;
		case 0x45E: //Get agent's hex bit map : Return byte
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgInt = 0;
			if((Agents[MsgWParam]->Effects & 0x0800)) MsgInt += 1;
			if((Agents[MsgWParam]->Effects & 0x0400)) MsgInt += 1;
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x45F: //Get agent's model animation : Return int/dword
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->ModelAnimation, 0);
			break;
		case 0x460: //Get agent's energy - ONLY WORKS FOR YOURSELF! : Return float
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			memcpy(&MsgInt, &Agents[MsgWParam]->Energy, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x461: //Get pointer to agent : Return ptr
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, (WPARAM)Agents[MsgWParam], 0);
			break;
		case 0x462: //Get agent type : Return int/long
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->Type, 0);
			break;
		case 0x463: //Get agent level : Return byte
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->Level, 0);
			break;
		case 0x464: //Get agent's name properties : Return int/long
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->NameProperties, 0);
			break;
		case 0x465: //Get max agent id : Return unsigned int/dword
			PostMessage((HWND)MsgLParam, 0x500, maxAgent, 0);
			break;
		case 0x466: //Get nearest agent to yourself : Return int/long
			MsgInt = GetNearestAgentToAgent(myId);
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x467: //Get distance between agent and you : Return float
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgFloat = GetDistanceFromAgentToAgent(myId, MsgWParam);
			memcpy(&MsgInt, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x468: //Get nearest agent to agent : Return int/long
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgInt = GetNearestAgentToAgent(MsgWParam);
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x469: //Get distance from agent (set previously in MsgInt by 0x406 : 1) to agent (MsgWParam) : Return float
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			if(MsgInt < 1 || (unsigned int)MsgInt > maxAgent){RESPONSE_INVALID;}
			MsgFloat = GetDistanceFromAgentToAgent(MsgInt, MsgWParam);
			memcpy(&MsgInt, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x46A: //Get nearest agent to agent AND the distance between them : Return int/long & float
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgInt = GetNearestAgentToAgent(MsgWParam);
			MsgFloat = GetDistanceFromAgentToAgent(MsgWParam, MsgInt);
			memcpy(&MsgInt2, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x46B: //Get model state of agent : Return int/long
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->ModelState, 0);
			break;
		case 0x46C: //Check if agent is attacking : Return int/bool
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			if(Agents[MsgWParam]->ModelState == 0x60||
				Agents[MsgWParam]->ModelState == 0x440||
				Agents[MsgWParam]->ModelState == 0x460)
			{
				PostMessage((HWND)MsgLParam, 0x500, 1, 1);
			}else{
				PostMessage((HWND)MsgLParam, 0x500, 0, 0);
			}
			break;
		case 0x46D: //Check if agent is knocked down : Return int/bool
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			if(Agents[MsgWParam]->ModelState == 0x450){
				PostMessage((HWND)MsgLParam, 0x500, 1, 1);
			}else{
				PostMessage((HWND)MsgLParam, 0x500, 0, 0);
			}
			break;
		case 0x46E: //Check if agent is moving : Return int/bool
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			if(Agents[MsgWParam]->ModelState == 0x0C||
				Agents[MsgWParam]->ModelState == 0x4C||
				Agents[MsgWParam]->ModelState == 0xCC)
			{
				PostMessage((HWND)MsgLParam, 0x500, 1, 1);
			}else{
				PostMessage((HWND)MsgLParam, 0x500, 0, 0);
			}
			break;
		case 0x46F: //Check if agent is dead : Return int/bool
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			if(Agents[MsgWParam]->ModelState == 0x400 || Agents[MsgWParam]->HP == 0){
				PostMessage((HWND)MsgLParam, 0x500, 1, 1);
			}else{
				PostMessage((HWND)MsgLParam, 0x500, 0, 0);
			}
			break;
		case 0x470: //Check if agent is casting/using skill : Return int/bool
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			if(Agents[MsgWParam]->Skill != NULL){
				PostMessage((HWND)MsgLParam, 0x500, 1, 1);
			}else{
				PostMessage((HWND)MsgLParam, 0x500, 0, 0);
			}
			break;
		case 0x471: //Get agent by player number (and the corresponding TeamId) : Return int/long & byte
			MsgInt = GetFirstAgentByPlayerNumber(MsgWParam);
			if(Agents[MsgInt] == NULL)
				PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			else
				PostMessage((HWND)MsgLParam, 0x500, MsgInt, Agents[MsgInt]->TeamId);
			break;
		case 0x472: //Get agents allegiance and team : Return word & byte
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->Allegiance, Agents[MsgWParam]->TeamId);
			break;
		case 0x473: //Get nearest enemy (by TeamId) to agent and the distance between them : Return int/long & float
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgInt = GetNearestEnemyToAgent(MsgWParam);
			MsgFloat = GetDistanceFromAgentToAgent(MsgWParam, MsgInt);
			memcpy(&MsgInt2, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x474: //Check if agent is under attack from enemy melee (by TeamId) : Return int
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgInt = IsAttackedMelee(MsgWParam);
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x475: //Get nearest item to agent : Return int/long & float
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgInt = GetNearestItemToAgent(MsgWParam);
			MsgFloat = GetDistanceFromAgentToAgent(MsgWParam, MsgInt);
			memcpy(&MsgInt2, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x476: //Get nearest agent by player number to self : Return int/long & float
			MsgInt = GetNearestAgentByPlayerNumber(MsgWParam);
			if(Agents[MsgInt] == NULL){
				PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			}else{
				MsgFloat = GetDistanceFromAgentToAgent(myId, MsgInt);
				memcpy(&MsgInt2, &MsgFloat, sizeof(float));
				PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			}
			break;
		case 0x477: //Get current speed of agent : Return float
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgFloat = sqrt(pow(Agents[MsgWParam]->MoveX, 2) + pow(Agents[MsgWParam]->MoveY, 2));
			memcpy(&MsgInt, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x478: //Get nearest enemy to agent by allegiance and the distance between them : Return int/long & float
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgInt = GetNearestEnemyToAgentByAllegiance(MsgWParam);
			MsgFloat = GetDistanceFromAgentToAgent(MsgWParam, MsgInt);
			memcpy(&MsgInt2, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x479: //Get nearest alive enemy to agent and the distance between them : Return int/long & float
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgInt = GetNearestAliveEnemyToAgent(MsgWParam);
			MsgFloat = GetDistanceFromAgentToAgent(MsgWParam, MsgInt);
			memcpy(&MsgInt2, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x47A: //Get weapon type : Return int/long
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->WeaponType, 0);
			break;
		case 0x47B: //Get nearest signpost to agent : Return int/long & float
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgInt = GetNearestSignpostToAgent(MsgWParam);
			MsgFloat = GetDistanceFromAgentToAgent(MsgWParam, MsgInt);
			memcpy(&MsgInt2, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x47C: //Get nearest npc to agent by allegiance : Return int/long & float
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgInt = GetNearestNpcToAgentByAllegiance(MsgWParam);
			MsgFloat = GetDistanceFromAgentToAgent(MsgWParam, MsgInt);
			memcpy(&MsgInt2, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x47D: //Get nearest agent to coords : No return (use 0x47E to return)
			memcpy(&MsgFloat, &MsgWParam, sizeof(float));
			memcpy(&MsgFloat2, &MsgLParam, sizeof(float));
			TmpVariable = GetNearestAgentToCoords(MsgFloat, MsgFloat2);
			break;
		case 0x47E: //Get the values of MsgInt2 and TmpVariable : Return int/long & int/long
			PostMessage((HWND)MsgLParam, 0x500, MsgInt2, TmpVariable);
			break;
		case 0x47F: //Get nearest NPC to coords : No return (use 0x47E to return)
			memcpy(&MsgFloat, &MsgWParam, sizeof(float));
			memcpy(&MsgFloat2, &MsgLParam, sizeof(float));
			TmpVariable = GetNearestNPCToCoords(MsgFloat, MsgFloat2);
			break;
		case 0x480: //Get login number of agent : Return int/long
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->LoginNumber, 0);
			break;
		case 0x481: //Get number of agents with certain player/model number : Return int/long
			if(MsgWParam == NULL){break;}
			PostMessage((HWND)MsgLParam, 0x500, GetNumberOfAgentsByPlayerNumber(MsgWParam), 0);
			break;
		case 0x482: //Get number of alive enemy agents in total : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GetNumberOfAliveEnemyAgents(), 0);
			break;
		case 0x483: //Get next item in iteration and the distance to it : Return int/long & float
			MsgInt = GetNextItem(MsgWParam);
			if(MsgInt){
				MsgFloat = GetDistanceFromAgentToAgent(myId, MsgInt);
				memcpy(&MsgInt2, &MsgFloat, sizeof(float));
			}else{
				MsgInt2 = 0;
			}
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x484: //Get target of agent : Return int/long
			if(MsgWParam == -1){MsgWParam = *(long*)CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, AgentTargets[MsgWParam], 0);
			break;
		case 0x485: //Set Attribute (Attribute,Value)
			if(MsgLParam < 0 || MsgLParam > 12){break;}
			SetAttribute((dword)MsgWParam,(dword)MsgLParam);
			break;

		//Item related commands
		case 0x510: //Get gold : Return int/long & int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->MoneySelf(), MySectionA->MoneyStorage());
			break;
		case 0x511: //Get bag size : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager->GetBagSize(MsgWParam), 0);
			break;
		case 0x512: //Select bag to work with : No return
			CurrentBag = MsgWParam;
			break;
		case 0x513: //Get current bag item id and model id : Return int/long
			if(!CurrentBag){break;}
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager->GetItemId(CurrentBag, MsgWParam), MyItemManager->GetItemModelId(CurrentBag, MsgWParam));
			break;
		case 0x514: //Get first ID kit item id : Return int/long & int/long
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager->FindIdKit(), 0);
			break;
		case 0x515: //Identify item by indexes : No return
			MsgInt = MyItemManager->FindIdKit();
			MsgInt2 = MyItemManager->GetItemId(MsgWParam, MsgLParam);
			if(!MsgInt || !MsgInt2){break;}
			IdentifyItem(MsgInt, MsgInt2);
			break;
		case 0x516: //Identify item by item id : No return
			MsgInt = MyItemManager->FindIdKit();
			if(!MsgInt){break;}
			IdentifyItem(MsgInt, MsgWParam);
			break;
		case 0x517: //Deposit gold in storage : No return
			MsgInt = MySectionA->MoneySelf();
			MsgInt2 = MySectionA->MoneyStorage();
			if(MsgWParam == -1){
				if((MsgInt2 + MsgInt) > 1000000){ MsgInt = 1000000 - MsgInt2; }
				MsgInt2 += MsgInt;
				MsgInt = MySectionA->MoneySelf() - MsgInt;
			}else{
				MsgInt2 += MsgWParam;
				MsgInt -= MsgWParam;
			}
			ChangeGold(MsgInt, MsgInt2);
			break;
		case 0x518: //Withdraw gold from storage : No return
			MsgInt = MySectionA->MoneySelf();
			MsgInt2 = MySectionA->MoneyStorage();
			if(MsgWParam == -1){
				if((MsgInt2 - (100000 - MsgInt)) < 0){
					MsgInt += MsgInt2;
					MsgInt2 = 0;
				}else{
					MsgInt2 -= 100000 - MsgInt;
					MsgInt += 100000;
				}
			}else{
				MsgInt2 -= MsgWParam;
				MsgInt += MsgWParam;
			}
			ChangeGold(MsgInt, MsgInt2);
			break;
		case 0x519: //Sell item by indexes : No return
			MsgInt = MyItemManager->GetItemId(MsgWParam, MsgLParam);
			if(!SellSessionId || !MsgInt){break;}
			SellItem(MsgInt);
			break;
		case 0x51A: //Sell item by item id : No return
			if(!SellSessionId){break;}
			SellItem(MsgWParam);
			break;
		case 0x51B: //Buy ID kit : No return
			if(!MySectionA->MerchantItems()){break;}
			BuyItem(*(long*)(MySectionA->MerchantItems() + 0x10), 1, 100);
			break;
		case 0x51C: //Buy superior ID kit : No return
			if(!MySectionA->MerchantItems()){break;}
			BuyItem(*(long*)(MySectionA->MerchantItems() + 0x14), 1, 500);
			break;
		case 0x51D: //Prepare MoveItem by setting item id (internal) : No return
			if(MsgWParam && MsgLParam){
				MoveItemId = MyItemManager->GetItemId(MsgWParam, MsgLParam);
			}else{
				MoveItemId = MsgWParam;
			}
			break;
		case 0x51E: //Move the item specified by 0x520 : No return
			if(!MoveItemId){RESPONSE_INVALID;}
			MoveItem(MoveItemId, MyItemManager->GetBagPtr(MsgWParam)->id, (MsgLParam - 1));
			break;
		case 0x51F: //Get current bag item rarity and quantity : Return byte & byte
			if(!CurrentBag){break;}
			if(!MyItemManager->GetItemPtr(CurrentBag, MsgWParam)){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager->GetItemPtr(CurrentBag, MsgWParam)->extraItemInfo->rarity,
				MyItemManager->GetItemPtr(CurrentBag, MsgWParam)->quantity);
			break;
		case 0x520: //Use item by indexes : No return
			MsgInt = MyItemManager->GetItemId(MsgWParam, MsgLParam);
			if(!MsgInt){break;}
			UseItem(MsgInt);
			break;
		case 0x521: //Use item by item id : No return
			UseItem(MsgWParam);
			break;
		case 0x522: //Drop item by indexes : No return
			if(MyItemManager->GetItemPtr(MsgWParam, MsgLParam)){
				DropItem(MyItemManager->GetItemId(MsgWParam, MsgLParam),
					MyItemManager->GetItemPtr(MsgWParam, MsgLParam)->quantity);
			}
			break;
		case 0x523: //Drop item by id and specifying amount : No return
			if(MsgLParam == -1 && MyItemManager->GetItemPtr(MsgWParam)){
				MsgLParam = MyItemManager->GetItemPtr(MsgWParam)->quantity;
			}
			DropItem(MsgWParam, MsgLParam);
			break;
		case 0x524: //Accept all unclaimed items : No return
			if(!MyItemManager->GetBagPtr(7)){break;}
			AcceptAllItems(MyItemManager->GetBagPtr(7)->id);
			break;
		case 0x525: //Get current bag item last modifier and customized : Return byte & wchar_t*
			if(!CurrentBag){break;}
			if(!MyItemManager->GetItemPtr(CurrentBag, MsgWParam)){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager->GetItemPtr(CurrentBag, MsgWParam)->extraItemInfo->lastModifier,
				(LPARAM)MyItemManager->GetItemPtr(CurrentBag, MsgWParam)->customized);
			break;
		case 0x526: //Find item by item model id : Return int/long
			if(MsgWParam==NULL){break;}
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager->GetItemByModelId(MsgWParam), 0);
			break;
		case 0x527: //Find next empty inventory/storage slot : Return int/long & int/long
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager->FindEmptySlot(MsgWParam, 1), MyItemManager->FindEmptySlot(MsgWParam, 2));
			break;
		case 0x528: //Find next gold item in inventory/storage : Return int/long
			if(MsgWParam==NULL){MsgWParam = 15;}
			MsgInt = MyItemManager->FindNextGoldItem(MsgWParam);
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MyItemManager->GetItemPtr(MsgInt)->id);
			break;
		case 0x529: //Get item position by item id : Return int/long & int/long
			if(MsgWParam==NULL){break;}
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager->GetItemPositionByItemId(MsgWParam, 1), MyItemManager->GetItemPositionByItemId(MsgWParam, 2));
			break;
		case 0x52A: //Get item position by model id : Return int/long & int/long
			if(MsgWParam==NULL){break;}
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager->GetItemPositionByModelId(MsgWParam, 1), MyItemManager->GetItemPositionByModelId(MsgWParam, 2));
			break;
		case 0x52B: //Get item position by rarity : Return int/long & int/long
			if(MsgWParam=NULL){break;}
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager->GetItemPositionByRarity(MsgWParam, 1), MyItemManager->GetItemPositionByRarity(MsgWParam, 2));
			break;
		case 0x52C: //Get item model id by item id : Return int/long
			if(MsgWParam==NULL){break;}
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager->GetItemModelId(MsgWParam), 0);
			break;
		case 0x52D: //Get item rarity and quantity by item id : Return byte & byte
			if(!MyItemManager->GetItemPtr(MsgWParam)){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager->GetItemPtr(MsgWParam)->extraItemInfo->rarity,
				MyItemManager->GetItemPtr(MsgWParam)->quantity);
			break;
		case 0x52E: //Get item last modifier and customized : Return byte & wchar_t*
			if(!MyItemManager->GetItemPtr(MsgWParam)){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager->GetItemPtr(MsgWParam)->extraItemInfo->lastModifier,
				(LPARAM)MyItemManager->GetItemPtr(MsgWParam)->customized);
			break;
		case 0x52F: //Equip item by indexes : No return
			if(MyItemManager->GetItemPtr(MsgWParam, MsgLParam)){
				EquipItem(MyItemManager->GetItemId(MsgWParam, MsgLParam));
			}
			break;
		case 0x530: //Equip item by item id : No return
			EquipItem(MsgWParam);
			break;
		case 0x531: //Salvage Item : No return
			if(MsgWParam==NULL){RESPONSE_INVALID;}
			if(MsgLParam==NULL){
				SalvageItem(MsgWParam, MyItemManager->FindSalvageKit());
			}else{
				SalvageItem(MyItemManager->GetItemId(MsgWParam, MsgLParam), MyItemManager->FindSalvageKit());
			}
			break;
		case 0x532: //Find salvage kit : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager->FindSalvageKit(), 0);
			break;
		case 0x533: //Buy item by index and cost : No return
			if(!MySectionA->MerchantItems()){break;}
			if(MsgWParam < 1 || MsgWParam > MySectionA->MerchantItemsSize()){break;}
			BuyItem(*(long*)(MySectionA->MerchantItems() + ((MsgWParam - 1) * 4)), 1, MsgLParam);
			break;

		//Title related commands
		case 0x550: //Get current Sunspear Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->TitleSunspear(), 0);
			break;
		case 0x551: //Get current Lightbringer Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->TitleLightbringer(), 0);
			break;
		case 0x552: //Get current Vanguard Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->TitleVanguard(), 0);
			break;
		case 0x553: //Get current Norn Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->TitleNorn(), 0);
			break;
		case 0x554: //Get current Asura Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->TitleAsura(), 0);
			break;
		case 0x555: //Get current Deldrimor Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->TitleDeldrimor(), 0);
			break;
		case 0x556: //Get current North Mastery Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->TitleNorthMastery(), 0);
			break;
		case 0x557: //Get current Drunkard Title : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->TitleDrunkard(), 0);
			break;
		case 0x558: //Get current Sweet Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->TitleSweet(), 0);
			break;
		case 0x559: //Get current Party Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->TitleParty(), 0);
			break;
		case 0x55A: //Get current Commander Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->TitleCommander(), 0);
			break;
		case 0x55B: //Get current Luxon Title Track: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->TitleTrackLuxon(), 0);
			break;
		case 0x55C: //Get current Kurzick Title Track: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->TitleTrackKurzick(), 0);
			break;

		//More general commands
		case 0x580: //Add hero to party: No Return
			AddHero(MsgWParam);
			break;
		case 0x581: //Kick hero from party : No Return
			KickHero(MsgWParam);
			break;
		case 0x582: //Switch between NM and HM: No Return
			SwitchMode(MsgWParam);
			break;
		case 0x583: //Add henchman to party : No Return
			AddNpc(MsgWParam);
			break;
		case 0x584: //Kick henchman from party : No Return
			KickNpc(MsgWParam);
			break;
		case 0x585: //Travel to Guild Hall : No return
			TravelGH();
			break;
		case 0x586: //Leave Guild Hall : No return
			LeaveGH();
			break;
		case 0x587: //Init Map Load : No return
			FinishedLoading = false;
			break;
		case 0x588: //Map Is Loaded : Return bool/int
			if(FinishedLoading == true){
				PostMessage((HWND)MsgLParam, 0x500, 1, 1);
			}else{
				PostMessage((HWND)MsgLParam, 0x500, 0, 0);
			}
			break;
		case 0x589: //Get coordinates of MapOverlay by index : Return float & float
			if(MsgWParam < 1 || MsgWParam > (WPARAM)(MySectionA->MapOverlaySize() - 1)){break;}
			memcpy(&MsgInt, &MySectionA->MapOverlayPointer()[MsgWParam].X, sizeof(float));
			memcpy(&MsgInt2, &MySectionA->MapOverlayPointer()[MsgWParam].Y, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x58A: //Get MapOverlay option and model id by index : Return int/long
			if(MsgWParam < 1 || MsgWParam > (WPARAM)(MySectionA->MapOverlaySize() - 1)){break;}
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->MapOverlayPointer()[MsgWParam].option,
				MySectionA->MapOverlayPointer()[MsgWParam].modelId);
			break;
		case 0x58B: //Get nearest MapOverlay to coordinates : No return (use 0x47E to return)
			memcpy(&MsgFloat, &MsgWParam, sizeof(float));
			memcpy(&MsgFloat2, &MsgLParam, sizeof(float));
			TmpVariable = GetNearestMapOverlayToCoords(MsgFloat, MsgFloat2);
			break;
		case 0x58C: //Get party info : Return in WM_COPYDATA
			SendPartyInfo((HWND)MsgLParam, MsgWParam, PartyTeamSize);
			break;
		case 0x58D: //Clear PacketQueue : Return int/long
			if(WaitForSingleObject(PacketMutex, 200) == WAIT_TIMEOUT) break;
			MsgInt = PacketQueue.size();
			PacketQueue.clear();
			ReleaseMutex(PacketMutex);
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x58E: //Set hero combat mode : No return
			switch(MsgWParam){
				case 1:
					MsgWParam = *(long*)(MySectionA->HeroesStruct() + 0x4); break;
				case 2:
					MsgWParam = *(long*)(MySectionA->HeroesStruct() + 0x28); break;
				case 3:
					MsgWParam = *(long*)(MySectionA->HeroesStruct() + 0x4C); break;
			}
			SetHeroMode(MsgWParam, MsgLParam);
			break;
		case 0x58F: //Check if you have a certain quest in your log : Return int/long
			if(MsgWParam==-1){MsgWParam = MySectionA->ActiveQuest();}
			if(MsgWParam==NULL){break;}
			MsgInt = 0;
			MsgInt2 = 0;
			for(MsgLoop = 0;MsgLoop < MySectionA->QuestLogSize();MsgLoop++){
				if(MySectionA->QuestLogPointer()[MsgLoop].Id == MsgWParam){
					MsgInt = MsgWParam;
					MsgInt2 = MySectionA->QuestLogPointer()[MsgLoop].LogState;
					break;
				}
			}
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x590: //Get coordinates of quest marker : Return float & float
			if(MsgWParam==-1){MsgWParam = MySectionA->ActiveQuest();}
			if(MsgWParam==NULL){break;}
			MsgFloat = 0;
			MsgFloat2 = 0;
			for(MsgLoop = 0;MsgLoop < MySectionA->QuestLogSize();MsgLoop++){
				if(MySectionA->QuestLogPointer()[MsgLoop].Id == MsgWParam){
					MsgFloat = MySectionA->QuestLogPointer()[MsgLoop].X;
					MsgFloat2 = MySectionA->QuestLogPointer()[MsgLoop].Y;
					break;
				}
			}
			memcpy(&MsgInt, &MsgFloat, sizeof(float));
			memcpy(&MsgInt2, &MsgFloat2, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x591: //Get id of currently active quest : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MySectionA->ActiveQuest(), 0);
			break;
		case 0x592: //Abandon specified quest : No return
			if(MsgWParam==-1){MsgWParam = MySectionA->ActiveQuest();}
			if(MsgWParam==NULL){break;}
			AbandonQuest(MsgWParam);
			break;
		case 0x593: //Set team size to use with SendPartyInfo
			if(MsgWParam < 0 || MsgWParam > 8){break;}
			PartyTeamSize = MsgWParam;
			break;
	}
	
	_asm {
		MOV EAX,1
		POP EDI
		POP ESI
		POP EBX
		MOV ESP,EBP
		POP EBP
		RETN 0x10
	}
}

void ReloadSkillbar(){
	MySkillbar = (Skillbar*)(MySectionA->SkillbarPtr());
}

void SellItem(long itemId){
	SellItemStruct* pSell = new SellItemStruct;
	pSell->sessionId = SellSessionId;
	pSell->itemId = itemId;

	_asm {
		MOV ECX,pSell
		CALL SellItemFunction
	}

	delete[] pSell;
}

void BuyItem(long id, long quantity, long value){
	long* itemQuantity = &quantity;
	long* itemId = &id;

	_asm {
		PUSH itemQuantity
		PUSH itemId
		PUSH 1
		PUSH 0
		PUSH 0
		PUSH 0
		PUSH 0
		MOV EDX,value
		MOV ECX,1
		CALL BuyItemFunction
	}
}

void SendPartyInfo(HWND hwndReceiver, long teamId, long teamSize){
	PartyInfo* PtInfo = new PartyInfo;
	memset(PtInfo, '\0', sizeof(PartyInfo));

	__try {
		PtInfo->HwndReceiver = hwndReceiver;
		PtInfo->TeamId = teamId;
		PtInfo->TeamSize = teamSize;

		TeamAgents.clear();
		for(unsigned int i = 1;i < maxAgent;i++){
			if(TeamAgents.size() == teamSize){break;}
			if(Agents[i] == NULL){continue;}
			if(Agents[i]->TeamId == teamId && Agents[i]->LoginNumber != 0){TeamAgents.push_back(i);}
		}

		for(int i = 0;i < 8;i++)
			PtInfo->Players[i].AgentId = 0;

		if(TeamAgents.size() == 0)
			PtInfo->TeamSize = 0;

		long plNum;
		for(unsigned int i = 0;i < TeamAgents.size();i++){
			if(Agents[TeamAgents[i]] == NULL){continue;}

			plNum = Agents[TeamAgents[i]]->PlayerNumber - (PtInfo->TeamSize * (teamId - 1)) - 1;
			if(plNum < 0){plNum += PtInfo->TeamSize;}
			if(plNum > 7){continue;}

			PtInfo->Players[plNum].AgentId = TeamAgents[i];
			PtInfo->Players[plNum].Effects = Agents[TeamAgents[i]]->Effects;
			PtInfo->Players[plNum].Hex = 0;
			if((Agents[TeamAgents[i]]->Effects & 0x0800)) PtInfo->Players[plNum].Hex += 1;
			if((Agents[TeamAgents[i]]->Effects & 0x0400)) PtInfo->Players[plNum].Hex += 1;
			PtInfo->Players[plNum].X = Agents[TeamAgents[i]]->X;
			PtInfo->Players[plNum].Y = Agents[TeamAgents[i]]->Y;
			PtInfo->Players[plNum].HP = Agents[TeamAgents[i]]->HP;
			PtInfo->Players[plNum].NamePtr = GetAgentName(TeamAgents[i]);
			PtInfo->Players[plNum].Primary = Agents[TeamAgents[i]]->Primary;
			PtInfo->Players[plNum].Secondary = Agents[TeamAgents[i]]->Secondary;
			PtInfo->Players[plNum].Target = AgentTargets[TeamAgents[i]];
			PtInfo->Players[plNum].Skill = Agents[TeamAgents[i]]->Skill;
			PtInfo->Players[plNum].Weapon = Agents[TeamAgents[i]]->WeaponType;
		}

		if(WaitForSingleObject(PartyMutex, 1000) != WAIT_TIMEOUT){
			PartyInfoQueue.push_back(PtInfo);
			ReleaseMutex(PartyMutex);
		}else{
			delete PtInfo;
		}
	}
	__except(1) {
		delete PtInfo;
		return;
	}
}

void WriteWhisper(const wchar_t* chatMsg, const wchar_t* chatName){
	 _asm {
		MOV EAX,chatMsg
		MOV EDX,chatName
		MOV ECX,0
		PUSH EAX
		CALL WriteWhisperStart
		MOV EAX,1
	}
}

wchar_t* GetAgentName(int agentId){
	__try {
		wchar_t* NamePtr = ReadPtrChain<wchar_t*>(MySectionA->BasePointer(), 0x18, 0x2C, 0x794,
			(Agents[agentId]->LoginNumber * 76) + 0x28);
		return NamePtr;
	}
	__except(1) {
		return NULL;
	}
}

void TargetNearestFoe(){
	_asm {
		MOV EBX,0
		MOV EAX,TargetFunctions
		CALL EAX
	}
}

void TargetNearestAlly(){
	_asm {
		MOV EBX,0
		MOV EAX,TargetFunctions
		ADD EAX,0x1D
		CALL EAX
	}
}

void TargetNearestItem(){
	_asm {
		MOV EBX,0
		MOV EAX,TargetFunctions
		ADD EAX,0x3A
		CALL EAX
	}
}

void TargetCalledTarget(){
	_asm {
		MOV EBX,0
		MOV EAX,TargetFunctions
		ADD EAX,0x115
		CALL EAX
	}
}

void TargetNextPartyMember(){
	_asm {
		MOV ECX,0
		MOV EAX,TargetFunctions
		ADD EAX,0x63
		CALL EAX
	}
}

void TargetNextFoe(){
	_asm {
		MOV EAX,TargetFunctions
		ADD EAX,0x57
		CALL EAX
	}
}

void GoAgent(long agentId){
	byte* pGoAgent = (byte*)((TargetFunctions - 0x616) + *(dword*)(TargetFunctions - 0x616) + 4);

	_asm {
		MOV ECX,agentId
		MOV EDX,1
		CALL pGoAgent
	}
}

void UseHeroSkill(long HeroId, long SkillNumber, long Target){
	_asm {
		MOV EDX,SkillNumber
		MOV ECX,HeroId
		PUSH Target
		CALL HeroSkillFunction
	}
}

void UseSkillNew(long SkillId, long Target, long Event){
	SkillType* tmpSkillType = (SkillType*)(SkillTypeBase + (MsgInt * 160));
	if(tmpSkillType == NULL){return;}
	if(tmpSkillType->Type == GW_SKILL_TYPE_ATTACKS){
		UseAttackSkill(MsgInt, MsgLParam, Event);
	}else{
		UseSkill(MsgInt, MsgLParam, Event);
	}
}

bool CompareAccName(wchar_t* cmpName){
	if(wcscmp(cmpName, MySectionA->Email()) == NULL)
		return true;
	else
		return false;
}

bool CompareCharName(wchar_t* cmpName){
	if(wcscmp(cmpName, MySectionA->Name()) == NULL)
		return true;
	else
		return false;
}

void ChangeMaxZoom(float fZoom){
	DWORD dwOldProtection;
	VirtualProtect(MaxZoomStill, sizeof(float), PAGE_EXECUTE_READWRITE, &dwOldProtection);
	memcpy(MaxZoomStill, &fZoom, sizeof(float));
	VirtualProtect(MaxZoomStill, sizeof(float), dwOldProtection, NULL);
	VirtualProtect(MaxZoomMobile, sizeof(float), PAGE_EXECUTE_READWRITE, &dwOldProtection);
	memcpy(MaxZoomMobile, &fZoom, sizeof(float));
	VirtualProtect(MaxZoomStill, sizeof(float), dwOldProtection, NULL);
}

void SetEngineHook(int Enable){
	if(Enable){
		DWORD dwOldProtection;
		VirtualProtect(EngineStart, 2, PAGE_EXECUTE_READWRITE, &dwOldProtection);
		memset(EngineStart, 0x90, 2);
		VirtualProtect(EngineStart, 2, dwOldProtection, NULL);
		WriteJMP(EngineStart+0x13, (byte*)EngineHook);
	}else{
		DWORD dwOldProtection;
		VirtualProtect(EngineStart, 32, PAGE_EXECUTE_READWRITE, &dwOldProtection);
		memcpy(EngineStart, EngineHookSave, 32);
		VirtualProtect(EngineStart, 32, dwOldProtection, NULL);
	}
}

void SendPacket(CPacket* pak){
	if(WaitForSingleObject(PacketMutex, 1000) == WAIT_TIMEOUT) return;
	PacketQueue.push_back(pak);
	ReleaseMutex(PacketMutex);
}

template <typename T> T ReadPtrChain(dword pBase, long pOffset1, long pOffset2, long pOffset3, long pOffset4){
	dword pRead = pBase;
	if(pRead == NULL){return 0;}

	if(pOffset1){pRead = *(dword*)(pRead + pOffset1);}
	if(pRead == NULL){return 0;}

	if(pOffset2){pRead = *(dword*)(pRead + pOffset2);}
	if(pRead == NULL){return 0;}

	if(pOffset3){pRead = *(dword*)(pRead + pOffset3);}
	if(pRead == NULL){return 0;}

	if(pOffset4){pRead = *(dword*)(pRead + pOffset4);}
	if(pRead == NULL){return 0;}

	return (T)pRead;
}

void SendPacketQueueThread(){
	while(true){
		Sleep(10);

		if(WaitForSingleObject(PacketMutex, 100) == WAIT_TIMEOUT) continue;
		if(PacketQueue.size() < 1 || mapLoading == 2) goto nextLoop;
		if(MySectionA->LoggedIn() != 1 && mapLoading != 1) goto nextLoop;

		{
			std::vector<CPacket*>::iterator itrPak = PacketQueue.begin();
			CPacket* CurPacket = *itrPak;

			//dword testValue = 0x99;
			//_asm {
			//	MOV ECX, FlagLocation
			//	MOV ECX, DWORD PTR DS:[ECX]
			//	MOVZX ECX, BYTE PTR DS:[ECX+8]
			//	MOV testValue, ECX
			//}
			//if((testValue & 1)){
				{
					byte* buffer = CurPacket->Buffer;
					dword psize = CurPacket->Size;
				
					_asm {
						MOV EAX, PacketLocation
						MOV EAX, DWORD PTR DS:[EAX]
						MOV ECX, EAX
						MOV EDX, psize
						PUSH buffer
						CALL PacketSendFunction
					}
				}

				delete [] CurPacket->Buffer;
				delete CurPacket;
			//}

			PacketQueue.erase(itrPak);
		}

nextLoop:
		ReleaseMutex(PacketMutex);
	}
}

void SkillLogQueueThread(){
	COPYDATASTRUCT SkillLogCDS;
	COPYDATASTRUCT PartyInfoCDS;
	LoggedSkillStruct SkillInfo;

	SkillLogCDS.dwData = 1;
	SkillLogCDS.lpData = &SkillInfo;
	SkillLogCDS.cbData = sizeof(LoggedSkillStruct);

	PartyInfoCDS.dwData = 2;
	PartyInfoCDS.cbData = sizeof(PartyInfo);

	wchar_t* sWindowText = new wchar_t[50];
	dword tTicks = 0;

	while(true){
		Sleep(10);
		if(SkillLogQueue.size() > 0 && LogSkills){
			SkillInfo.AgentId = SkillLogQueue.front().AgentId;
			
			if(Agents[SkillInfo.AgentId] != NULL){
				SkillInfo.TeamId = Agents[SkillInfo.AgentId]->TeamId;
				SkillInfo.Allegiance = Agents[SkillInfo.AgentId]->Allegiance;
			}else{
				SkillInfo.TeamId = 0;
				SkillInfo.Allegiance = 0;
			}

			SkillInfo.Distance = GetDistanceFromAgentToAgent(myId, SkillInfo.AgentId);
			SkillInfo.MyId = myId;
			SkillInfo.SkillId = SkillLogQueue.front().Skill;
			SkillInfo.Activation = SkillLogQueue.front().Activation;
			SkillInfo.Ping = MySectionA->Ping();
			SkillInfo.TargetId = AgentTargets[SkillInfo.AgentId];

			SendMessage(ScriptHwnd, WM_COPYDATA, 0, (LPARAM)(LPVOID)&SkillLogCDS);

			SkillLogQueue.erase(SkillLogQueue.begin()); //Remove handled skill from queue
		}else{
			SkillLogQueue.clear();
		}

		if(SkillCancelQueue.size() > 0 && LogSkills){
			PostMessage(ScriptHwnd, 0x501, SkillCancelQueue.front().AgentId, SkillCancelQueue.front().Skill);
			SkillCancelQueue.erase(SkillCancelQueue.begin());
		}else{
			SkillCancelQueue.clear();
		}

		if(WaitForSingleObject(PartyMutex, 50) != WAIT_TIMEOUT){
			if(PartyInfoQueue.size() > 0){
				PartyInfoCDS.lpData = PartyInfoQueue.front();
				SendMessage(PartyInfoQueue.front()->HwndReceiver, WM_COPYDATA, 0, (LPARAM)(LPVOID)&PartyInfoCDS);
				delete PartyInfoQueue.front();
				PartyInfoQueue.erase(PartyInfoQueue.begin());
			}
			ReleaseMutex(PartyMutex);
		}

		if(*(HWND*)WinHandle){
			if((GetTickCount() - tTicks) > 5000){
				tTicks = GetTickCount();

				if(!MySectionA->Name()[0]){
					SetWindowTextW(*(HWND*)WinHandle, L"Guild Wars");
				}else{
					swprintf(sWindowText, 50, L"Guild Wars - %s", MySectionA->Name());
					SetWindowTextW(*(HWND*)WinHandle, sWindowText);
				}
			}
		}
	}
}

void FindOffsets(){
	byte* start = (byte*)0x00401000;
	byte* end = (byte*)0x00900000;

	byte PacketSendCode[] = { 0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x2C, 0x53, 0x56, 0x57, 0x8B,
		0xF9, 0x85 };

	byte BaseOffsetCode[] = { 0x56, 0x33, 0xF6, 0x3B, 0xCE, 0x74, 0x0E, 0x56, 0x33, 0xD2 };

	byte AgentBaseCode[] = { 0x56, 0x8B, 0xF1, 0x3B, 0xF0, 0x72, 0x04 };

	byte MessageHandlerCode[] = { 0x8B, 0x86, 0xA4, 0x0C, 0x00, 0x00, 0x85, 0xC0, 0x0F };

	byte SkillLogCode[] = { 0x8B, 0x46, 0x10, 0x5F, 0x40 };

	byte MapIdLocationCode[] = { 0xB0, 0x7F, 0x8D, 0x55 };

	byte WriteWhisperCode[] = { 0x55, 0x8B, 0xEC, 0x51, 0x53, 0x89, 0x4D, 0xFC, 0x8B, 0x4D,
		0x08, 0x56, 0x57, 0x8B };

	byte TargetFunctionsCode[] = { 0xBA, 0x01, 0x00, 0x00, 0x00, 0xB9, 0x00, 0x80, 0x00, 0x00,
		0xE8 };

	byte HeroSkillFunctionCode[] = { 0x5E, 0xC3, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		0x90, 0x90, 0x55, 0x8B, 0xEC, 0x8B, 0x45, 0x08, 0x50 };

	byte ClickToMoveCode[] = { 0x3D, 0xD3, 0x01, 0x00, 0x00, 0x74 };

	byte BuildNumberCode[] = { 0x8D, 0x85, 0x00, 0xFC, 0xFF, 0xFF, 0x8D };

	byte ChangeTargetCode[] = { 0x33, 0xC0, 0x3B, 0xDA, 0x0F, 0x95, 0xC0, 0x33 };

	byte MaxZoomStillCode[] = { 0x3B, 0x44, 0x8B, 0xCB };

	byte MaxZoomMobileCode[] = { 0x50, 0xEB, 0x11, 0x68, 0x00, 0x80, 0x3B, 0x44, 0x8B, 0xCE };

	byte SkillCancelCode[] = { 0x85, 0xC0, 0x74, 0x1D, 0x6A, 0x00, 0x6A, 0x42 };

	//byte AgentNameCode[] = { 0x57, 0x8B, 0x14, 0x81, 0x8B, 0x82, 0x04, 0x00, 0x00, 0x00,
	//	0x8B, 0x78, 0x2C, 0xE8 };

	byte SellSessionCode[] = { 0x33, 0xD2, 0x8B, 0xCF, 0xC7, 0x46, 0x0C };

	byte SellItemCode[] = { 0x8B, 0x46, 0x0C, 0x8D, 0x7E, 0x0C, 0x85 };

	byte BuyItemCode[] = { 0x64, 0x8B, 0x0D, 0x2C, 0x00, 0x00, 0x00, 0x89, 0x55, 0xFC,
		0x8B };

	byte PingLocationCode[] = { 0x90, 0x8D, 0x41, 0x24, 0x8B, 0x49, 0x18, 0x6A, 0x30 };

	byte LoggedInLocationCode[] = { 0x85, 0xC0, 0x74, 0x11, 0xB8, 0x07 };

	byte NameLocationCode[] = { 0x6A, 0x14, 0x8D, 0x96, 0xBC };

	byte DeadLocationCode[] = { 0x85, 0xC0, 0x74, 0x11, 0xB8, 0x02 };

	byte BasePointerLocationCode[] = { 0x85, 0xC9, 0x74, 0x3D, 0x8B, 0x46 };

	byte DialogCode[] = { 0x55, 0x8B, 0xEC, 0x8B, 0x41, 0x08, 0xA8, 0x01, 0x75, 0x24 };

	byte EngineCode[] = { 0x53, 0x56, 0xDF, 0xE0, 0xF6, 0xC4, 0x41 };

	byte SkillTypeBaseCode[] = { 0x8D, 0x04, 0xB6, 0x5E, 0xC1, 0xE0, 0x05, 0x05 };

	byte WinHandleCode[] = { 0x56, 0x8B, 0xF1, 0x85, 0xC0, 0x89, 0x35 };

	byte LoadFinishedCode[] = { 0x89, 0x4D, 0xD8, 0x8B, 0x4D, 0x0C, 0x89, 0x55, 0xDC, 0x8B };

	byte TargetLogCode[] = { 0x53, 0x89, 0x4D, 0xF4, 0x56, 0x64 };
	
	byte SetAttrisCode[] = { 0x8B, 0x56, 0x08, 0x8D, 0x46, 0x3C, 0x8D, 0x4E, 0x0C, 0x50, 0x51, 0x8B, 0xCF, 0xE8 };

	while(start!=end){
		if(!memcmp(start, AgentBaseCode, sizeof(AgentBaseCode))){
			AgentArrayPtr = (byte*)(*(dword*)(start+0xC));
			AgentArrayMaxPtr = AgentArrayPtr+0x8;
			CurrentTarget = AgentArrayPtr-0x500;
		}
		if(!memcmp(start, BaseOffsetCode, sizeof(BaseOffsetCode))){
			BaseOffset = start;
		}
		if(!memcmp(start, PacketSendCode, sizeof(PacketSendCode))){
			PacketSendFunction = start;
		}
		if(!memcmp(start, MessageHandlerCode, sizeof(MessageHandlerCode))){
			MessageHandlerStart = start-0x95;
			MessageHandlerReturn = MessageHandlerStart+9;
		}
		if(!memcmp(start, SkillLogCode, sizeof(SkillLogCode))){
			SkillLogStart = start;
			SkillLogReturn = SkillLogStart+8;
		}
		if(!memcmp(start, MapIdLocationCode, sizeof(MapIdLocationCode))){
			MapIdLocation = (byte*)(*(dword*)(start+0x46));
		}
		if(!memcmp(start, WriteWhisperCode, sizeof(WriteWhisperCode))){
			WriteWhisperStart = start;
		}
		if(!memcmp(start, TargetFunctionsCode, sizeof(TargetFunctionsCode))){
			TargetFunctions = start;
		}
		if(!memcmp(start, HeroSkillFunctionCode, sizeof(HeroSkillFunctionCode))){
			HeroSkillFunction = start+0xC;
		}
		if(!memcmp(start, ClickToMoveCode, sizeof(ClickToMoveCode))){
			ClickToMoveFix = start;
		}
		if(!memcmp(start, BuildNumberCode, sizeof(BuildNumberCode))){
			BuildNumber = start+0x53;
		}
		if(!memcmp(start, ChangeTargetCode, sizeof(ChangeTargetCode))){
			ChangeTargetFunction = start-0x78;
		}
		if(!memcmp(start, MaxZoomStillCode, sizeof(MaxZoomStillCode))){
			MaxZoomStill = start-2;
		}
		if(!memcmp(start, MaxZoomMobileCode, sizeof(MaxZoomMobileCode))){
			MaxZoomMobile = start+4;
		}
		if(!memcmp(start, SkillCancelCode, sizeof(SkillCancelCode))){
			SkillCancelStart = start-0xE;
			SkillCancelReturn = SkillCancelStart+7;
		}
		//if(!memcmp(start, AgentNameCode, sizeof(AgentNameCode))){
		//	AgentNameFunction = start-0x16;
		//}
		if(!memcmp(start, SellSessionCode, sizeof(SellSessionCode))){
			SellSessionStart = start-0x48;
			SellSessionReturn = SellSessionStart+9;
		}
		if(!memcmp(start, SellItemCode, sizeof(SellItemCode))){
			SellItemFunction = start-8;
		}
		if(!memcmp(start, BuyItemCode, sizeof(BuyItemCode))){
			BuyItemFunction = start-0xE;
		}
		if(!memcmp(start, PingLocationCode, sizeof(PingLocationCode))){
			PingLocation = (byte*)(*(dword*)(start-9));
		}
		if(!memcmp(start, LoggedInLocationCode, sizeof(LoggedInLocationCode))){
			LoggedInLocation = (byte*)(*(dword*)(start-4) + 4);
		}
		if(!memcmp(start, NameLocationCode, sizeof(NameLocationCode))){
			NameLocation = (byte*)(*(dword*)(start+9));
			EmailLocation = (byte*)(*(dword*)(start-9));
		}
		if(!memcmp(start, DeadLocationCode, sizeof(DeadLocationCode))){
			DeadLocation = (byte*)(*(dword*)(start-4));
		}
		if(!memcmp(start, BasePointerLocationCode, sizeof(BasePointerLocationCode))){
			BasePointerLocation = (byte*)(*(dword*)(start-4));
		}
		if(!memcmp(start, DialogCode, sizeof(DialogCode))){
			DialogStart = start;
			DialogReturn = DialogStart+8;
		}
		if(!memcmp(start, EngineCode, sizeof(EngineCode))){
			EngineStart = start+0x65;
			memcpy(EngineHookSave, EngineStart, 0x20);
		}
		if(!memcmp(start, SkillTypeBaseCode, sizeof(SkillTypeBaseCode))){
			SkillTypeBase = (byte*)(*(dword*)(start+8));
		}
		if(!memcmp(start, WinHandleCode, sizeof(WinHandleCode))){
			WinHandle = (byte*)(*(dword*)(start+7));
		}
		if(!memcmp(start, LoadFinishedCode, sizeof(LoadFinishedCode))){
			LoadFinished = start+0x4E;
		}
		if(!memcmp(start, TargetLogCode, sizeof(TargetLogCode))){
			TargetLogStart = start;
			TargetLogReturn = TargetLogStart+5;
		}
		if(!memcmp(start, SetAttrisCode, sizeof(SetAttrisCode))){
			SetAttrisFunc = start + 0xE + *(dword*)(start+0xE) + 4;
		}
		if(	CurrentTarget &&
			BaseOffset &&
			PacketSendFunction &&
			MessageHandlerStart &&
			SkillLogStart &&
			MapIdLocation &&
			WriteWhisperStart &&
			TargetFunctions &&
			HeroSkillFunction &&
			ClickToMoveFix &&
			BuildNumber &&
			ChangeTargetFunction &&
			MaxZoomStill &&
			MaxZoomMobile &&
			SkillCancelStart &&
			//AgentNameFunction &&
			SellSessionStart &&
			SellItemFunction &&
			BuyItemFunction &&
			PingLocation &&
			LoggedInLocation &&
			NameLocation &&
			DeadLocation &&
			BasePointerLocation &&
			DialogStart &&
			EngineStart &&
			SkillTypeBase &&
			WinHandle &&
			LoadFinished &&
			TargetLogStart &&
			SetAttrisFunc){
			return;
		}
		start++;
	}
}

void WriteJMP(byte* location, byte* newFunction){
	DWORD dwOldProtection;
	VirtualProtect(location, 5, PAGE_EXECUTE_READWRITE, &dwOldProtection);
		location[0] = 0xE9;
		*((dword*)(location + 1)) = (dword)(newFunction - location) - 5;
	VirtualProtect(location, 5, dwOldProtection, &dwOldProtection);
}

void InjectErr(const char* lpzText){
	char* buf = new char[100];
	sprintf_s(buf, 100, "The %s could not be found!\nPlease contact SOMEONE about this issue.", lpzText);
	MessageBox(NULL, buf, "Hooking error!", MB_OK);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch(dwReason){
		case DLL_PROCESS_ATTACH:
			FindOffsets();
			if(!BaseOffset){
				InjectErr("BaseOffset");
				return false;
			}
			if(!PacketSendFunction){
				InjectErr("PacketSendFunction");
				return false;
			}else{
				PacketLocation = *(reinterpret_cast<dword*>(BaseOffset - 4));
				FlagLocation = PacketLocation - 0x130;
				PacketMutex = CreateMutex(NULL, false, NULL);
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&SendPacketQueueThread, 0, 0, 0);
			}
			if(!AgentArrayPtr){
				InjectErr("AgentArrayPtr");
				return false;
			}
			if(!MessageHandlerStart){
				InjectErr("MessageHandler");
				return false;
			}else{
				DWORD dwOldProtection;
				VirtualProtect(MessageHandlerStart, 9, PAGE_EXECUTE_READWRITE, &dwOldProtection);
				memset(MessageHandlerStart, 0x90, 9);
				VirtualProtect(MessageHandlerStart, 9, dwOldProtection, NULL);
				WriteJMP(MessageHandlerStart, (byte*)CustomMsgHandler);
			}
			if(!SkillLogStart){
				InjectErr("SkillLog");
				return false;
			}else{
				DWORD dwOldProtection;
				VirtualProtect(SkillLogStart, 8, PAGE_EXECUTE_READWRITE, &dwOldProtection);
				memset(SkillLogStart, 0x90, 8);
				VirtualProtect(SkillLogStart, 8, dwOldProtection, NULL);
				WriteJMP(SkillLogStart, (byte*)SkillLogHook);
				PartyMutex = CreateMutex(NULL, false, NULL);
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&SkillLogQueueThread, 0, 0, 0);
			}
			if(!MapIdLocation){
				InjectErr("MapIdLocation");
				return false;
			}
			if(!WriteWhisperStart){
				InjectErr("WriteWhisperStart");
				return false;
			}
			if(!TargetFunctions){
				InjectErr("TargetFunctions");
				return false;
			}
			if(!HeroSkillFunction){
				InjectErr("HeroSkillFunction");
				return false;
			}
			if(!ClickToMoveFix){
				InjectErr("ClickToMoveFix");
				return false;
			}else{
				DWORD dwOldProtection;
				VirtualProtect(ClickToMoveFix, 5, PAGE_EXECUTE_READWRITE, &dwOldProtection);
				byte ClickToMoveFixCode[] = { 0x83, 0xF8, 0x00, 0x90, 0x90 };
				memcpy(ClickToMoveFix, ClickToMoveFixCode, 5);
				VirtualProtect(ClickToMoveFix, 5, dwOldProtection, NULL);
			}
			if(!BuildNumber){
				InjectErr("BuildNumber");
				return false;
			}
			if(!ChangeTargetFunction){
				InjectErr("ChangeTargetFunction");
				return false;
			}
			if(!MaxZoomStill){
				InjectErr("MaxZoomStill");
				return false;
			}
			if(!MaxZoomMobile){
				InjectErr("MaxZoomMobile");
				return false;
			}
			if(!SkillCancelStart){
				InjectErr("SkillCancelStart");
				return false;
			}else{
				DWORD dwOldProtection;
				VirtualProtect(SkillCancelStart, 7, PAGE_EXECUTE_READWRITE, &dwOldProtection);
				memset(SkillCancelStart, 0x90, 7);
				VirtualProtect(SkillCancelStart, 7, dwOldProtection, NULL);
				WriteJMP(SkillCancelStart, (byte*)SkillCancelHook);
			}
			//if(!AgentNameFunction){
			//	InjectErr("AgentNameFunction");
			//	return false;
			//}
			if(!SellSessionStart){
				InjectErr("SellSessionStart");
				return false;
			}else{
				DWORD dwOldProtection;
				VirtualProtect(SellSessionStart, 9, PAGE_EXECUTE_READWRITE, &dwOldProtection);
				memset(SellSessionStart, 0x90, 9);
				VirtualProtect(SellSessionStart, 9, dwOldProtection, NULL);
				WriteJMP(SellSessionStart, (byte*)SellSessionHook);
			}
			if(!SellItemFunction){
				InjectErr("SellItemFunction");
				return false;
			}
			if(!BuyItemFunction){
				InjectErr("BuyItemFunction");
				return false;
			}
			if(!PingLocation){
				InjectErr("PingLocation");
				return false;
			}
			if(!LoggedInLocation){
				InjectErr("LoggedInLocation");
				return false;
			}
			if(!NameLocation){
				InjectErr("NameLocation");
				return false;
			}
			if(!DeadLocation){
				InjectErr("DeadLocation");
				return false;
			}
			if(!BasePointerLocation){
				InjectErr("BasePointerLocation");
				return false;
			}
			if(!DialogStart){
				InjectErr("DialogStart");
				return false;
			}else{
				DWORD dwOldProtection;
				VirtualProtect(DialogStart, 8, PAGE_EXECUTE_READWRITE, &dwOldProtection);
				memset(DialogStart, 0x90, 8);
				VirtualProtect(DialogStart, 8, dwOldProtection, NULL);
				WriteJMP(DialogStart, (byte*)DialogHook);
			}
			if(!EngineStart){
				InjectErr("EngineStart");
				return false;
			}
			if(!SkillTypeBase){
				InjectErr("SkillTypeBase");
				return false;
			}
			if(!WinHandle){
				InjectErr("WinHandle");
				return false;
			}
			if(!LoadFinished){
				InjectErr("LoadFinished");
				return false;
			}else{
				DWORD dwOldProtection;
				VirtualProtect(LoadFinished, 6, PAGE_EXECUTE_READWRITE, &dwOldProtection);
				memset(LoadFinished, 0x90, 6);
				VirtualProtect(LoadFinished, 6, dwOldProtection, NULL);
				WriteJMP(LoadFinished, (byte*)LoadHook);
			}
			if(!TargetLogStart){
				InjectErr("TargetLogStart");
				return false;
			}else{
				for(int i = 1;i < 2560;i++){ AgentTargets[i] = 0; }
				WriteJMP(TargetLogStart, (byte*)TargetLogHook);
			}
			if(!SetAttrisFunc){
				InjectErr("SetAttrisFunc");
				return false;
			}
			/*
			AllocConsole();
			SetConsoleTitleA("GWCA Console");
			FILE *fh;
			freopen_s(&fh, "CONOUT$", "wb", stdout);
			printf("BaseOffset=0x%06X\n", BaseOffset);
			printf("PacketSendFunction=0x%06X\n", PacketSendFunction);
			printf("CurrentTarget=0x%06X\n", CurrentTarget);
			printf("AgentArrayPtr=0x%06X\n", AgentArrayPtr);
			printf("AgentArrayMaxPtr=0x%06X\n", AgentArrayMaxPtr);
			printf("MessageHandlerStart=0x%06X\n", MessageHandlerStart);
			printf("MessageHandlerReturn=0x%06X\n", MessageHandlerReturn);
			printf("SkillLogStart=0x%06X\n", SkillLogStart);
			printf("SkillLogReturn=0x%06X\n", SkillLogReturn);
			printf("WriteWhisperStart=0x%06X\n", WriteWhisperStart);
			printf("TargetFunctions=0x%06X\n", TargetFunctions);
			printf("HeroSkillFunction=0x%06X\n", HeroSkillFunction);
			printf("ClickToMoveFix=0x%06X\n", ClickToMoveFix);
			printf("BuildNumber=0x%06X\n", BuildNumber);
			printf("ChangeTargetFunction=0x%06X\n", ChangeTargetFunction);
			printf("MaxZoomStill=0x%06X\n", MaxZoomStill);
			printf("MaxZoomMobile=0x%06X\n", MaxZoomMobile);
			printf("SkillCancelStart=0x%06X\n", SkillCancelStart);
			printf("SkillCancelReturn=0x%06X\n", SkillCancelReturn);
			//printf("AgentNameFunction=0x%06X\n", AgentNameFunction);
			printf("SellSessionStart=0x%06X\n", SellSessionStart);
			printf("SellItemFunction=0x%06X\n", SellItemFunction);
			printf("BuyItemFunction=0x%06X\n", BuyItemFunction);
			printf("PingLocation=0x%06X\n", PingLocation);
			printf("LoggedInLocation=0x%06X\n", LoggedInLocation);
			printf("NameLocation=0x%06X\n", NameLocation);
			printf("DeadLocation=0x%06X\n", DeadLocation);
			printf("BasePointerLocation=0x%06X\n", BasePointerLocation);
			printf("DialogStart=0x%06X\n", DialogStart);
			printf("DialogReturn=0x%06X\n", DialogReturn);
			printf("EngineStart=0x%06X\n", EngineStart);
			printf("SkillTypeBase=0x%06X\n", SkillTypeBase);
			printf("WinHandle=0x%06X\n", WinHandle);
			printf("LoadFinished=0x%06X\n", LoadFinished);
			printf("TargetLogStart=0x%06X\n", TargetLogStart);
			printf("TargetLogReturn=0x%06X\n", TargetLogReturn);
			*/
			break;

		case DLL_PROCESS_DETACH:
			break;
	}
	return true;
}