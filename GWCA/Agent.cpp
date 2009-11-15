#include "Agent.h"

AgentArray Agents;

void AgentArray::Reload(){
	mpBasePtr = *(reinterpret_cast<dword*>(GWMemory.AgentArrayPtr));
}

Agent* AgentArray::GetAgent(unsigned int aIndex){
	Reload();
	if(mpBasePtr == NULL){return NULL;}
	if(aIndex >= maxAgent){return NULL;}
	Agent* pAgent = (Agent*)(*(dword*)(mpBasePtr + (4 * aIndex)));
	return pAgent;
}