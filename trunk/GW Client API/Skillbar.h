#ifndef _SKILLBAR_H
#define _SKILLBAR_H

/*	Structure definition of the Skillbar and it's associated SkillbarSkill structs.
	Also contains SkillLogSkill struct for the SkillLogQueue. */

/* SKILL TYPES */
#define GW_SKILL_TYPE_STANCES         3
#define GW_SKILL_TYPE_HEXES           4
#define GW_SKILL_TYPE_SPELLS          5
#define GW_SKILL_TYPE_ENCHANTMENTS    6
#define GW_SKILL_TYPE_SIGNETS         7
#define GW_SKILL_TYPE_WELLS           9
#define GW_SKILL_TYPE_SKILLS          10
#define GW_SKILL_TYPE_WARDS           11
#define GW_SKILL_TYPE_GLYPHS          12
#define GW_SKILL_TYPE_ATTACKS         14
#define GW_SKILL_TYPE_SHOUTS          15
#define GW_SKILL_TYPE_PREPARATIONS    19
#define GW_SKILL_TYPE_TRAPS           21
#define GW_SKILL_TYPE_RITUALS         22
#define GW_SKILL_TYPE_ITEM_SPELLS     24
#define GW_SKILL_TYPE_WEAPON_SPELLS   25
#define GW_SKILL_TYPE_CHANTS          27
#define GW_SKILL_TYPE_ECHOS           28
#define GW_SKILL_TYPE_REFRAINS        28

struct SkillType
{
   long Id;
   byte unk[8];
   byte Type;
   byte unk2[3];
   long EliteType[2];
};

struct SkillbarSkill {
	long Adrenaline;
	long Adrenaline2;
	dword Recharge;
	dword Id;
	dword Event;
};

struct SkillLogSkill {
	long AgentId;
	long Skill;
	float Activation;
};

struct Skillbar {
	long AgentId; //This is your own agentId - inferior to the SectionD method
	SkillbarSkill Skill[8]; //Array holding all 8 skills
	byte unknown[12];
	dword Casting; //Bool that is true when you're casting
};

struct Buff {
	long SkillId;		//SkillId of the Buff
	byte unk2[4];
	long BuffId;		//used to cancel the buff
	long BuffedAgent; //the agent that is buffed
};

class BuffHandler{
public:
	BuffHandler(){};
	~BuffHandler(){};

	int GetBuffCount(int Hero){ //Hero 0 = self
		return ReadPtrChain<int>(MySectionA->BasePointer(),0x18,0x2C,0x4A4,(4 * (Hero + (Hero * 8))) + 8) + 1;
	}

	Buff* GetBuff(int Hero,int index){ //Hero 0 = self
		dword* Base = ReadPtrChain<dword*>(MySectionA->BasePointer(),0x18,0x2C,0x4A4,((4 * (Hero + (Hero * 8))) + 4));
		Buff* pBuff = (Buff*)Base + (10 * index);
		return pBuff;
	}

	Buff* GetBuff(long SkillId){
		for(int i=0;i<4;i++)
			for(int j=0;j<GetBuffCount(i);j++)
				if(GetBuff(i,j)->SkillId == SkillId)
					return GetBuff(i,j);
		return NULL;
	}

	bool HasBuff(long AgentId,long SkillId){
		for(int i=0;i<4;i++)
			for(int j=0;j<GetBuffCount(i);j++)
				if(GetBuff(i,j)->SkillId == SkillId && GetBuff(i,j)->BuffedAgent == AgentId)
					return true;
		return false;
	}
};

#endif