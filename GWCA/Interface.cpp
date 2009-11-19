#include "Interface.h"
#define CHECK(parameter,Name) if(lua_gettop(L) < parameter){LuaError(##Name##); lua_pop( L, lua_gettop( L ) ); return 0;}



	
	

void LuaError(const char* lpzText){
	char* buf = new char[100];
	sprintf(buf, " Error in Function %s !\nNot enough Arguments.", lpzText);
	MessageBox(NULL,(LPCSTR)*buf,"LUA error!", MB_OK);
}
//GWFunctions
int LUA_MovePlayer( lua_State* L ){
	CHECK(2,"MovePlayer")
	float nX = lua_tonumber(L,1);
	float nY = lua_tonumber(L,2);
	float nZ = lua_tonumber(L,3);
	MovePlayer(nX, nY,nZ);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_SendChat( lua_State* L ){
	CHECK(2,"SendChat")
	char channel = (char)lua_tointeger(L,1);
	char* message = (char*)lua_tostring(L,2);
	SendChat(channel,message);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_MoveMap( lua_State* L ){
	CHECK(1,"MoveMap")
	dword MapID = lua_tonumber(L,1);
	dword Region = lua_tonumber(L,2);
	dword District = lua_tonumber(L,3);
	dword Language = lua_tonumber(L,4);
	MoveMap(MapID,Region,District,Language);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}

int LUA_ChangeDistrict( lua_State* L ){
	dword Region = lua_tonumber(L,1);
	dword Language = lua_tonumber(L,2);
	ChangeDistrict(Region,Language);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}

int LUA_UseSkill( lua_State* L ){
	CHECK(2,"UseSkill")
	dword SkillID = lua_tonumber(L,1);
	dword TargetID = lua_tonumber(L,2);
	dword Event = lua_tonumber(L,3);
	UseSkill(SkillID,TargetID,Event);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_ChangeWeaponSet( lua_State* L ){
	CHECK(1,"ChangeWeaponSet")
	dword WeaponSet = lua_tonumber(L,1);
	ChangeWeaponSet(WeaponSet);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_PickUpItem( lua_State* L){
	CHECK(1,"PickUpItem")
	long ID = lua_tonumber(L,1);
	PickUpItem(ID);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_UseItem( lua_State* L){
	CHECK(1,"UseItem")
	dword ID = lua_tonumber(L,1);
	UseItem(ID);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_EquipItem( lua_State* L){
	CHECK(1,"EquipItem")
	dword ID = lua_tonumber(L,1);
	EquipItem(ID);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_DropItem( lua_State* L){
	CHECK(1,"EquipItem")
	dword ID = lua_tonumber(L,1);
	dword Amount = lua_tonumber(L,2);
	DropItem(ID,Amount);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_DropGold( lua_State* L){
	CHECK(1,"DropGold")
	dword Amount = lua_tonumber(L,1);
	DropGold(Amount);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_AttackTarget( lua_State* L){
	CHECK(1,"AttackTarget")
	dword ID = lua_tonumber(L,1);
	AttackTarget(ID);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_CommandHero( lua_State* L){
	CHECK(3,"CommandHero")
	dword ID = lua_tonumber(L,1);
	if(ID==1){
		ID = *(long*)(GWMemory.HeroesStruct() + 0x4);
	}else if(ID==2){
		ID = *(long*)(GWMemory.HeroesStruct() + 0x28);
	}else if(ID==3){
		ID = *(long*)(GWMemory.HeroesStruct() + 0x4C);
	}
	dword X = lua_tonumber(L,2);
	dword Y = lua_tonumber(L,3);
	CommandHero(ID,X,Y);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_CommandAll( lua_State* L){
	CHECK(2,"CommandAll")
	dword X = lua_tonumber(L,1);
	dword Y = lua_tonumber(L,2);
	CommandAll(X,Y);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_Dialog( lua_State* L){
	CHECK(1,"Dialog")
	long ID = lua_tonumber(L,1);
	Dialog(ID);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_GoNPC( lua_State* L){
	CHECK(1,"GoNPC")
	dword ID = lua_tonumber(L,1);
	GoNPC(ID);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_DonateFaction( lua_State* L){
	DonateFaction();
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_ExchangeAmber( lua_State* L){
	ExchangeAmber();
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_OpenChest( lua_State* L){
	OpenChest();
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_GoSignpost( lua_State* L){
	CHECK(1,"GoSignpost")
	long ID = lua_tonumber(L,1);
	GoSignpost(ID);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_EnterChallenge( lua_State* L){
	EnterChallenge();
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_GoPlayer( lua_State* L){
	CHECK(1,"GoPlayer")
	long ID = lua_tonumber(L,1);
	GoPlayer(ID);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_UseAttackSkill( lua_State* L){
	CHECK(2,"UseAttackSkill")
	dword ID = lua_tonumber(L,1);
	dword target = lua_tonumber(L,2);
	dword Event = lua_tonumber(L,3);
	UseAttackSkill(ID,target,Event);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_CancelAction( lua_State* L){
	CancelAction();
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_IdentifyItem( lua_State* L){
	CHECK(2,"IdentifyItem")
	long kit = lua_tonumber(L,1);
	long item = lua_tonumber(L,2);
	IdentifyItem(kit,item);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_ChangeGold( lua_State* L){
	CHECK(2,"ChangeGold")
	long self = lua_tonumber(L,1);
	long storage = lua_tonumber(L,2);
	ChangeGold(self,storage);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_MoveItem( lua_State* L){
	CHECK(3,"MoveItem")
	long item = lua_tonumber(L,1);
	long bag = lua_tonumber(L,2);
	long slot = lua_tonumber(L,3);
	MoveItem(item,bag,slot);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_ReturnToOutpost( lua_State* L){
	ReturnToOutpost();
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_AcceptAllItems( lua_State* L){
	if(!MyItemManager.GetBagPtr(7)){lua_pop( L, lua_gettop( L ) );
	return 0;}
	AcceptAllItems(MyItemManager.GetBagPtr(7)->id);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_AddHero( lua_State* L){
	CHECK(1,"AddHero")
	long ID = lua_tonumber(L,1);
	AddHero(ID);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_KickHero( lua_State* L){
	CHECK(1,"KickHero")
	long ID = lua_tonumber(L,1);
	KickHero(ID);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_SwitchMode( lua_State* L){
	CHECK(1,"SwitchMode")
	long mode = lua_tonumber(L,1);
	SwitchMode(mode);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_AddNpc( lua_State* L){
	CHECK(1,"AddNPC")
	long ID = lua_tonumber(L,1);
	AddNpc(ID);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_KickNpc( lua_State* L){
	CHECK(1,"KickNPC")
	long ID = lua_tonumber(L,1);
	KickNpc(ID);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_TravelGH( lua_State* L){
	TravelGH();
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_LeaveGH( lua_State* L){
	LeaveGH();
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_GetNearestAgentToAgent( lua_State* L){
	CHECK(1,"GetNearestAgentToAgent")
	long ID = lua_tonumber(L,1);
	long agent=GetNearestAgentToAgent(ID);
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,agent);
	return 1;
}
int LUA_GetNearestEnemyToAgent( lua_State* L){
	CHECK(1,"GetNearestEnemyToAgent")
	long ID = lua_tonumber(L,1);
	long agent=GetNearestEnemyToAgent(ID);
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,agent);
	return 1;
}
int LUA_GetNearestEnemyToAgentByAllegiance( lua_State* L){
	CHECK(1,"GetNearestEnemyToAgentByAllegiance")
	long ID = lua_tonumber(L,1);
	long agent=GetNearestEnemyToAgentByAllegiance(ID);
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,agent);
	return 1;
}
int LUA_GetNearestAliveEnemyToAgent( lua_State* L){
	CHECK(1,"GetNearestAliveEnemyToAgent")
	long ID = lua_tonumber(L,1);
	long agent=GetNearestAliveEnemyToAgent(ID);
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,agent);
	return 1;
}
int LUA_GetNearestItemToAgent( lua_State* L){
	CHECK(1,"GetNearestItemToAgent")
	long ID = lua_tonumber(L,1);
	long item=GetNearestItemToAgent(ID);
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,item);
	return 1;
}
int LUA_GetNearestSignpostToAgent( lua_State* L){
	CHECK(1,"GetNearestSignpostToAgent")
	long ID = lua_tonumber(L,1);
	long signpost = GetNearestSignpostToAgent(ID);
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,signpost);
	return 1;
}
int LUA_GetNearestNpcToAgentByAllegiance( lua_State* L){
	CHECK(1,"GetNearestNPCToAgentByAllegiance")
	long ID = lua_tonumber(L,1);
	long NPC=GetNearestNpcToAgentByAllegiance(ID);
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,NPC);
	return 1;
}
int LUA_GetDistanceFromAgentToAgent( lua_State* L){
	CHECK(2,"GetDistanceFromAgentToAgent")
	long ID1 = lua_tonumber(L,1);
	long ID2 = lua_tonumber(L,2);
	float distance=GetDistanceFromAgentToAgent(ID1,ID2);
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,distance);
	return 1;
}
int LUA_GetAngleFromAgentToAgent( lua_State* L){
	CHECK(2,"GetAngleFromAgentToAgent")
	long ID1 = lua_tonumber(L,1);
	long ID2 = lua_tonumber(L,2);
	float angle=GetDistanceFromAgentToAgent(ID1,ID2);
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,angle);
	return 1;
}
int LUA_GetFirstAgentByPlayerNumber( lua_State* L){
	CHECK(1,"GetFirstAgentByPlayerNumber")
	word ID1 = lua_tonumber(L,1);
	long agent=GetFirstAgentByPlayerNumber(ID1);
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,agent);
	return 1;
}
int LUA_GetNearestAgentByPlayerNumber( lua_State* L){
	CHECK(1,"GetNearestAgentByPlayerNumber")
	word ID1 = lua_tonumber(L,1);
	long agent=GetNearestAgentByPlayerNumber(ID1);
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,agent);
	return 1;
}
int LUA_GetNearestAgentToCoords( lua_State* L){
	CHECK(2,"GetNearestAgentToCoords")
	float x = lua_tonumber(L,1);
	float y = lua_tonumber(L,2);
	long agent=GetNearestAgentToCoords(x,y);
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,agent);
	return 1;
}
int LUA_GetNearestNPCToCoords( lua_State* L){
	CHECK(2,"GetNearestNPCToCoords")
	float x = lua_tonumber(L,1);
	float y = lua_tonumber(L,2);
	long agent=GetNearestNPCToCoords(x,y);
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,agent);
	return 1;
}
int LUA_IsAttackedMelee( lua_State* L){
	CHECK(1,"IsAttackedMelee")
	float ID = lua_tonumber(L,1);
	long agent=IsAttackedMelee(ID);
	lua_pop( L, lua_gettop( L ) );
	lua_pushinteger(L,agent);
	return 1;
}
int LUA_SellItem( lua_State* L){
	CHECK(1,"SellItem")
	long ID = lua_tonumber(L,1);
	SellItem(ID);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_BuyItem( lua_State* L){
	CHECK(3,"BuyItem")
	long ID = lua_tonumber(L,1);
	long quantity = lua_tonumber(L,2);
	long value = lua_tonumber(L,3);
	BuyItem(ID,quantity,value);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_WriteWhisper( lua_State* L){
	CHECK(2,"WriteWhisper")
	const wchar_t* msg = (const wchar_t*)lua_tostring(L,1);
	const wchar_t* name = (const wchar_t*)lua_tostring(L,2);
	WriteWhisper(msg,name);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_GetAgentName( lua_State* L){
	CHECK(1,"GetAgentName")
	int ID = lua_tointeger(L,1);
	wchar_t* name = GetAgentName(ID);
	lua_pop( L, lua_gettop( L ) );
	lua_pushstring(L,(const char*)name);
	return 1;
}
int LUA_TargetNearestFoe( lua_State* L){
	TargetNearestFoe();
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_TargetNearestAlly( lua_State* L){
	TargetNearestAlly();
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_TargetNearestItem( lua_State* L){
	TargetNearestItem();
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_TargetCalledTarget( lua_State* L){
	TargetCalledTarget();
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_UseHeroSkill( lua_State* L){
	CHECK(3,"UseHeroSkill")
	long ID = lua_tointeger(L,1);
	long skill = lua_tointeger(L,2);
	long target = lua_tointeger(L,3);
	UseHeroSkill(ID,skill,target);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_CompareCharName( lua_State* L){
	CHECK(1,"CompareCharName")
	wchar_t* name = (wchar_t*)lua_tostring(L,1);
	bool comp = CompareCharName(name);
	lua_pop( L, lua_gettop( L ) );
	lua_pushboolean(L,comp);
	return 1;
}
int LUA_ChangeMaxZoom( lua_State* L){
	CHECK(1,"ChangeMaxZoom")
	float zoom = lua_tonumber(L,1);
	ChangeMaxZoom(zoom);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}






int LUA_IsCasting( lua_State* L){
	ReloadSkillbar();
	bool cast;
	memcpy(&cast,&MySkillbar->Casting,sizeof(bool));
	lua_pop( L, lua_gettop( L ) );
	lua_pushboolean(L,cast);
	return 1;
}
int LUA_SkillRecharge( lua_State* L){
	CHECK(1,"SkillRecharge")
	int ID = lua_tointeger(L,1);
	ReloadSkillbar();
	long recharge;
	memcpy(&recharge,&MySkillbar->Skill[ID-1].Recharge,sizeof(long));
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,recharge);
	return 1;
}
int LUA_SkillAdrenaline( lua_State* L){
	CHECK(1,"SkillAdrenaline")
	int ID = lua_tointeger(L,1);
	ReloadSkillbar();
	long adrenaline;
	memcpy(&adrenaline,&MySkillbar->Skill[ID-1].Adrenaline,sizeof(long));
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,adrenaline);
	return 1;
}
int LUA_GetSkillbarSkillId( lua_State* L){
	CHECK(1,"GetSkillbarSkillId")
	int ID = lua_tointeger(L,1);
	ReloadSkillbar();
	long id;
	memcpy(&id,&MySkillbar->Skill[ID-1].Id,sizeof(long));
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,id);
	return 1;
}
int LUA_GetAgentHP(lua_State* L){
	float hp;
	if(lua_gettop( L )<1){
		lua_pop( L, lua_gettop( L ) );
		memcpy(&hp,&Agents[myId]->HP,sizeof(float));
		lua_pushnumber(L,hp);
		return 1;
	}
	long ID = lua_tonumber(L,1);
	memcpy(&hp,&Agents[ID]->HP,sizeof(float));
	lua_pushnumber(L,hp);
	return 1;
}
int LUA_GetAgentEnergy(lua_State* L){
	float e;
	if(lua_gettop( L )<1){
		lua_pop( L, lua_gettop( L ) );
		memcpy(&e,&Agents[myId]->Energy,sizeof(float));
		lua_pushnumber(L,e);
		return 1;
	}
	long ID = lua_tonumber(L,1);
	memcpy(&e,&Agents[ID]->Energy,sizeof(float));
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,e);
	return 1;
}
int LUA_ChangeTarget(lua_State* L){
	CHECK(1,"ChangeTarget")
	long Target = lua_tonumber(L,1);
	_asm MOV ECX,Target
	_asm MOV EDX,0
	_asm CALL GWMemory.ChangeTargetFunction
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_GetProffessions(lua_State* L){
	CHECK(1,"GetProffesions")
	int ID = lua_tointeger(L,1);
	int prim;
	memcpy(&prim,&Agents[ID]->Primary,sizeof(int));
	int sec;
	memcpy(&sec,&Agents[ID]->Secondary,sizeof(int));
	lua_pop( L, lua_gettop( L ) );
	lua_pushinteger(L,prim);
	lua_pushinteger(L,sec);
	return 2;
}
int LUA_GetPlayerNumberOfAgent(lua_State* L){
	CHECK(1,"GetPlayerNumberOfAgent")
	int ID = lua_tointeger(L,1);
	int num;
	memcpy(&num,&Agents[ID]->PlayerNumber,sizeof(int));
	lua_pop( L, lua_gettop( L ) );
	lua_pushinteger(L,num);
	return 1;
}
int LUA_GetRotationOfAgent(lua_State* L){
	CHECK(1,"GetRotationOfAgent")
	int ID = lua_tointeger(L,1);
	float rot;
	memcpy(&rot,&Agents[ID]->Rotation,sizeof(float));
	rot = rot * 180 / 3.14159265358979323846;
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,rot);
	return 1;
}
int LUA_GetAgentPosition(lua_State* L){
	CHECK(1,"GetAgentPosition")
	int ID = lua_tointeger(L,1);
	float x=0.;
	float y=0.;
	if(Agents[ID]==NULL){
		LuaError("lol");
	}
	memcpy(&x, &Agents[ID]->X, sizeof(float));
	memcpy(&y, &Agents[ID]->Y, sizeof(float));
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,x);
	lua_pushnumber(L,y);
	return 2;
}
int LUA_GetAgentTeamId(lua_State* L){
	CHECK(1,"GetAgentTeamId")
	int ID = lua_tointeger(L,1);
	int team;
	memcpy(&team,&Agents[ID]->TeamId,sizeof(int));
	lua_pop( L, lua_gettop( L ) );
	lua_pushinteger(L,team);
	return 1;
}
int LUA_GetAgentHPPips(lua_State* L){
	CHECK(1,"GetAgentHPPips")
	int ID = lua_tointeger(L,1);
	int pips;
	memcpy(&pips,&Agents[ID]->HPPips,sizeof(int));
	pips =((pips / 0.0038) > 0.0) ? floor((pips / 0.0038) + 0.5) : ceil((pips / 0.0038) - 0.5);//floor((Agents[MsgWParam]->HPPips / 0.0038) + 0.5);
	lua_pop( L, lua_gettop( L ) );
	lua_pushinteger(L,pips);
	return 1;
}
int LUA_GetAgentEffectBitmap(lua_State* L){
	CHECK(1,"GetAgentEffectBitmap")
	int ID = lua_tointeger(L,1);
	byte effects;
	memcpy(&effects,&Agents[ID]->Effects,sizeof(byte));
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,effects);
	return 1;
}
int LUA_GetAgentHexBitmap(lua_State* L){
	CHECK(1,"GetAgentHexBitmap")
	int ID = lua_tointeger(L,1);
	byte hex;
	memcpy(&hex,&Agents[ID]->Hex,sizeof(byte));
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,hex);
	return 1;
}
int LUA_GetAgentType(lua_State* L){
	CHECK(1,"GetAgentType")
	int ID = lua_tointeger(L,1);
	long type;
	memcpy(&type,&Agents[ID]->Type,sizeof(long));
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,type);
	return 1;
}
int LUA_GetAgentLevel(lua_State* L){
	CHECK(1,"GetAgentLevel")
	int ID = lua_tointeger(L,1);
	int lvl;
	memcpy(&lvl,&Agents[ID]->Level,sizeof(int));
	lua_pop( L, lua_gettop( L ) );
	lua_pushinteger(L,lvl);
	return 1;
}
int LUA_GetAgentProperties(lua_State* L){
	CHECK(1,"GetAgentProperties")
	int ID = lua_tointeger(L,1);
	long prop;
	memcpy(&prop,&Agents[ID]->NameProperties,sizeof(long));
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,prop);
	return 1;
}
int LUA_IsAgentAttacking(lua_State* L){
	CHECK(1,"IsAgentAttacking")
	int ID = lua_tointeger(L,1);
	bool atk;
	if(Agents[ID]->ModelState == 0x60||
		Agents[ID]->ModelState == 0x440||
		Agents[ID]->ModelState == 0x460)
	{
		atk = true;
	}else{
		atk=false;
	}
	lua_pop( L, lua_gettop( L ) );
	lua_pushboolean(L,atk);
	return 1;
}
int LUA_IsAgentKnockedDown(lua_State* L){
	CHECK(1,"IsAgentKnockedDown")
	int ID = lua_tointeger(L,1);
	bool kd;
	if(Agents[ID]->ModelState == 0x450){
		kd = true;
	}else{
		kd=false;
	}
	lua_pop( L, lua_gettop( L ) );
	lua_pushboolean(L,kd);
	return 1;
}
int LUA_IsAgentMoving(lua_State* L){
	CHECK(1,"IsAgentMoving")
	int ID = lua_tointeger(L,1);
	bool mov;
	if(Agents[ID]->ModelState == 0x0C||
	   Agents[ID]->ModelState == 0x4C||
	   Agents[ID]->ModelState == 0xCC)
	{
		mov= true;
	}else{
		mov=false;
	}
	lua_pop( L, lua_gettop( L ) );
	lua_pushboolean(L,mov);
	return 1;
}
int LUA_IsAgentCasting(lua_State* L){
	CHECK(1,"IsAgentCasting")
	int ID = lua_tointeger(L,1);
	bool cast;
	if(Agents[ID]->Skill != NULL){
		cast= true;
	}else{
		cast=false;
	}
	lua_pop( L, lua_gettop( L ) );
	lua_pushboolean(L,cast);
	return 1;
}
int LUA_GetAgentAllegiance(lua_State* L){
	CHECK(1,"GetAgentAllegiance")
	int ID = lua_tointeger(L,1);
	int all;
	memcpy(&all,&Agents[ID]->Allegiance,sizeof(int));
	lua_pop( L, lua_gettop( L ) );
	lua_pushinteger(L,all);
	return 1;
}
int LUA_GetBagSize(lua_State* L){
	CHECK(1,"GetBagSize")
	int ID = lua_tointeger(L,1);
	int size=MyItemManager.GetBagSize(ID);
	lua_pop( L, lua_gettop( L ) );
	lua_pushinteger(L,size);
	return 1;
}
int LUA_GetFirstIdKitId(lua_State* L){
	CHECK(1,"GetFirstIdKit")
	int ID = lua_tointeger(L,1);
	int kit = MyItemManager.FindIdKit();
	lua_pop( L, lua_gettop( L ) );
	lua_pushinteger(L,kit);
	return 1;
}
int LUA_IdentifieItemByIndex(lua_State* L){
	CHECK(2,"IdentifieItemByIndex")
	int bag = lua_tointeger(L,1);
	int slot = lua_tointeger(L,2);
	int kit = MyItemManager.FindIdKit();
	int item = MyItemManager.GetItemId(bag,slot);
	if(!kit || !item){
		lua_pop( L, lua_gettop( L ) );
		return 0;
	}
	IdentifyItem(kit,item);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_IdentifieItemByID(lua_State* L){
	CHECK(1,"IdentifieItemByID")
	int kit = MyItemManager.FindIdKit();
	int item = lua_tointeger(L,1);
	if(!kit || !item){
		lua_pop( L, lua_gettop( L ) );
		return 0;
	}
	IdentifyItem(kit,item);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_DepositGold(lua_State* L){
	CHECK(1,"DepositGold")
	int amount = lua_tointeger(L,1);
	int self = GWMemory.MoneySelf();
	int storage = GWMemory.MoneyStorage();
	if(amount == -1){
		if((self + storage) > 1000000){ self = 1000000 - storage; }
		storage += self;
		self = GWMemory.MoneySelf() - self;
	}else{
		storage += amount;
		self -= amount;
	}
	ChangeGold(self, storage);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_WithdrawGold(lua_State* L){
	CHECK(1,"WithdrawGold")
	int amount = lua_tointeger(L,1);
	int self = GWMemory.MoneySelf();
	int storage = GWMemory.MoneyStorage();
	if(amount == -1){
		if((storage - (100000 - self)) < 0){
			self += storage;
			storage = 0;
		}else{
			storage -= 100000 - self;
			self += 100000;
		}
	}else{
		storage -= amount;
		self += amount;
	}
	ChangeGold(self, storage);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_SellItemByIndex(lua_State* L){
	CHECK(2,"SellItemByIndex")
	int bag = lua_tointeger(L,1);
	int slot = lua_tointeger(L,2);
	int ID = MyItemManager.GetItemId(bag, slot);
	if(!SellSessionId || !ID){
		lua_pop( L, lua_gettop( L ) );
		return 0;
	}
	SellItem(ID);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_BuyIDKit(lua_State* L){
	BuyItem(*(long*)(GWMemory.MerchantItems() + 0x10), 1, 100);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_BuySuperiorIDKit(lua_State* L){
	BuyItem(*(long*)(GWMemory.MerchantItems() + 0x14), 1, 500);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_GetItemId(lua_State* L){
	CHECK(2,"GetItemId")
	int bag = lua_tointeger(L,1);
	int slot = lua_tointeger(L,2);
	int ID=MyItemManager.GetItemId(bag, slot);
	int model=MyItemManager.GetItemModelId(bag, slot);
	lua_pop( L, lua_gettop( L ) );
	lua_pushinteger(L,ID);
	lua_pushinteger(L,model);
	return 2;
}
int LUA_GetItemInfo(lua_State* L){
	CHECK(2,"GetItemInfo")
	int bag = lua_tointeger(L,1);
	int slot = lua_tointeger(L,2);
	int rar;
	memcpy(&rar,&MyItemManager.GetItemPtr(bag,slot)->extraItemInfo->rarity,sizeof(int));
	int quant;
	memcpy(&quant,&MyItemManager.GetItemPtr(bag, slot)->quantity,sizeof(int));
		lua_pop( L, lua_gettop( L ) );
	lua_pushinteger(L,rar);
	lua_pushinteger(L,slot);
	return 2;
}
int LUA_UseItemByIndex(lua_State* L){
	CHECK(2,"UseItemByIndex")
	int bag = lua_tointeger(L,1);
	int slot = lua_tointeger(L,2);
	int ID = MyItemManager.GetItemId(bag, slot);
	if(!ID){
		lua_pop( L, lua_gettop( L ) );
		return 0;
	}
	UseItem(ID);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}
int LUA_GetItemLastModifier(lua_State* L){
	CHECK(2,"GetItemLastModifier")
	int bag = lua_tointeger(L,1);
	int slot = lua_tointeger(L,2);
	if(!MyItemManager.GetItemPtr(bag, slot)){lua_pop( L, lua_gettop( L ) );
	return 0;}
	int mod;
	memcpy(&mod,&MyItemManager.GetItemPtr(bag,slot)->extraItemInfo->lastModifier,sizeof(int));
	wchar_t* test;
	memcpy(&test,&MyItemManager.GetItemPtr(bag, slot)->customized,sizeof(wchar_t*));
	lua_pop( L, lua_gettop( L ) );
	lua_pushinteger(L,mod);
	lua_pushstring(L,(const char*)test);
	return 2;
}

int LUA_GetCurrentTargetID( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	int id;
	memcpy(&id,GWMemory.CurrentTarget,sizeof(int));
	lua_pushinteger(L,id);
	return 1;
}
int LUA_GetOwnID( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	int id;
	memcpy(&id,&myId,sizeof(int));
	lua_pushinteger(L,id);
	return 1;
}
int LUA_GetMyMaxHP( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	int hp;
	memcpy(&hp,&Agents[myId]->MaxHP,sizeof(int));
	lua_pushnumber(L,hp);
	return 1;
}
int LUA_GetMyMaxEnergy( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	int e;
	memcpy(&e,&Agents[myId]->Energy,sizeof(int));
	lua_pushnumber(L,e);
	return 1;
}

int LUA_GetMapLoading( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	int map;
	memcpy(&map,&mapLoading,sizeof(int));
	lua_pushnumber(L,map);
	return 1;
}

int LUA_GetMapId( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,GWMemory.MapId());
	return 1;
}
int LUA_GetPing( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,GWMemory.Ping());
	return 1;
}int LUA_GetLoggedIn( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,GWMemory.LoggedIn());
	return 1;
}int LUA_GetDead( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,GWMemory.Dead());
	return 1;
}int LUA_GetCurrentBalthFaction( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,GWMemory.CurrentBalthFaction());
	return 1;
}int LUA_GetMaxBalthFaction( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,GWMemory.MaxBalthFaction());
	return 1;
}int LUA_GetCurrentKurzickFaction( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,GWMemory.CurrentKurzickFaction());
	return 1;
}int LUA_GetMaxKurzickFaction( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,GWMemory.MaxKurzickFaction());
	return 1;
}


int LUA_GetCurrentLuxonFaction( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,GWMemory.CurrentLuxonFaction());
	return 1;
}
int LUA_GetMaxLuxonFaction( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,GWMemory.MaxLuxonFaction());
	return 1;
}
int LUA_GetTitleTreasure( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,GWMemory.TitleTreasure());
	return 1;
}
int LUA_GetTitleLucky( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,GWMemory.TitleLucky());
	return 1;
}
int LUA_GetTitleUnlucky( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,GWMemory.TitleUnlucky());
	return 1;
}
int LUA_GetTitleWisdom( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,GWMemory.TitleWisdom());
	return 1;
}
int LUA_GetMoneySelf( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,GWMemory.MoneySelf());
	return 1;
}
int LUA_GetMoneyStorage( lua_State* L){
	lua_pop( L, lua_gettop( L ) );
	lua_pushnumber(L,GWMemory.MoneyStorage());
	return 1;
}
int LUA_SetSkillbarSkill( lua_State* L){
	CHECK(2,"SetSkillbarSkill");
	dword slot = lua_tonumber(L,1);
	dword id = lua_tonumber(L,2);
	SetSkillbarSkill(slot,id);
	lua_pop( L, lua_gettop( L ) );
	return 0;
}





