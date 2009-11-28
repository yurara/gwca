#include "Functions.h"
long SellSessionId = NULL;
#define NEWPAK(cmd, size)	CPacket* newPak = new CPacket((byte*)(new byte[0x##size##]), 0x##size##); newPak->NewPacket(0x##cmd##,0x##size##); memset(newPak->Buffer + 4, 0, 0x##size## - 4);


void SetSkillbarSkill(dword slot,dword id){
	NEWPAK(53,14);
	newPak->Set<dword>(4,myId);
	newPak->Set<dword>(8,slot-1);
	newPak->Set<dword>(12,id);
	newPak->Set<dword>(16,(dword)0);
	newPak->SendPacket(newPak);
}
void ChangeSecondClass(dword newclass){
	NEWPAK(3A,0C);
	newPak->Set<dword>(4,myId);
	newPak->Set<dword>(8,newclass);
	newPak->SendPacket(newPak);
}
//use on own risk
void IncreaseAttribute(dword Attribute){
	NEWPAK(7,10);
	newPak->Set<dword>(4,myId);
	newPak->Set<dword>(8,(dword)0);
	newPak->Set<dword>(12,Attribute);
	newPak->SendPacket(newPak);
}
void DecreaseAttribute(dword Attribute){
	NEWPAK(6,10);
	newPak->Set<dword>(4,myId);
	newPak->Set<dword>(8,(dword)0);
	newPak->Set<dword>(12,Attribute);
	newPak->SendPacket(newPak);
}
//-------------

void MovePlayer(float nX, float nY, float nZ){
	NEWPAK(37, 10);
	newPak->Set<float>(4, nX);
	newPak->Set<float>(8, nY);
	newPak->Set<float>(12, nZ);
	newPak->SendPacket(newPak);
}

void SendChat(char channel, char* message){
	NEWPAK(5c, 11c);
	newPak->Set<word>(4, channel);
	newPak->SetUString(6, message);
	newPak->SendPacket(newPak);
}

void SendChat(char channel, wchar_t* message){
	NEWPAK(5c, 11c);
	newPak->Set<word>(4, channel);
	newPak->SetUString(6, message);
	newPak->SendPacket(newPak);
}

void MoveMap(dword MapID, dword Region, dword District, dword Language){
	NEWPAK(a9, 18);
	newPak->Set<dword>(4, MapID);
	newPak->Set<dword>(8, Region);
	newPak->Set<dword>(12, District);
	newPak->Set<dword>(16, Language);
	newPak->Set<dword>(20, 1);
	newPak->SendPacket(newPak);
}

void ChangeDistrict(dword Region, dword Language){
	NEWPAK(a9, 18);
	newPak->Set<dword>(4, GWMemory.MapId());
	newPak->Set<dword>(8, Region);
	newPak->Set<dword>(12, 0);
	newPak->Set<dword>(16, Language);
	newPak->Set<dword>(20, 1);
	newPak->SendPacket(newPak);
}

void UseSkill(dword SkillID, dword TargetID, dword Event){
	NEWPAK(3f, 14);
	newPak->Set<dword>(4, SkillID);
	newPak->Set<dword>(8, Event);
	newPak->Set<dword>(12, TargetID);
	newPak->Set<dword>(16, 0);
	newPak->SendPacket(newPak);
}

void UseAttackSkill(dword SkillID, dword TargetID, dword Event){
	NEWPAK(20, 14);
	newPak->Set<dword>(4, SkillID);
	newPak->Set<dword>(8, Event);
	newPak->Set<dword>(12, TargetID);
	newPak->Set<dword>(16, 0);
	newPak->SendPacket(newPak);
}

void _UseSkill(dword SkillID, dword TargetID, dword Event){
	SkillType mySkill = *(SkillType*)(GWMemory.SkillTypeBase + (SkillID * 160));
	if(mySkill.Type == GW_SKILL_TYPE_ATTACKS)
		UseAttackSkill(SkillID, TargetID, Event);
	else
		UseSkill(SkillID, TargetID, Event);
}


