#ifndef _AGENT_H
#define _AGENT_H

/*	Structure definition of the Agent struct and it's associated AgentArray class for handling.
	Hint: unknown does in most cases just mean not fully understood yet and/or insignificant.
	Also contains CSectionA class. */

struct Agent {
	byte unknown1[28];
	byte unknown2[4]; //This actually points to the agent before but with a small offset
	byte* NextAgent; //Pointer to the next agent (by id)
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
	byte unknown10[68];
	float WeaponAttackSpeed; //The base attack speed in float of last attacks weapon. 1.33 = axe, sword, daggers etc.
	float AttackSpeedModifier; //Attack speed modifier of the last attack. 0.67 = 33% increase (1-.33)
	word PlayerNumber; //Selfexplanatory. All non-players have identifiers for their type. Two of the same mob = same number
	byte unknown11[20];
	byte Primary; //Primary profession 0-10 (None,W,R,Mo,N,Me,E,A,Rt,P,D)
	byte Secondary; //Secondary profession 0-10 (None,W,R,Mo,N,Me,E,A,Rt,P,D)
	byte Level; //Duh!
	byte TeamId; //0=None, 1=Blue, 2=Red, 3=Yellow
	byte unknown12[14];
	float Energy; //Only works for yourself
	long MaxEnergy; //Only works for yourself
	byte unknown13[4];
	float HPPips; //Regen/degen as float
	byte unknown14[4];
	//Offset +0x130
	float HP; //Health in % where 1=100% and 0=0%
	long MaxHP; //Only works for yourself
	byte Effects; //Bitmap for effects to display when targetted. Does not include hexes
	byte unknown15[7];
	byte Hex; //Bitmap for the hex effect when targetted
	byte unknown16[18];
	long ModelState; //Different values for different states of the model.
	byte CombatMode; //Kinda dodgy, don't rely on it too much.
	byte unknown17[20];
	long InSpiritRange; //Tells if agent is within spirit range of you. Doesn't work anymore?
	byte unknown18[12];
	long LoginNumber; //Unique number in instance that only works for players
	byte unknown19[4];
	float ModelMode; //Float for the current mode the agent is in. Varies a lot
	long ModelAnimation; //Id of the current animation
	byte unknown20[32];
	word Allegiance; //0x100 = ally/non-attackable, 0x300 = enemy, 0x400 = spirit/pet, 0x500 = minion, 0x600 = npc/minipet
	word WeaponType; //1=bow, 2=axe, 3=hammer, 4=daggers, 5=scythe, 6=spear, 7=sword, 10=wand, 12=staff, 14=staff
	//Offset +0x1B4
	word Skill; //0 = not using a skill. Anything else is the Id of that skill
};

class AgentArray {
public:
	void SetBasePtr(dword pBasePtr){
		mpBasePtr = pBasePtr;
	}

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
	dword MerchantItems(){
		return ReadPtrChain<dword>(BasePointer(), 0x18, 0x2C, 0x24);
	}
	dword HeroesStruct(){
		return ReadPtrChain<dword>(BasePointer(), 0x18, 0x2C, 0x520);
	}
	dword GHStruct(){
		return ReadPtrChain<dword>(BasePointer(), 0x18, 0x3C);
	}
};

#endif