#ifndef _AGENT_H
#define _AGENT_H

/*	Structure definition of the Agent struct and it's associated AgentArray class for handling.
	Hint: unknown does in most cases just mean not fully understood yet and/or insignificant.
	Also contains CSectionA class. */

struct Agent {
	byte* vtable;
	byte unknown1[24];
	byte unknown2[4]; //This actually points to the agent before but with a small offset
	Agent* NextAgent; //Pointer to the next agent (by id)
	byte unknown3[8];
	long Id; //AgentId
	float Z; //Z coord in float
	byte unknown4[8];
	float BoxHoverWidth; //Width of the model's box
	float BoxHoverHeight; //Height of the model's box
	byte unknown5[8];
	float Rotation; //Rotation in radians from East (-pi to pi)
	byte unknown6[8];
	long NameProperties; //Bitmap basically telling what the agent is
	byte unknown7[24];
	float X; //X coord in float
	float Y; //Y coord in float
	byte unknown8[8];
	float NameTagX; //Exactly the same as X above
	float NameTagY; //Exactly the same aswell
	float NameTagZ; //Z coord in float (actually negated)
	byte unknown9[12];
	long Type; //0xDB = players, npc's, monsters etc. 0x200 = signpost/chest/object (unclickable). 0x400 = item to pick up
	float MoveX; //If moving, how much on the X axis per second
	float MoveY; //If moving, how much on the Y axis per second
	//byte unknown10[68];
	byte unknown10[40];
	long ExtraType;
	byte unknown11[24];
	float WeaponAttackSpeed; //The base attack speed in float of last attacks weapon. 1.33 = axe, sword, daggers etc.
	float AttackSpeedModifier; //Attack speed modifier of the last attack. 0.67 = 33% increase (1-.33)
	word PlayerNumber; //Selfexplanatory. All non-players have identifiers for their type. Two of the same mob = same number
	byte unknown12[6];
	Equipment** Equip;
	byte unknown13[10];
	byte Primary; //Primary profession 0-10 (None,W,R,Mo,N,Me,E,A,Rt,P,D)
	byte Secondary; //Secondary profession 0-10 (None,W,R,Mo,N,Me,E,A,Rt,P,D)
	byte Level; //Duh!
	byte TeamId; //0=None, 1=Blue, 2=Red, 3=Yellow
	byte unknown14[14];
	float Energy; //Only works for yourself
	long MaxEnergy; //Only works for yourself
	byte unknown15[4];
	float HPPips; //Regen/degen as float
	byte unknown16[4];
	//Offset +0x130
	float HP; //Health in % where 1=100% and 0=0%
	long MaxHP; //Only works for yourself
	long Effects; //Bitmap for effects to display when targetted. DOES include hexes
	byte unknown17[4];
	byte Hex; //Bitmap for the hex effect when targetted (apparently obsolete!)
	byte unknown18[18];
	long ModelState; //Different values for different states of the model.
	long TypeMap; //Odd variable! 0x08 = dead, 0xC00 = boss, 0x40000 = spirit, 0x400000 = player
	byte unknown19[16];
	long InSpiritRange; //Tells if agent is within spirit range of you. Doesn't work anymore?
	byte unknown20[16];
	long LoginNumber; //Unique number in instance that only works for players
	float ModelMode; //Float for the current mode the agent is in. Varies a lot
	byte unknown21[4];
	long ModelAnimation; //Id of the current animation
	byte unknown22[32];
	word Allegiance; //0x100 = ally/non-attackable, 0x300 = enemy, 0x400 = spirit/pet, 0x500 = minion, 0x600 = npc/minipet
	word WeaponType; //1=bow, 2=axe, 3=hammer, 4=daggers, 5=scythe, 6=spear, 7=sword, 10=wand, 12=staff, 14=staff
	//Offset +0x1B4
	word Skill; //0 = not using a skill. Anything else is the Id of that skill
	byte unknown23[4];
	word WeaponItemId;
	word OffhandItemId;
};

struct AgentMovement {
	byte* vtable;
	byte unknown1[12];
	long AgentId;
	byte unknown2[40];
	long Moving1; //tells if you are stuck even if your client doesn't know
	byte unknown3[8];
	long Moving2; //exactly same as Moving1
	byte unknown4[28];
	float X2;
	float Y2;
	byte unknown5[8];
	float X;
	float Y;
	//rest is shit - didn't bother to document
};

class AgentArray {
public:
	void Reload(){
		mpBasePtr = *(reinterpret_cast<dword*>(AgentArrayPtr));
	}

	Agent* GetAgent(unsigned int aIndex){
		Reload();
		if(mpBasePtr == NULL){return NULL;}
		if(aIndex >= maxAgent){return NULL;}
		Agent* pAgent = (Agent*)(*(dword*)(mpBasePtr + (4 * aIndex)));
		return pAgent;
	}

	Agent* operator[](unsigned int aIndex){
		Agent* pAgent = GetAgent(aIndex);
		return pAgent;
	}

private:
	dword mpBasePtr;
};

class AgentMovementArray {
public:
	void Reload(){
		mpBasePtr = ReadPtrChain<dword>(*(dword*)(BasePointerLocation), 0x18, 0x8, 0xE8);
	}