void ChangeWeaponSet(dword WeaponSet){
	NEWPAK(2b, 08);
	newPak->Set<dword>(4, WeaponSet);
	newPak->SendPacket(newPak);
}

void PickUpItem(long TargetID){
	NEWPAK(38, 0c);
	newPak->Set<long>(4, TargetID);
	newPak->Set<dword>(8, 0);
	newPak->SendPacket(newPak);
}

void UseItem(dword ItemID){
	NEWPAK(76, 08);
	newPak->Set<dword>(4, ItemID);
	newPak->SendPacket(newPak);
}

void EquipItem(dword ItemID){
	NEWPAK(29, 08);
	newPak->Set<dword>(4, ItemID);
	newPak->SendPacket(newPak);
}

void DropItem(dword ItemID, dword Amount){
	NEWPAK(25, 0C);
	newPak->Set<dword>(4, ItemID);
	newPak->Set<dword>(8, Amount);
	newPak->SendPacket(newPak);
}

void DropGold(dword Amount){
	NEWPAK(28, 08);
	newPak->Set<dword>(4, Amount);
	newPak->SendPacket(newPak);
}

void AttackTarget(long TargetID){
	NEWPAK(1f, 0c);
	newPak->Set<long>(4, TargetID);
	newPak->Set<dword>(8, 0);
	newPak->SendPacket(newPak);
}

void CommandHero(long id, float nX, float nY){
	NEWPAK(12,14);
	newPak->Set<long>(4, id);
	newPak->Set<float>(8, nX);
	newPak->Set<float>(12, nY);
	newPak->Set<float>(16, 0);
	newPak->SendPacket(newPak);
}

void CommandAll(float nX, float nY){
	NEWPAK(13,10);
	newPak->Set<float>(4, nX);
	newPak->Set<float>(8, nY);
	newPak->Set<float>(12, 0);
	newPak->SendPacket(newPak);
}

void GetQuest(byte id){
	NEWPAK(34,08);
	newPak->Set<byte>(4,0x01);
	newPak->Set<byte>(5,id);
	newPak->Set<byte>(6,0x81);
	newPak->SendPacket(newPak);
}

void QuestReward(byte id){
	NEWPAK(34,08);
	newPak->Set<byte>(4,0x06);
	newPak->Set<byte>(5,id);
	newPak->Set<byte>(6,0x81);
	newPak->SendPacket(newPak);
}

void Dialog(long pick){
	NEWPAK(34,08);
	newPak->Set<long>(4, pick);
	newPak->SendPacket(newPak);
}

void ExchangeAmber(){
	NEWPAK(34,08);
	newPak->Set<byte>(4,0x01);
	newPak->Set<byte>(5,0x01);
	newPak->Set<byte>(6,0x80);
	newPak->SendPacket(newPak);
}

void GoNPC(long id){
	NEWPAK(32,0c);
	newPak->Set<long>(4,id);
	newPak->Set<dword>(8,0);
	newPak->SendPacket(newPak);
}

void GoPlayer(long id){
	NEWPAK(2c,0c);
	newPak->Set<long>(4,id);
	newPak->Set<dword>(8,0);
	newPak->SendPacket(newPak);
}

void DonateFaction(){
	NEWPAK(2e,10);
	newPak->Set<dword>(12,0x88);
	newPak->Set<byte>(13,0x13);
	newPak->SendPacket(newPak);
}

void OpenChest(){
	NEWPAK(4c,08);
	newPak->Set<byte>(4,2);
	newPak->SendPacket(newPak);
}

void GoSignpost(long id){
	NEWPAK(4a,0c);
	newPak->Set<long>(4,id);
	newPak->Set<dword>(8,0);
	newPak->SendPacket(newPak);
}

void EnterChallenge(){
	NEWPAK(9d,08);
	newPak->Set<byte>(4,1);
	newPak->SendPacket(newPak);
}

