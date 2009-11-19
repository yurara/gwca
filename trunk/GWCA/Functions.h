#pragma once
#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H
/* Contains all Functions to Interact with GW
   TODO: make Functions Class	*/
#include "stdafx.h"
#include "Packet.h"
#include "Agent.h"
#include "Item.h"

extern long SellSessionId;

void MovePlayer(float nX, float nY, float nZ = 0);
void SendChat(char channel, char* message);
void SendChat(char channel, wchar_t* message);
void MoveMap(dword MapID, dword Region = 2, dword District = 0, dword Language = 0);
void ChangeDistrict(dword Region = 2, dword Language = 0);
void UseSkill(dword SkillID, dword TargetID, dword Event = 0);
void ChangeWeaponSet(dword WeaponSet);
void PickUpItem(long TargetID);
void UseItem(dword ItemID);
void EquipItem(dword ItemID);
void DropItem(dword ItemID, dword Amount = 1);
void DropGold(dword Amount);
void AttackTarget(long TargetID);
void CommandHero(long id, float nX, float nY);
void CommandAll(float nX, float nY);
void GetQuest(byte id);
void QuestReward(byte id);
void Dialog(long pick);
void GoNPC(long id);
void DonateFaction();
void ExchangeAmber();
void OpenChest();
void GoSignpost(long id);
void EnterChallenge();
void GoPlayer(long id);
void UseAttackSkill(dword SkillID, dword TargetID, dword Event = 0);
void CancelAction();
void IdentifyItem(long idKit, long itemId);
void ChangeGold(long goldSelf, long goldStorage);
void MoveItem(long itemId, long bagId, long slot);
void ReturnToOutpost();
void AcceptAllItems(long bagId);
void AddHero(long HeroId);
void KickHero(long HeroId);
void SwitchMode(long NMHM = 1);
void AddNpc(long NpcId);
void KickNpc(long NpcId);
void TravelGH();
void LeaveGH();
void SetSkillbarSkill(dword slot,dword id);

long GetNearestAgentToAgent(long agentId);
long GetNearestEnemyToAgent(long agentId);
long GetNearestEnemyToAgentByAllegiance(long agentId);
long GetNearestAliveEnemyToAgent(long agentId);
long GetNearestItemToAgent(long agentId);
long GetNearestSignpostToAgent(long agentId);
long GetNearestNpcToAgentByAllegiance(long agentId);
float GetDistanceFromAgentToAgent(long agent1, long agent2);
float GetAngleFromAgentToAgent(long agent1, long agent2);
long GetFirstAgentByPlayerNumber(word playerNum);
long GetNearestAgentByPlayerNumber(word playerNum);
long GetNearestAgentToCoords(float x, float y);
long GetNearestNPCToCoords(float x, float y);
int IsAttackedMelee(long agentId);

void SellItem(long itemId);
void BuyItem(long id, long quantity, long value);
void WriteWhisper(const wchar_t* chatMsg, const wchar_t* chatName);
wchar_t* GetAgentName(int agentId);
void TargetNearestFoe();
void TargetNearestAlly();
void TargetNearestItem();
void TargetCalledTarget();
void UseHeroSkill(long HeroId, long SkillNumber, long Target);
bool CompareCharName(wchar_t* cmpName);
void ChangeMaxZoom(float fZoom);

#endif