	AgentMovement* GetAgent(unsigned int aIndex){
		Reload();
		if(mpBasePtr == NULL){return NULL;}
		if(aIndex >= maxAgent){return NULL;}
		AgentMovement* pAgent = (AgentMovement*)(*(dword*)(mpBasePtr + (4 * aIndex)));
		return pAgent;
	}

	AgentMovement* operator[](unsigned int aIndex){
		AgentMovement* pAgent = GetAgent(aIndex);
		return pAgent;
	}

private:
	dword mpBasePtr;
};

class CSectionA {
public:
	long MapId(){
		return *(long*)(MapIdLocation);
	}
	long Ping(){
		return *(long*)(PingLocation);
	}
	long LoggedIn(){
		return *(long*)(LoggedInLocation);
	}
	wchar_t* Name(){
		return (wchar_t*)(NameLocation);
	}
	wchar_t* Email(){
		return (wchar_t*)(EmailLocation);
	}
	long Dead(){
		return *(long*)(DeadLocation);
	}
	dword BasePointer(){
		return *(dword*)(BasePointerLocation);
	}
	dword SkillbarPtr(){
		return ReadPtrChain<dword>(BasePointer(), 0x18, 0x2C, 0x68C);
	}
	long CurrentBalthFaction(){
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x724);
	}
	long MaxBalthFaction(){
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x74C);
	}
	long CurrentKurzickFaction(){
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x6E4);
	}
	long MaxKurzickFaction(){
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x744);
	}
	long CurrentLuxonFaction(){
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x6F4);
	}
	long MaxLuxonFaction(){
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x748);
	}
	long MoneySelf(){
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x40, 0xF8, 0x7C);
	}
	long MoneyStorage(){
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x40, 0xF8, 0x80);
	}
	Bag** BagArrayPointer(){
		return ReadPtrChain<Bag**>(BasePointer(), 0x18, 0x40, 0xF8);
	}
	long TitleTreasure(){ //credits to ddarek
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x70C);
	}
	long TitleLucky(){ //credits to ddarek
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x25C);
	}
	long TitleUnlucky(){ //credits to ddarek
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x284);
	}
	long TitleWisdom(){ //credits to ddarek
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x734);
	}
	long TitleDrunkard(){ //credits to ddarek
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x11C);
	}
    long TitleSunspear(){ //credits to ddarek
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x2AC);
	}
    long TitleVanguard(){ //credits to ddarek
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x644);
	}
    long TitleNorn(){ //credits to ddarek
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x66C);
	}
    long TitleAsura(){ //credits to ddarek
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x5F4);
	}
    long TitleDeldrimor(){ //credits to ddarek
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x61C);
	}
    long TitleNorthMastery(){ //credits to ddarek
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x694);
    }
    long TitleLightbringer(){ //credits to ddarek
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x324);
	}
	long TitleSweet(){ //credits to ddarek
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x554);
	}
    long TitleParty(){ //credits to ddarek
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x6BC);
	}
    long TitleCommander(){ //credits to ddarek
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x374);
	}
    long TitleTrackLuxon(){ //total points transfered to Luxon's - credits to ddarek
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0xF4);
	}
    long TitleTrackKurzick(){ //total points transfered to Kurzick's - credits to ddarek
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0xCC);
	}
	long TitleGamer(){
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x39C);
	}
	long Experience(){
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x6DC);
	}
	dword MerchantItems(){
		return ReadPtrChain<dword>(BasePointer(), 0x18, 0x2C, 0x24);
	}
	unsigned long MerchantItemsSize(){
		return ReadPtrChain<unsigned long>(BasePointer(), 0x18, 0x2C, 0x28);
	}
	dword HeroesStruct(){
		return ReadPtrChain<dword>(BasePointer(), 0x18, 0x2C, 0x520);
	}
	dword GHStruct(){
		return ReadPtrChain<dword>(BasePointer(), 0x18, 0x3C);
	}
	MapOverlay* MapOverlayPointer(){
		return ReadPtrChain<MapOverlay*>(BasePointer(), 0x18, 0x2C, 0x774);
	}
	long MapOverlaySize(){
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x77C);
	}
	Quest* QuestLogPointer(){
		return ReadPtrChain<Quest*>(BasePointer(), 0x18, 0x2C, 0x4C8);
	}
	long QuestLogSize(){
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x4D0);
	}
	long ActiveQuest(){
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x4C4);
	}
	long* ActiveQuestPtr(){
		return (ReadPtrChain<long*>(BasePointer(), 0x18, 0x2C) + 0x4C4);
	}
	long ItemArraySize(){
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x40, 0xC0);
	}
	Item** ItemArray(){
		return ReadPtrChain<Item**>(BasePointer(), 0x18, 0x40, 0xB8);
	}
	long Region(){
		return *(long*)(RegionLocation);
	}
	long Language(){
		return *(long*)(LanguageLocation + 0xC);
	}
	long Connection(){
		return *(long*)(ConnectionLocation);
	}
	long SalvageSessionId(){
		return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x62C);
	}
	dword MapBoundariesPtr(){
		return ReadPtrChain<dword>(BasePointer(), 0x18, 0x14);
	}
	long MatchStatus(){
		return *(long*)(MatchDoneLocation);
	}
};

#endif