void CancelAction(){
	NEWPAK(21,04);
	newPak->SendPacket(newPak);
}

void IdentifyItem(long idKit, long itemId){
	NEWPAK(64,0c);
	newPak->Set<long>(4, idKit);
	newPak->Set<long>(8, itemId);
	newPak->SendPacket(newPak);
}

void MoveItem(long itemId, long bagId, long slot){
	NEWPAK(6a, 10);
	newPak->Set<long>(4, itemId);
	newPak->Set<long>(8, bagId);
	newPak->Set<long>(12, slot);
	newPak->SendPacket(newPak);
}

void ChangeGold(long goldSelf, long goldStorage){
	NEWPAK(74,0c);
	newPak->Set<long>(4, goldSelf);
	newPak->Set<long>(8, goldStorage);
	newPak->SendPacket(newPak);
}

void ReturnToOutpost(){
	NEWPAK(9f,04);
	newPak->SendPacket(newPak);
}

void AcceptAllItems(long bagId){
	NEWPAK(6b,08);
	newPak->Set<long>(4, bagId);
	newPak->SendPacket(newPak);
}

void AddHero(long HeroId){
	NEWPAK(16,08);
	newPak->Set<long>(4, HeroId);
	newPak->SendPacket(newPak);
}

void KickHero(long HeroId){
	NEWPAK(17,08);
	newPak->Set<long>(4, HeroId);
	newPak->SendPacket(newPak);
}

void SwitchMode(long NMHM){
	NEWPAK(93,08);
	newPak->Set<long>(4, NMHM);
	newPak->SendPacket(newPak);
}

void AddNpc(long NpcId){
	NEWPAK(97,08);
	newPak->Set<long>(4, NpcId);
	newPak->SendPacket(newPak);
}

void KickNpc(long NpcId){
	NEWPAK(a0,08);
	newPak->Set<long>(4, NpcId);
	newPak->SendPacket(newPak);
}

void TravelGH(){
	NEWPAK(a8,18);
	newPak->Set<dword>(4, *(dword*)(GWMemory.GHStruct() + 0x64));
	newPak->Set<dword>(8, *(dword*)(GWMemory.GHStruct() + 0x68));
	newPak->Set<dword>(12, *(dword*)(GWMemory.GHStruct() + 0x6c));
	newPak->Set<dword>(16, *(dword*)(GWMemory.GHStruct() + 0x70));
	newPak->Set<dword>(20, 1);
	newPak->SendPacket(newPak);
}

void LeaveGH(){
	NEWPAK(aa,08);
	newPak->Set<long>(4, 1);
	newPak->SendPacket(newPak);
}

long GetNearestAgentToAgent(long agentId){
	if(Agents[agentId] == NULL){return 0;}

	float* aDistance = new float[maxAgent+1];
	aDistance[0] = 50000;
	long lLowest = 0;
	__try {
	for(unsigned int i = 1;i < maxAgent;i++){
		aDistance[i] = GetDistanceFromAgentToAgent(agentId, i);
		if(aDistance[i] < aDistance[lLowest] && aDistance[i] != 0){lLowest = i;}
	}
	}
   __except(1) {
      return false;
   }
	return lLowest;
}

long GetNearestEnemyToAgent(long agentId){
	if(Agents[agentId] == NULL){return 0;}

	float* aDistance = new float[maxAgent+1];
	aDistance[0] = 50000;
	long lLowest = 0;
	__try {
	for(unsigned int i = 1;i < maxAgent;i++){
		if(Agents[i] == NULL){continue;}
		if(Agents[agentId]->TeamId != Agents[i]->TeamId){
			aDistance[i] = GetDistanceFromAgentToAgent(agentId, i);
			if(aDistance[i] < aDistance[lLowest] && aDistance[i] != 0){lLowest = i;}
		}
	}
	}
   __except(1) {
      return false;
   }
	return lLowest;
}

