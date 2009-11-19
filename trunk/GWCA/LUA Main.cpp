#include "LUA Main.h"

#define REGISTER(Function,Name) lua_pushcfunction( g_LuaInstance, ##Function## ); lua_setglobal( g_LuaInstance, ##Name## );

lua_State* g_LuaInstance = NULL;

void run_script(char* script){
	luaL_dofile(g_LuaInstance,script);
}
void init_lua()
{
  g_LuaInstance = lua_open();
  luaopen_base( g_LuaInstance );
  //register functions
  REGISTER(LUA_MovePlayer,"MovePlayer");
  REGISTER(LUA_SendChat,"SendChat");
  REGISTER(LUA_MoveMap,"MoveMap");
  REGISTER(LUA_ChangeDistrict,"ChangeDistrict");
  REGISTER(LUA_UseSkill,"UseSkill");
  REGISTER(LUA_ChangeWeaponSet,"ChangeWeaponSet");
  REGISTER(LUA_PickUpItem,"PickUpItem");
  REGISTER(LUA_UseItem,"UseItem");
  REGISTER(LUA_EquipItem,"EquipItem");
  REGISTER(LUA_DropItem,"DropItem");
  REGISTER(LUA_DropGold,"DropGold");
  REGISTER(LUA_AttackTarget,"AttackTarget");
  REGISTER(LUA_CommandHero,"CommandHero");
  REGISTER(LUA_CommandAll,"CommandAll");
  REGISTER(LUA_Dialog,"Dialog");
  REGISTER(LUA_GoNPC,"GoNPC");
  REGISTER(LUA_DonateFaction,"DonateFaction");
  REGISTER(LUA_ExchangeAmber,"ExchangeAmber");
  REGISTER(LUA_OpenChest,"penChest");
  REGISTER(LUA_GoSignpost,"GoSignpost");
  REGISTER(LUA_EnterChallenge,"EnterChallenge");
  REGISTER(LUA_GoPlayer,"GoPlayer");
  REGISTER(LUA_UseAttackSkill,"UseAttackSkill");
  REGISTER(LUA_CancelAction,"CancelAction");
  REGISTER(LUA_IdentifyItem,"IdentifyItem");
  REGISTER(LUA_ChangeGold,"ChangeGold");
  REGISTER(LUA_MoveItem,"MoveItem");
  REGISTER(LUA_ReturnToOutpost,"ReturnToOutpost");
  REGISTER(LUA_AcceptAllItems,"AcceptAllItems");
  REGISTER(LUA_AddHero,"AddHero");
  REGISTER(LUA_KickHero,"KickHero");
  REGISTER(LUA_SwitchMode,"SwitchMode");
  REGISTER(LUA_AddNpc,"AddNpc");
  REGISTER(LUA_KickNpc,"KickNpc");
  REGISTER(LUA_TravelGH,"TravelGH");
  REGISTER(LUA_LeaveGH,"LeaveGH");
  REGISTER(LUA_GetNearestAgentToAgent,"GetNearestAgentToAgent");
  REGISTER(LUA_GetNearestEnemyToAgent,"GetNearestEnemyToAgent");
  REGISTER(LUA_GetNearestEnemyToAgentByAllegiance,"GetNearestEnemyToAgentByAllegiance");
  REGISTER(LUA_GetNearestAliveEnemyToAgent,"GetNearestAliveEnemyToAgent");
  REGISTER(LUA_GetNearestItemToAgent,"GetNearestItemToAgent");
  REGISTER(LUA_GetNearestSignpostToAgent,"GetNearestSignpostToAgent");
  REGISTER(LUA_GetNearestNpcToAgentByAllegiance,"GetNearestNpcToAgentByAllegiance");
  REGISTER(LUA_GetDistanceFromAgentToAgent,"GetDistanceFromAgentToAgent");
  REGISTER(LUA_GetAngleFromAgentToAgent,"GetAngleFromAgentToAgent");
  REGISTER(LUA_GetFirstAgentByPlayerNumber,"GetFirstAgentByPlayerNumber");
  REGISTER(LUA_GetNearestAgentByPlayerNumber,"GetNearestAgentByPlayerNumber");
  REGISTER(LUA_GetNearestAgentToCoords,"GetNearestAgentToCoords");
  REGISTER(LUA_GetNearestNPCToCoords,"GetNearestNPCToCoords");
  REGISTER(LUA_IsAttackedMelee,"IsAttackedMelee");
  REGISTER(LUA_SellItem,"SellItem");
  REGISTER(LUA_BuyItem,"BuyItem");
  REGISTER(LUA_WriteWhisper,"WriteWhisper");
  REGISTER(LUA_GetAgentName,"GetAgentName");
  REGISTER(LUA_TargetNearestFoe,"TargetNearestFoe");
  REGISTER(LUA_TargetNearestAlly,"TargetNearestAlly");
  REGISTER(LUA_TargetNearestItem,"TargetNearestItem");
  REGISTER(LUA_TargetCalledTarget,"TargetCalledTarget");
  REGISTER(LUA_UseHeroSkill,"UseHeroSkill");
  REGISTER(LUA_CompareCharName,"CompareCharName");
  REGISTER(LUA_ChangeMaxZoom,"ChangeMaxZoom");
  REGISTER(LUA_IsCasting,"IsCasting");
  REGISTER(LUA_SkillRecharge,"SkillRecharge");
  REGISTER(LUA_SkillAdrenaline,"SkillAdrenaline");
  REGISTER(LUA_GetSkillbarSkillId,"GetSkillbarSkillId");
  REGISTER(LUA_GetAgentHP,"GetAgentHP");
  REGISTER(LUA_GetAgentEnergy,"GetAgentEnergy");
  REGISTER(LUA_ChangeTarget,"ChangeTarget");
  REGISTER(LUA_GetProffessions,"GetProffessions");
  REGISTER(LUA_GetPlayerNumberOfAgent,"GetPlayerNumberOfAgent");
  REGISTER(LUA_GetRotationOfAgent,"GetRotationOfAgent");
  REGISTER(LUA_GetAgentPosition,"GetAgentPosition");
  REGISTER(LUA_GetAgentTeamId,"GetAgentTeamId");
  REGISTER(LUA_GetAgentHPPips,"GetAgentHPPips");
  REGISTER(LUA_GetAgentEffectBitmap,"GetAgentEffectBitmap");
  REGISTER(LUA_GetAgentHexBitmap,"GetAgentHexBitmap");
  REGISTER(LUA_GetAgentType,"GetAgentType");
  REGISTER(LUA_GetAgentLevel,"GetAgentLevel");
  REGISTER(LUA_GetAgentProperties,"GetAgentProperties");
  REGISTER(LUA_IsAgentAttacking,"IsAgentAttacking");
  REGISTER(LUA_IsAgentKnockedDown,"IsAgentKnockedDown");
  REGISTER(LUA_IsAgentMoving,"IsAgentMoving");
  REGISTER(LUA_IsAgentCasting,"IsAgentCasting");
  REGISTER(LUA_GetAgentAllegiance,"GetAgentAllegiance");
  REGISTER(LUA_GetBagSize,"GetBagSize");
  REGISTER(LUA_GetFirstIdKitId,"GetFirstIdKitId");
  REGISTER(LUA_IdentifieItemByIndex,"IdentifieItemByIndex");
  REGISTER(LUA_IdentifieItemByID,"IdentifieItemByID");
  REGISTER(LUA_DepositGold,"DepositGold");
  REGISTER(LUA_WithdrawGold,"WithdrawGold");
  REGISTER(LUA_SellItemByIndex,"SellItemByIndex");
  REGISTER(LUA_BuyIDKit,"BuyIDKit");
  REGISTER(LUA_BuySuperiorIDKit,"BuySuperiorIDKit");
  REGISTER(LUA_GetItemId,"GetItemId");
  REGISTER(LUA_GetItemInfo,"GetItemInfo");
  REGISTER(LUA_UseItemByIndex,"UseItemByIndex");
  REGISTER(LUA_GetItemLastModifier,"GetItemLastModifier");
  REGISTER(LUA_GetCurrentTargetID,"GetCurrentTargetID");
  REGISTER(LUA_GetOwnID,"GetOwnID");
  REGISTER(LUA_GetMyMaxHP,"GetMyMaxHP");
  REGISTER(LUA_GetMyMaxEnergy,"GetMyMaxEnergy");
  REGISTER(LUA_GetMapLoading,"GetMapLoading");
  REGISTER(LUA_GetMapId,"GetMapId");
  REGISTER(LUA_GetPing,"GetPing");
  REGISTER(LUA_GetLoggedIn,"GetLoggedIn");
  REGISTER(LUA_GetDead,"GetDead");
  REGISTER(LUA_GetCurrentBalthFaction,"GetCurrentBalthFaction");
  REGISTER(LUA_GetMaxBalthFaction,"GetMaxBalthFaction");
  REGISTER(LUA_GetCurrentKurzickFaction,"GetCurrentKurzickFaction");
  REGISTER(LUA_GetMaxKurzickFaction,"GetMaxKurzickFaction");
  REGISTER(LUA_GetCurrentLuxonFaction,"GetCurrentLuxonFaction");
  REGISTER(LUA_GetMaxLuxonFaction,"GetMaxLuxonFaction");
  REGISTER(LUA_GetTitleTreasure,"GetTitleTreasure");
  REGISTER(LUA_GetTitleLucky,"GetTitleLucky");
  REGISTER(LUA_GetTitleUnlucky,"GetTitleUnlucky");
  REGISTER(LUA_GetTitleWisdom,"GetTitleWisdom");
  REGISTER(LUA_GetMoneySelf,"GetMoneySelf");
  REGISTER(LUA_GetMoneyStorage,"GetMoneyStorage");
  REGISTER(LUA_SetSkillbarSkill,"SetSkillbarSkill");
}

void close_lua()
{
  lua_close( g_LuaInstance );
}
