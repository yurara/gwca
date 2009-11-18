#include "Skillbar.h"

Skillbar *MySkillbar;

void ReloadSkillbar(){
	MySkillbar = (Skillbar*)(GWMemory.SkillbarPtr());
}