long GetNearestEnemyToAgentByAllegiance(long agentId){
	if(Agents[agentId] == NULL){return 0;}

	float* aDistance = new float[maxAgent+1];
	aDistance[0] = 500000;
	long lLowest = 0;
	__try {
	for(unsigned int i = 1;i < maxAgent;i++){
		if(Agents[i] == NULL){continue;}
		if(Agents[i]->Allegiance == 0x300){
			aDistance[i] = GetDistanceFromAgentToAgent(agentId, i);
			if(aDistance[i] < aDistance[lLowest] && aDistance[i] != 0){lLowest = i;}
		}
	}
	}
   __except(1) {
      return false;
   }
	return lLowest;
}

long GetNearestAliveEnemyToAgent(long agentId){
	if(Agents[agentId] == NULL){return 0;}

	float* aDistance = new float[maxAgent+1];
	aDistance[0] = 50000;
	long lLowest = 0;
	__try {
	for(unsigned int i = 1;i < maxAgent;i++){
		if(Agents[i] == NULL){continue;}
		if(Agents[i]->Allegiance == 0x300 && Agents[i]->HP != 0){
			aDistance[i] = GetDistanceFromAgentToAgent(agentId, i);
			if(aDistance[i] < aDistance[lLowest] && aDistance[i] != 0){lLowest = i;}
		}
	}
	}
   __except(1) {
      return false;
   }
	return lLowest;
}

long GetNearestItemToAgent(long agentId){
	if(Agents[agentId] == NULL){return 0;}

	float* aDistance = new float[maxAgent+1];
	aDistance[0] = 50000;
	long lLowest = 0;
	__try {
	for(unsigned int i = 1;i < maxAgent;i++){
		if(Agents[i] == NULL){continue;}
		if(Agents[i]->Type == 0x400){
			aDistance[i] = GetDistanceFromAgentToAgent(agentId, i);
			if(aDistance[i] < aDistance[lLowest] && aDistance[i] != 0){lLowest = i;}
		}
	}
	}
   __except(1) {
      return false;
   }
	return lLowest;
}

long GetNearestSignpostToAgent(long agentId){
	if(Agents[agentId] == NULL){return 0;}

	float* aDistance = new float[maxAgent+1];
	aDistance[0] = 50000;
	long lLowest = 0;
	__try {
	for(unsigned int i = 1;i < maxAgent;i++){
		if(Agents[i] == NULL){continue;}
		if(Agents[i]->Type == 0x200){
			aDistance[i] = GetDistanceFromAgentToAgent(agentId, i);
			if(aDistance[i] < aDistance[lLowest] && aDistance[i] != 0){lLowest = i;}
		}
	}
	}
   __except(1) {
      return false;
   }
	return lLowest;
}

long GetNearestNpcToAgentByAllegiance(long agentId){
	if(Agents[agentId] == NULL){return 0;}

	float* aDistance = new float[maxAgent+1];
	aDistance[0] = 50000;
	long lLowest = 0;
	__try {
	for(unsigned int i = 1;i < maxAgent;i++){
		if(Agents[i] == NULL){continue;}
		if(Agents[i]->Allegiance == 0x600){
			aDistance[i] = GetDistanceFromAgentToAgent(agentId, i);
			if(aDistance[i] < aDistance[lLowest] && aDistance[i] != 0){lLowest = i;}
		}
	}
	}
   __except(1) {
      return false;
   }
	return lLowest;
}

