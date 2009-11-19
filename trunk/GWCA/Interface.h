#ifndef _INTERFACE_H
#define _INTERFACE_H

/*			TODO:

 -structure the whole shit
 -add mem read/write
 -add hooks
							*/
#include "LUA\include\lua.hpp"

#include "stdafx.h"

#include "Skillbar.h"
#include "Agent.h"
#include "Item.h"
#include "Functions.h"
#include "Packet.h"


void LuaError(const char* lpzText);

int LUA_MovePlayer( lua_State* L );
int LUA_SendChat( lua_State* L );
int LUA_MoveMap( lua_State* L );
int LUA_ChangeDistrict( lua_State* L );
int LUA_UseSkill( lua_State* L );
int LUA_ChangeWeaponSet( lua_State* L );
int LUA_PickUpItem( lua_State* L);
int LUA_UseItem( lua_State* L);
int LUA_EquipItem( lua_State* L);
int LUA_DropItem( lua_State* L);
int LUA_DropGold( lua_State* L);
int LUA_AttackTarget( lua_State* L);
int LUA_CommandHero( lua_State* L);
int LUA_CommandAll( lua_State* L);
int LUA_Dialog( lua_State* L);
int LUA_GoNPC( lua_State* L);
int LUA_DonateFaction( lua_State* L);
int LUA_ExchangeAmber( lua_State* L);
int LUA_OpenChest( lua_State* L);
int LUA_GoSignpost( lua_State* L);
int LUA_EnterChallenge( lua_State* L);
int LUA_GoPlayer( lua_State* L);
int LUA_UseAttackSkill( lua_State* L);
int LUA_CancelAction( lua_State* L);
int LUA_IdentifyItem( lua_State* L);
int LUA_ChangeGold( lua_State* L);
int LUA_MoveItem( lua_State* L);
int LUA_ReturnToOutpost( lua_State* L);
int LUA_AcceptAllItems( lua_State* L);
int LUA_AddHero( lua_State* L);
int LUA_KickHero( lua_State* L);
int LUA_SwitchMode( lua_State* L);
int LUA_AddNpc( lua_State* L);
int LUA_KickNpc( lua_State* L);
int LUA_TravelGH( lua_State* L);
int LUA_LeaveGH( lua_State* L);
int LUA_GetNearestAgentToAgent( lua_State* L);
int LUA_GetNearestEnemyToAgent( lua_State* L);
int LUA_GetNearestEnemyToAgentByAllegiance( lua_State* L);
int LUA_GetNearestAliveEnemyToAgent( lua_State* L);
int LUA_GetNearestItemToAgent( lua_State* L);
int LUA_GetNearestSignpostToAgent( lua_State* L);
int LUA_GetNearestNpcToAgentByAllegiance( lua_State* L);
int LUA_GetDistanceFromAgentToAgent( lua_State* L);
int LUA_GetAngleFromAgentToAgent( lua_State* L);
int LUA_GetFirstAgentByPlayerNumber( lua_State* L);
int LUA_GetNearestAgentByPlayerNumber( lua_State* L);
int LUA_GetNearestAgentToCoords( lua_State* L);
int LUA_GetNearestNPCToCoords( lua_State* L);
int LUA_IsAttackedMelee( lua_State* L);
int LUA_SellItem( lua_State* L);
int LUA_BuyItem( lua_State* L);
int LUA_WriteWhisper( lua_State* L);
int LUA_GetAgentName( lua_State* L);
int LUA_TargetNearestFoe( lua_State* L);
int LUA_TargetNearestAlly( lua_State* L);
int LUA_TargetNearestItem( lua_State* L);
int LUA_TargetCalledTarget( lua_State* L);
int LUA_UseHeroSkill( lua_State* L);
int LUA_CompareCharName( lua_State* L);
int LUA_ChangeMaxZoom( lua_State* L);
int LUA_IsCasting( lua_State* L);
int LUA_SkillRecharge( lua_State* L);
int LUA_SkillAdrenaline( lua_State* L);
int LUA_GetSkillbarSkillId( lua_State* L);
int LUA_GetAgentHP(lua_State* L);
int LUA_GetAgentEnergy(lua_State* L);
int LUA_ChangeTarget(lua_State* L);
int LUA_GetProffessions(lua_State* L);
int LUA_GetPlayerNumberOfAgent(lua_State* L);
int LUA_GetRotationOfAgent(lua_State* L);
int LUA_GetAgentPosition(lua_State* L);
int LUA_GetAgentTeamId(lua_State* L);
int LUA_GetAgentHPPips(lua_State* L);
int LUA_GetAgentEffectBitmap(lua_State* L);
int LUA_GetAgentHexBitmap(lua_State* L);
int LUA_GetAgentType(lua_State* L);
int LUA_GetAgentLevel(lua_State* L);
int LUA_GetAgentProperties(lua_State* L);
int LUA_IsAgentAttacking(lua_State* L);
int LUA_IsAgentKnockedDown(lua_State* L);
int LUA_IsAgentMoving(lua_State* L);
int LUA_IsAgentCasting(lua_State* L);
int LUA_GetAgentAllegiance(lua_State* L);
int LUA_GetBagSize(lua_State* L);
int LUA_GetFirstIdKitId(lua_State* L);
int LUA_IdentifieItemByIndex(lua_State* L);
int LUA_IdentifieItemByID(lua_State* L);
int LUA_DepositGold(lua_State* L);
int LUA_WithdrawGold(lua_State* L);
int LUA_SellItemByIndex(lua_State* L);
int LUA_BuyIDKit(lua_State* L);
int LUA_BuySuperiorIDKit(lua_State* L);
int LUA_GetItemId(lua_State* L);
int LUA_GetItemInfo(lua_State* L);
int LUA_UseItemByIndex(lua_State* L);
int LUA_GetItemLastModifier(lua_State* L);
int LUA_GetCurrentTargetID(lua_State* L);
int LUA_GetOwnID(lua_State* L);
int LUA_GetMyMaxHP(lua_State* L);
int LUA_GetMyMaxEnergy(lua_State* L);
int LUA_GetMapLoading(lua_State* L);
int LUA_GetMapId(lua_State* L);
int LUA_GetPing(lua_State* L);
int LUA_GetLoggedIn(lua_State* L);
int LUA_GetDead(lua_State* L);
int LUA_GetCurrentBalthFaction(lua_State* L);
int LUA_GetMaxBalthFaction(lua_State* L);
int LUA_GetCurrentKurzickFaction(lua_State* L);
int LUA_GetMaxKurzickFaction(lua_State* L);
int LUA_GetCurrentLuxonFaction(lua_State* L);
int LUA_GetMaxLuxonFaction(lua_State* L);
int LUA_GetTitleTreasure(lua_State* L);
int LUA_GetTitleLucky(lua_State* L);
int LUA_GetTitleUnlucky(lua_State* L);
int LUA_GetTitleWisdom(lua_State* L);
int LUA_GetMoneySelf(lua_State* L);
int LUA_GetMoneyStorage(lua_State* L);
int LUA_SetSkillbarSkill( lua_State* L);
#endif