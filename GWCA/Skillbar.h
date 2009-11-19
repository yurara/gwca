
#pragma once
#ifndef _SKILLBAR_H
#define _SKILLBAR_H
#include "stdafx.h"
/*	Structure definition of the Skillbar and it's associated SkillbarSkill structs.
	Also contains SkillLogSkill struct for the SkillLogQueue. */

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