float GetDistanceFromAgentToAgent(long agent1, long agent2){
	if(Agents[agent1] == NULL){return 0;}
	if(Agents[agent2] == NULL){return 0;}
	if(agent1 == agent2){return 0;}

	return sqrt(pow((Agents[agent2]->Y - Agents[agent1]->Y), 2) + pow((Agents[agent2]->X - Agents[agent1]->X), 2));
}
float GetAngleFromAgentToAgent(long agent1, long agent2){
	if(Agents[agent1] == NULL){return 0;}
	if(Agents[agent2] == NULL){return 0;}
	if(agent1 == agent2){return 0;}

	return atan2(Agents[agent2]->Y - Agents[agent1]->Y,Agents[agent2]->X - Agents[agent1]->X);
}
long GetFirstAgentByPlayerNumber(word playerNum){
	__try {
	for(unsigned int i = 1;i < maxAgent;i++){
		if(Agents[i] == NULL){continue;}
		if(Agents[i]->Type == 0x200 || Agents[i]->Type == 0x400){continue;}
		if(Agents[i]->PlayerNumber == playerNum){return i;}
	}
	}
   __except(1) {
      return false;
   }
	return NULL;
}

long GetNearestAgentByPlayerNumber(word playerNum){
	float* aDistance = new float[maxAgent+1];
	aDistance[0] = 50000;
	long lLowest = 0;
	__try {
	for(unsigned int i = 1;i < maxAgent;i++){
		if(Agents[i] == NULL){continue;}
		if(Agents[i]->Type == 0x200 || Agents[i]->Type == 0x400){continue;}
		if(Agents[i]->PlayerNumber == playerNum){
			aDistance[i] = GetDistanceFromAgentToAgent(myId, i);
			if(aDistance[i] < aDistance[lLowest] && aDistance[i] != 0){lLowest = i;}
		}
	}
	}
   __except(1) {
      return false;
	}
	return lLowest;
}

long GetNearestAgentToCoords(float x, float y){
	float* aDistance = new float[maxAgent+1];
	aDistance[0] = 50000;
	long lLowest = 0;
	__try {
	for(unsigned int i = 1;i < maxAgent;i++){
		if(Agents[i] == NULL){continue;}
		aDistance[i] = sqrt(pow((Agents[i]->Y - y), 2) + pow((Agents[i]->X - x), 2));
		if(aDistance[i] < aDistance[lLowest]){lLowest = i;}
	}
	}
   __except(1) {
      return false;
	 }
	return lLowest;
}

long GetNearestNPCToCoords(float x, float y){
   float* aDistance = new float[maxAgent+1];
   aDistance[0] = 50000;
   long lLowest = 0;

   __try {
   for(unsigned int i = 1;i < maxAgent;i++){
      if(Agents[i] == NULL){continue;}
      if(Agents[i]->Allegiance != 0x600){continue;}
      aDistance[i] = sqrt(pow((Agents[i]->Y - y), 2) + pow((Agents[i]->X - x), 2));
      if(aDistance[i] < aDistance[lLowest]){lLowest = i;}
   }
   }
   __except(1) {
      return false;
   }
   return lLowest;
}

