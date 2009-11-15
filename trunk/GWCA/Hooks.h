#pragma once
#ifndef _HOOKS_H
#define _HOOKS_H
/* Contains all Hooks and their Globals */

#include "stdafx.h"
#include "Skillbar.h"
#include "Agent.h"
#include "Functions.h"

std::vector<SkillLogSkill> SkillLogQueue;
std::vector<SkillLogSkill> SkillCancelQueue;



long LastDialogId = 0;

unsigned int MsgUInt = NULL;
WPARAM MsgWParam = NULL;
LPARAM MsgLParam = NULL;
HWND MsgHwnd = NULL;
int MsgInt;
int MsgInt2;
int MsgEvent = 0;
float MsgFloat;
float MsgFloat2;

bool LogSkills = false;
HWND ScriptHwnd = NULL;
wchar_t* pName;
long MoveItemId = NULL;
long TmpVariable = NULL;
long CurrentBag = 1;

/*
fucking c++ :X
void _declspec(naked) SkillLogHook();
void _declspec(naked) SkillCancelHook();
void _declspec(naked) SellSessionHook();
void _declspec(naked) DialogHook();
void _declspec(naked) EngineHook();
void _declspec(naked) CustomMsgHandler();
*/
void _declspec(naked) EngineHook(){
	Sleep(100);
	_asm RET
}
void SetEngineHook(int Enable){
	if(Enable){
		DWORD dwOldProtection;
		VirtualProtect(GWMemory.EngineStart, 2, PAGE_EXECUTE_READWRITE, &dwOldProtection);
		memset(GWMemory.EngineStart, 0x90, 2);
		VirtualProtect(GWMemory.EngineStart, 2, dwOldProtection, NULL);
		GWMemory.WriteJMP(GWMemory.EngineStart+0x13, (byte*)EngineHook);
	}else{
		DWORD dwOldProtection;
		VirtualProtect(GWMemory.EngineStart, 32, PAGE_EXECUTE_READWRITE, &dwOldProtection);
		memcpy(GWMemory.EngineStart, GWMemory.EngineHookSave, 32);
		VirtualProtect(GWMemory.EngineStart, 32, dwOldProtection, NULL);
	}
}
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
		JMP  GWMemory.SkillLogReturn
	}
}

