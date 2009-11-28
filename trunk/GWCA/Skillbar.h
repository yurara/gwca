#pragma once
#ifndef _SKILLBAR_H
#define _SKILLBAR_H
#include "stdafx.h"
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

extern Skillbar *MySkillbar;

struct LoggedSkillStruct {
	long AgentId;
	long MyId;
	long SkillId;
	float Activation;
	byte TeamId;
	word Allegiance;
	float Distance;
	long Ping;
};

void ReloadSkillbar();


#endif