int IsAttackedMelee(long agentId){
	if(Agents[agentId] == NULL){return -10;}

	int points = 0;
	long lNear = GetNearestEnemyToAgent(agentId);

	if(Agents[lNear] == NULL){return -10;}
	if(!Agents[agentId]->HP){return -10;}

	if(Agents[lNear]->Primary == 1||
		Agents[lNear]->Primary == 7||
		Agents[lNear]->Primary == 10||
		Agents[lNear]->Primary == 0)
	{
		//meh
	}else{
		return -10;
	}

	if(Agents[agentId]->ModelState == 0x450){
		points += 2;
	}

	if(Agents[lNear]->ModelState == 0x60|| //attacking
		Agents[lNear]->ModelState == 0x440||
		Agents[lNear]->ModelState == 0x460)
	{
		points += 2;
	}else if(Agents[lNear]->ModelState == 0x0C|| //moving
		Agents[lNear]->ModelState == 0x4C||
		Agents[lNear]->ModelState == 0xCC)
	{
		points += 1;
	}else if(Agents[lNear]->ModelState == 0x450){ //knocked down
		points -= 3;
	}

	float fDistance = GetDistanceFromAgentToAgent(agentId, lNear);
	if(fDistance < 80){ //extremely close
		points += 3;
	}else if(fDistance < 100){ //very close
		points += 2;
	}else if(fDistance < 150){ //still reachable
		points += 1;
	}else if(fDistance > 250){ //not of importance
		points -= 2;
	}

	if(Agents[lNear]->Skill != NULL){ //attack skills?
		points += 2;
	}

	if(Agents[lNear]->WeaponAttackSpeed < 1.34 && //axe,sword,daggers
		Agents[lNear]->WeaponAttackSpeed > 1.33)
	{
		points += 1;
	}else if(Agents[lNear]->WeaponAttackSpeed == 1.75) //hammer
	{
		points += 1;
	}

	if(Agents[lNear]->AttackSpeedModifier < 0.68 && //frenzy/flail/primal rage!
		Agents[lNear]->AttackSpeedModifier > 0.66)
	{
		points += 2;
	}else if(Agents[lNear]->AttackSpeedModifier < 0.84){
		points += 1;
	}

	if(Agents[lNear]->HP == 0){ //dead? :O
		return -10;
	}

	return points;
}
void SellItem(long itemId){
	SellItemStruct* pSell = new SellItemStruct;
	pSell->sessionId = SellSessionId;
	pSell->itemId = itemId;

	_asm {
		MOV ECX,pSell
		CALL GWMemory.SellItemFunction
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
		CALL GWMemory.BuyItemFunction
	}
}

void WriteWhisper(const wchar_t* chatMsg, const wchar_t* chatName){
	 _asm {
		MOV EAX,chatMsg
		MOV EDX,chatName
		MOV ECX,0
		PUSH EAX
		CALL GWMemory.WriteWhisperStart
		MOV EAX,1
	}
}

wchar_t* GetAgentName(int agentId){
	_asm {
		MOV ECX,agentId
		CALL GWMemory.AgentNameFunction
		ADD EAX,4
	}
}

void TargetNearestFoe(){
	_asm {
		MOV EBX,0
		MOV EAX,GWMemory.TargetFunctions
		CALL EAX
	}
}

void TargetNearestAlly(){
	_asm {
		MOV EBX,0
		MOV EAX,GWMemory.TargetFunctions
		ADD EAX,0x1D
		CALL EAX
	}
}

void TargetNearestItem(){
	_asm {
		MOV EBX,0
		MOV EAX,GWMemory.TargetFunctions
		ADD EAX,0x3A
		CALL EAX
	}
}

void TargetCalledTarget(){
	_asm {
		MOV EBX,0
		MOV EAX,GWMemory.TargetFunctions
		ADD EAX,0x115
		CALL EAX
	}
}

void UseHeroSkill(long HeroId, long SkillNumber, long Target){
	_asm {
		MOV EDX,SkillNumber
		MOV ECX,HeroId
		PUSH Target
		CALL GWMemory.HeroSkillFunction
	}
}

bool CompareAccName(wchar_t* cmpName){
	if(wcscmp(cmpName, GWMemory.Email()) == NULL)
		return true;
	else
		return false;
}

bool CompareCharName(wchar_t* cmpName){
	if(wcscmp(cmpName, GWMemory.Name()) == NULL)
		return true;
	else
		return false;
}

void ChangeMaxZoom(float fZoom){
	DWORD dwOldProtection;
	VirtualProtect(GWMemory.MaxZoomStill, sizeof(float), PAGE_EXECUTE_READWRITE, &dwOldProtection);
	memcpy(GWMemory.MaxZoomStill, &fZoom, sizeof(float));
	VirtualProtect(GWMemory.MaxZoomStill, sizeof(float), dwOldProtection, NULL);
	VirtualProtect(GWMemory.MaxZoomMobile, sizeof(float), PAGE_EXECUTE_READWRITE, &dwOldProtection);
	memcpy(GWMemory.MaxZoomMobile, &fZoom, sizeof(float));
	VirtualProtect(GWMemory.MaxZoomStill, sizeof(float), dwOldProtection, NULL);
}