void _declspec(naked) SkillCancelHook(){
	SkillLogSkill* cancelSkillPtr;

	_asm {
		MOV ESI,ECX
		MOV EAX,DWORD PTR DS:[EDI]
		MOV ECX,DWORD PTR DS:[ESI+0x4]
		PUSHAD
		MOV cancelSkillPtr,EDI
	}

	SkillCancelQueue.push_back(*cancelSkillPtr);

	_asm {
		POPAD
		JMP GWMemory.SkillCancelReturn
	}
}
void SkillLogQueueThread(){
	COPYDATASTRUCT SkillLogCDS;
	LoggedSkillStruct SkillInfo;

	SkillLogCDS.dwData = 1;
	SkillLogCDS.lpData = &SkillInfo;
	SkillLogCDS.cbData = sizeof(LoggedSkillStruct);

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
			SkillInfo.Ping = GWMemory.Ping();

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
		
		if(MsgHwnd){
			if((GetTickCount() - tTicks) > 5000){
				tTicks = GetTickCount();

				if(!GWMemory.Name()[0]){
					SetWindowTextW(MsgHwnd, L"Guild Wars");
				}else{
					swprintf(sWindowText, L"Guild Wars - %s", GWMemory.Name());
					SetWindowTextW(MsgHwnd, sWindowText);
				}
			}
		}
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

		JMP GWMemory.SellSessionReturn
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
		JMP GWMemory.DialogReturn
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
			JMP GWMemory.MessageHandlerReturn
		}
	}

	switch(MsgUInt){
		//Stuff related to you
		case 0x401: //Current Target : No return
			PostMessage((HWND)MsgLParam, 0x500, *(long*)GWMemory.CurrentTarget, 0);
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
			PostMessage((HWND)MsgLParam, 0x500, MySkillbar->Skill[MsgWParam-1].Recharge, 0);
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
			PostMessage((HWND)MsgLParam, 0x500, (WPARAM)GWMemory.AgentArrayPtr, (LPARAM)GWMemory.CurrentTarget);
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
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			AttackTarget(MsgWParam);
			break;
		case 0x411: //Move to x, y : No return
			memcpy(&MsgFloat, &MsgWParam, sizeof(int));
			memcpy(&MsgFloat2, &MsgLParam, sizeof(int));
			MovePlayer(MsgFloat, MsgFloat2);
			break;
		case 0x412: //Use skill : No return
			if(MsgLParam == -1){MsgLParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgLParam == -2){MsgLParam = myId;}
			UseSkill(MsgWParam, MsgLParam, MsgEvent);
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
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			GoNPC(MsgWParam);
			break;
		case 0x417: //Go to player : No return
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			GoPlayer(MsgWParam);
			break;
		case 0x418: //Go to signpost : No return
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			GoSignpost(MsgWParam);
			break;
		case 0x419: //Use attack skill : No return
			if(MsgLParam == -1){MsgLParam = *(long*)GWMemory.CurrentTarget;}
			UseAttackSkill(MsgWParam, MsgLParam, MsgEvent);
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
			if(MsgLParam == -1){MsgLParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgLParam == -2){MsgLParam = myId;}
			if(MySkillbar==NULL){RESPONSE_INVALID;}
			UseSkill(MySkillbar->Skill[MsgWParam-1].Id, MsgLParam, MySkillbar->Skill[MsgWParam-1].Event);
			break;
		case 0x41E: //Pick up item : No return
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			PickUpItem(MsgWParam);
			break;
		case 0x41F: //Use skillbar attack skill : No return
			ReloadSkillbar();
			if(MsgLParam == -1){MsgLParam = *(long*)GWMemory.CurrentTarget;}
			if(MySkillbar==NULL){RESPONSE_INVALID;}
			UseAttackSkill(MySkillbar->Skill[MsgWParam-1].Id, MsgLParam, MySkillbar->Skill[MsgWParam-1].Event);
			break;
		case 0x420: //Dialog packet : No return
			Dialog(MsgWParam);
			break;
		case 0x421: //Change target : No return
			_asm MOV ECX,MsgWParam
			_asm MOV EDX,0
			_asm CALL GWMemory.ChangeTargetFunction
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
			if(MsgLParam == -1){MsgLParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgLParam == -2){MsgLParam = myId;}
			if(Agents[MsgLParam]==NULL && MsgLParam!=0){RESPONSE_INVALID;}
			UseHeroSkill(*(long*)(GWMemory.HeroesStruct() + 0x4), MsgWParam-1, MsgLParam);
			break;
		case 0x429: //Use hero 2 skill : No return
			if(MsgLParam == -1){MsgLParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgLParam == -2){MsgLParam = myId;}
			if(Agents[MsgLParam]==NULL && MsgLParam!=0){RESPONSE_INVALID;}
			UseHeroSkill(*(long*)(GWMemory.HeroesStruct() + 0x28), MsgWParam-1, MsgLParam);
			break;
		case 0x42A: //Use hero 3 skill : No return
			if(MsgLParam == -1){MsgLParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgLParam == -2){MsgLParam = myId;}
			if(Agents[MsgLParam]==NULL && MsgLParam!=0){RESPONSE_INVALID;}
			UseHeroSkill(*(long*)(GWMemory.HeroesStruct() + 0x4C), MsgWParam-1, MsgLParam);
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
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			pName = GetAgentName(MsgWParam);
			PostMessage((HWND)MsgLParam, 0x500, (WPARAM)pName, 0);
			break;
		case 0x42F: //Command hero 1 to location : No return
			memcpy(&MsgFloat, &MsgWParam, sizeof(float));
			memcpy(&MsgFloat2, &MsgLParam, sizeof(float));
			CommandHero(*(long*)(GWMemory.HeroesStruct() + 0x4), MsgFloat, MsgFloat2);
			break;
		case 0x430: //Command hero 2 to location : No return
			memcpy(&MsgFloat, &MsgWParam, sizeof(float));
			memcpy(&MsgFloat2, &MsgLParam, sizeof(float));
			CommandHero(*(long*)(GWMemory.HeroesStruct() + 0x28), MsgFloat, MsgFloat2);
			break;
		case 0x431: //Command hero 3 to location : No return
			memcpy(&MsgFloat, &MsgWParam, sizeof(float));
			memcpy(&MsgFloat2, &MsgLParam, sizeof(float));
			CommandHero(*(long*)(GWMemory.HeroesStruct() + 0x4C), MsgFloat, MsgFloat2);
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

		//SectionA related commands
		case 0x440: //Check if map is loading : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, mapLoading, 0);
			break;
		case 0x441: //Get map id : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.MapId(), 0);
			break;
		case 0x442: //Get ping : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.Ping(), 0);
			break;
		case 0x443: //Check if logged in : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.LoggedIn(), 0);
			break;
		case 0x444: //Check if you're dead : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.Dead(), 0);
			break;
		case 0x445: //Get current and max balthazar faction : Return int/long & int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.CurrentBalthFaction(), GWMemory.MaxBalthFaction());
			break;
		case 0x446: //Get current and max kurzick faction : Return int/long & int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.CurrentKurzickFaction(), GWMemory.MaxKurzickFaction());
			break;
		case 0x447: //Get current and max luxon faction : Return int/long & int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.CurrentLuxonFaction(), GWMemory.MaxLuxonFaction());
			break;
		case 0x448: //Get current Treasure Title (credits to ddarek): Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.TitleTreasure(), 0);
			break;
		case 0x449: //Get current Lucky Title (credits to ddarek): Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.TitleLucky(), 0);
			break;
		case 0x44A: //Get current Unlucky Title (credits to ddarek): Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.TitleUnlucky(), 0);
			break;
		case 0x44B: //Get current Wisdom Title (credits to ddarek): Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.TitleWisdom(), 0);
			break;

		//Agent Related Commands
		case 0x450: //Check for agent existency : Return int/bool
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){
				PostMessage((HWND)MsgLParam, 0x500, 0, 0);
			}else{
				PostMessage((HWND)MsgLParam, 0x500, 1, 1);
			}
			break;
		case 0x451: //Get agent's primary and secondary profession : Return byte & byte
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->Primary, Agents[MsgWParam]->Secondary);
			break;
		case 0x452: //Get player number of agent : Return word
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->PlayerNumber, 0);
			break;
		case 0x453: //Get HP of agent : Return float
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			memcpy(&MsgInt, &Agents[MsgWParam]->HP, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x454: //Get rotation of agent in degrees : Return float
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgFloat = (float)(Agents[MsgWParam]->Rotation * 180 / 3.14159265358979323846);
			memcpy(&MsgInt, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x455: //Get agent's current skill : Return word
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->Skill, 0);
			break;
		case 0x456: //Get X,Y coords of agent : Return float & float
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			memcpy(&MsgInt, &Agents[MsgWParam]->X, sizeof(float));
			memcpy(&MsgInt2, &Agents[MsgWParam]->Y, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x457: //Get weapon speeds of agent (weapon attack speed, attack speed modifier) : Return float & float
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			memcpy(&MsgInt, &Agents[MsgWParam]->WeaponAttackSpeed, sizeof(float));
			memcpy(&MsgInt2, &Agents[MsgWParam]->AttackSpeedModifier, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x458: //Is agent in spirit range of me : Return int/long
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->InSpiritRange, 0);
			break;
		case 0x459: //Get team ID of agent (0 = none) : Return byte
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->TeamId, 0);
			break;
		case 0x45A: //Get agent's combat mode : Return byte
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->CombatMode, 0);
			break;
		case 0x45B: //Get agent's model mode : Return float
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			memcpy(&MsgInt, &Agents[MsgWParam]->ModelMode, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x45C: //Get agent's health pips : Return int
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			//memcpy(&MsgInt, &Agents[MsgWParam]->HPPips, sizeof(float));
			MsgInt = ((Agents[MsgWParam]->HPPips / 0.0038) > 0.0) ? floor((Agents[MsgWParam]->HPPips / 0.0038) + 0.5) : ceil((Agents[MsgWParam]->HPPips / 0.0038) - 0.5);//floor((Agents[MsgWParam]->HPPips / 0.0038) + 0.5);
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x45D: //Get agent's effect bit map : Return byte
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->Effects, 0);
			break;
		case 0x45E: //Get agent's hex bit map : Return byte
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->Hex, 0);
			break;
		case 0x45F: //Get agent's model animation : Return int/dword
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->ModelAnimation, 0);
			break;
		case 0x460: //Get agent's energy - ONLY WORKS FOR YOURSELF! : Return float
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			memcpy(&MsgInt, &Agents[MsgWParam]->Energy, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x461: //Get pointer to agent : Return ptr
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, (WPARAM)Agents[MsgWParam], 0);
			break;
		case 0x462: //Get agent type : Return int/long
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->Type, 0);
			break;
		case 0x463: //Get agent level : Return byte
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->Level, 0);
			break;
		case 0x464: //Get agent's name properties : Return int/long
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
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
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgFloat = GetDistanceFromAgentToAgent(myId, MsgWParam);
			memcpy(&MsgInt, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x468: //Get nearest agent to agent : Return int/long
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgInt = GetNearestAgentToAgent(MsgWParam);
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x469: //Get distance from agent (set previously in MsgInt by 0x406 : 1) to agent (MsgWParam) : Return float
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			if(MsgInt < 1 || (unsigned int)MsgInt > maxAgent){RESPONSE_INVALID;}
			MsgFloat = GetDistanceFromAgentToAgent(MsgInt, MsgWParam);
			memcpy(&MsgInt, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x46A: //Get nearest agent to agent AND the distance between them : Return int/long & float
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgInt = GetNearestAgentToAgent(MsgWParam);
			MsgFloat = GetDistanceFromAgentToAgent(MsgWParam, MsgInt);
			memcpy(&MsgInt2, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x46B: //Get model state of agent : Return int/long
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->ModelState, 0);
			break;
		case 0x46C: //Check if agent is attacking : Return int/bool
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
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
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			if(Agents[MsgWParam]->ModelState == 0x450){
				PostMessage((HWND)MsgLParam, 0x500, 1, 1);
			}else{
				PostMessage((HWND)MsgLParam, 0x500, 0, 0);
			}
			break;
		case 0x46E: //Check if agent is moving : Return int/bool
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
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
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			if(Agents[MsgWParam]->ModelState == 0x400 || Agents[MsgWParam]->HP == 0){
				PostMessage((HWND)MsgLParam, 0x500, 1, 1);
			}else{
				PostMessage((HWND)MsgLParam, 0x500, 0, 0);
			}
			break;
		case 0x470: //Check if agent is casting/using skill : Return int/bool
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
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
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->Allegiance, Agents[MsgWParam]->TeamId);
			break;
		case 0x473: //Get nearest enemy (by TeamId) to agent and the distance between them : Return int/long & float
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgInt = GetNearestEnemyToAgent(MsgWParam);
			MsgFloat = GetDistanceFromAgentToAgent(MsgWParam, MsgInt);
			memcpy(&MsgInt2, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x474: //Check if agent is under attack from enemy melee (by TeamId) : Return int
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgInt = IsAttackedMelee(MsgWParam);
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x475: //Get nearest item to agent : Return int/long & float
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
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
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgFloat = sqrt(pow(Agents[MsgWParam]->MoveX, 2) + pow(Agents[MsgWParam]->MoveY, 2));
			memcpy(&MsgInt, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, 0);
			break;
		case 0x478: //Get nearest enemy to agent by allegiance and the distance between them : Return int/long & float
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgInt = GetNearestEnemyToAgentByAllegiance(MsgWParam);
			MsgFloat = GetDistanceFromAgentToAgent(MsgWParam, MsgInt);
			memcpy(&MsgInt2, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x479: //Get nearest alive enemy to agent and the distance between them : Return int/long & float
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgInt = GetNearestAliveEnemyToAgent(MsgWParam);
			MsgFloat = GetDistanceFromAgentToAgent(MsgWParam, MsgInt);
			memcpy(&MsgInt2, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x47A: //Get weapon type : Return int/long
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->WeaponType, 0);
			break;
		case 0x47B: //Get nearest signpost to agent : Return int/long & float
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			MsgInt = GetNearestSignpostToAgent(MsgWParam);
			MsgFloat = GetDistanceFromAgentToAgent(MsgWParam, MsgInt);
			memcpy(&MsgInt2, &MsgFloat, sizeof(float));
			PostMessage((HWND)MsgLParam, 0x500, MsgInt, MsgInt2);
			break;
		case 0x47C: //Get nearest npc to agent by allegiance : Return int/long & float
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
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
			if(MsgWParam == -1){MsgWParam = *(long*)GWMemory.CurrentTarget;}
			if(MsgWParam == -2){MsgWParam = myId;}
			if(Agents[MsgWParam]==NULL){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, Agents[MsgWParam]->LoginNumber, 0);
			break;

		//Item related commands
		case 0x510: //Get gold : Return int/long & int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.MoneySelf(), GWMemory.MoneyStorage());
			break;
		case 0x511: //Get bag size : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager.GetBagSize(MsgWParam), 0);
			break;
		case 0x512: //Select bag to work with : No return
			CurrentBag = MsgWParam;
			break;
		case 0x513: //Get current bag item id : Return int/long
			if(!CurrentBag){break;}
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager.GetItemId(CurrentBag, MsgWParam), MyItemManager.GetItemModelId(CurrentBag, MsgWParam));
			break;
		case 0x514: //Get first ID kit item id : Return int/long & int/long
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager.FindIdKit(), 0);
			break;
		case 0x515: //Identify item by indexes : No return
			MsgInt = MyItemManager.FindIdKit();
			MsgInt2 = MyItemManager.GetItemId(MsgWParam, MsgLParam);
			if(!MsgInt || !MsgInt2){break;}
			IdentifyItem(MsgInt, MsgInt2);
			break;
		case 0x516: //Identify item by item id : No return
			MsgInt = MyItemManager.FindIdKit();
			if(!MsgInt){break;}
			IdentifyItem(MsgInt, MsgWParam);
			break;
		case 0x517: //Deposit gold in storage : No return
			MsgInt = GWMemory.MoneySelf();
			MsgInt2 = GWMemory.MoneyStorage();
			if(MsgWParam == -1){
				if((MsgInt2 + MsgInt) > 1000000){ MsgInt = 1000000 - MsgInt2; }
				MsgInt2 += MsgInt;
				MsgInt = GWMemory.MoneySelf() - MsgInt;
			}else{
				MsgInt2 += MsgWParam;
				MsgInt -= MsgWParam;
			}
			ChangeGold(MsgInt, MsgInt2);
			break;
		case 0x518: //Withdraw gold from storage : No return
			MsgInt = GWMemory.MoneySelf();
			MsgInt2 = GWMemory.MoneyStorage();
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
			MsgInt = MyItemManager.GetItemId(MsgWParam, MsgLParam);
			if(!SellSessionId || !MsgInt){break;}
			SellItem(MsgInt);
			break;
		case 0x51A: //Sell item by item id : No return
			if(!SellSessionId){break;}
			SellItem(MsgWParam);
			break;
		case 0x51B: //Buy ID kit : No return
			if(!GWMemory.MerchantItems()){break;}
			BuyItem(*(long*)(GWMemory.MerchantItems() + 0x10), 1, 100);
			break;
		case 0x51C: //Buy superior ID kit : No return
			if(!GWMemory.MerchantItems()){break;}
			BuyItem(*(long*)(GWMemory.MerchantItems() + 0x14), 1, 500);
			break;
		case 0x51D: //Prepare MoveItem by setting item id (internal) : No return
			if(MsgWParam && MsgLParam){
				MoveItemId = MyItemManager.GetItemId(MsgWParam, MsgLParam);
			}else{
				MoveItemId = MsgWParam;
			}
			break;
		case 0x51E: //Move the item specified by 0x520 : No return
			if(!MoveItemId){RESPONSE_INVALID;}
			MoveItem(MoveItemId, MyItemManager.GetBagPtr(MsgWParam)->id, (MsgLParam - 1));
			break;
		case 0x51F: //Get current bag item rarity and quantity : Return byte & byte
			if(!CurrentBag){break;}
			if(!MyItemManager.GetItemPtr(CurrentBag, MsgWParam)){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager.GetItemPtr(CurrentBag, MsgWParam)->extraItemInfo->rarity,
				MyItemManager.GetItemPtr(CurrentBag, MsgWParam)->quantity);
			break;
		case 0x520: //Use item by indexes : No return
			MsgInt = MyItemManager.GetItemId(MsgWParam, MsgLParam);
			if(!MsgInt){break;}
			UseItem(MsgInt);
			break;
		case 0x521: //Use item by item id : No return
			UseItem(MsgWParam);
			break;
		case 0x522: //Drop item by indexes : No return
			if(MyItemManager.GetItemPtr(MsgWParam, MsgLParam)){
				DropItem(MyItemManager.GetItemId(MsgWParam, MsgLParam),
					MyItemManager.GetItemPtr(MsgWParam, MsgLParam)->quantity);
			}
			break;
		case 0x523: //Drop item by id and specifying amount : No return
			if(MsgLParam == -1 && MyItemManager.GetItemPtr(MsgWParam)){
				MsgLParam = MyItemManager.GetItemPtr(MsgWParam)->quantity;
			}
			DropItem(MsgWParam, MsgLParam);
			break;
		case 0x524: //Accept all unclaimed items : No return
			if(!MyItemManager.GetBagPtr(7)){break;}
			AcceptAllItems(MyItemManager.GetBagPtr(7)->id);
			break;
		case 0x525: //Get current bag item last modifier and customized : Return byte & wchar_t*
			if(!CurrentBag){break;}
			if(!MyItemManager.GetItemPtr(CurrentBag, MsgWParam)){RESPONSE_INVALID;}
			PostMessage((HWND)MsgLParam, 0x500, MyItemManager.GetItemPtr(CurrentBag, MsgWParam)->extraItemInfo->lastModifier,
				(LPARAM)MyItemManager.GetItemPtr(CurrentBag, MsgWParam)->customized);
			break;

		//Title related commands
		case 0x550: //Get current Sunspear Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.TitleSunspear(), 0);
			break;
		case 0x551: //Get current Lightbringer Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.TitleLightbringer(), 0);
			break;
		case 0x552: //Get current Vanguard Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.TitleVanguard(), 0);
			break;
		case 0x553: //Get current Norn Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.TitleNorn(), 0);
			break;
		case 0x554: //Get current Asura Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.TitleAsura(), 0);
			break;
		case 0x555: //Get current Deldrimor Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.TitleDeldrimor(), 0);
			break;
		case 0x556: //Get current North Mastery Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.TitleNorthMastery(), 0);
			break;
		case 0x557: //Get current Drunkard Title : Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.TitleDrunkard(), 0);
			break;
		case 0x558: //Get current Sweet Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.TitleSweet(), 0);
			break;
		case 0x559: //Get current Party Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.TitleParty(), 0);
			break;
		case 0x55A: //Get current Commander Title: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.TitleCommander(), 0);
			break;
		case 0x55B: //Get current Luxon Title Track: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.TitleTrackLuxon(), 0);
			break;
		case 0x55C: //Get current Kurzick Title Track: Return int/long
			PostMessage((HWND)MsgLParam, 0x500, GWMemory.TitleTrackKurzick(), 0);
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
#endif