/*	Classes used for communication with the client.
	
	Created by Cronos
	Credits:
		GWCA Project for the base to build on
		Google for many nice codesnippets to use
		Gamedeception for the hooking tutorial*/

#if !defined COMMUNICATOR_H
#define COMMUNICATOR_H

#include <Windows.h>
#include <atlbase.h>
#include <list>

#define BUFSIZE 1024
#define TEXTBUFFERLENGTH 504
#define ASSERT(ver) 

union Param_t{
	int 	i_Param;
	float 	f_Param;
	DWORD	d_Param;

	Param_t(){memset(this,0,sizeof(Param_t));}
};

enum message_type
{
	IS_NUMERIC = 0x00,
	IS_TEXT = 0x01,
	IS_COMMAND = 0x02,
	IS_REQUEST = 0x04,
	IS_RESPONSE = 0x08
};
enum GWCA_COMMANDS{
	//Pipe Control
	CA_DisconnectPipe = 0x001, CA_AliveRequest, CA_IsAlive,

	//Error Control
	CA_Error = 0x050,

	//Commands
	CA_CommandsBegin = 0x101,/* CA_TestBeep, CA_TestMessageBox, CA_TestWhisper,*/
	CA_Attack, CA_Dialog, CA_CancelAction, CA_ClearPacketQueue, CA_QuestAbandon, CA_DonateFaction, CA_SetLogAndHwnd, 
	CA_Move, CA_GoNpc, CA_GoPlayer, CA_GoSignpost, CA_GoAgent, 
	CA_UseSkill, CA_SetEventSkillMode, CA_SetSkillbarSkill, CA_SetAttribute, CA_ChangeSecondProfession, 
	CA_ChangeWeaponSet, CA_EquipItem, CA_EquipItemById, CA_UseItem, CA_UseItemById, 
	CA_DropGold, CA_DepositGold, CA_WithdrawGold,
	CA_ChangeTarget, CA_TargetNearestFoe, CA_TargetNearestAlly, CA_TargetNextPartyMember, CA_TargetNextFoe, 
	CA_TargetNearestItem, CA_TargetCalledTarget,
	CA_UseHero1Skill, CA_UseHero2Skill, CA_UseHero3Skill,
	CA_CommandHero1, CA_CommandHero2, CA_CommandHero3, CA_CommandAll, CA_SetHeroMode, 
	CA_AddHero, CA_KickHero, CA_AddNpc, CA_KickNpc, 
	CA_ChangeDistrict, CA_ZoneMap, CA_SwitchMode, CA_InitMapLoad, CA_SkipCinematic, 
	CA_DismissBuff, CA_SendChat, CA_SetTeamSize, CA_FreeMem, 
	CA_Resign, CA_ReturnToOutpost, CA_EnterChallenge, CA_TravelGH, CA_LeaveGH, 
	CA_SetBag, CA_PrepareMoveItem, CA_MoveItem, 
	CA_IdentifyItem, CA_IdentifyItemById, CA_SalvageItem, 
	CA_SellItem, CA_SellItemById, CA_BuyIdKit, CA_BuySuperiorIdKit,
	CA_BuyItem, CA_TraderRequest, CA_TraderRequestSell, CA_TraderRequestSellById, 
	CA_OpenChest, CA_AcceptAllItems, CA_PickupItem, CA_DropItem, CA_DropItemById,
	CA_CommandsEnd,

	//Requests
	CA_RequestsBegin = 0x301, 
	CA_GetCurrentTarget, 
	CA_GetMyId, CA_GetMyMaxHP, CA_GetMyMaxEnergy, CA_GetMyNearestAgent, CA_GetMyDistanceToAgent, 
	CA_Casting, CA_SkillRecharge, CA_SkillAdrenaline, CA_GetSkillbarSkillId, 
	CA_GetTarget, CA_GetAgentAndTargetPtr, 
	CA_GetBuildNumber, CA_ChangeMaxZoom, CA_GetLastDialogId, CA_SetEngineHook, 
	CA_GetGold, CA_GetBagSize, CA_GetItemId, 
	CA_GetIdKit, CA_GetSalvageKit, 
	CA_GetItemInfo, CA_GetItemLastModifier, CA_GetItemLastModifierById, 
	CA_FindItemByModelId, CA_FindEmptySlot, CA_FindGoldItem, 
	CA_GetItemPositionByItemId, CA_GetItemPositionByModelId, CA_GetItemPositionByRarity, 
	CA_GetItemModelIdById, CA_GetItemInfoById, 
	CA_GetItemIdByAgent, CA_GetItemInfoByAgent, CA_GetItemLastModifierByAgent, CA_GetNearestItemByModelId,
	CA_GetMapLoading, CA_GetMapId, CA_MapIsLoaded, CA_GetRegionAndLanguage, CA_GetPing, CA_GetLoggedIn, CA_GetDead, 
	CA_GetBalthFaction, CA_GetKurzFaction, CA_GetLuxonFaction, 
	CA_GetTitleTreasure, CA_GetTitleLucky, CA_GetTitleUnlucky, CA_GetTitleWisdom, CA_GetTitleGamer, CA_GetExperience,
	CA_GetTitleSunspear, CA_GetTitleLightbringer, CA_GetTitleVanguard, CA_GetTitleNorn, CA_GetTitleAsura, CA_GetTitleDeldrimor, 
	CA_GetTitleNorthMastery, CA_GetTitleDrunkard, CA_GetTitleSweet, CA_GetTitleParty, CA_GetTitleCommander, CA_GetTitleLuxon, CA_GetTitleKurzick, 
	CA_GetMapOverlayCoords, CA_GetMapOverlayInfo, CA_GetNearestMapOverlayToCoords, CA_GetPartyInfo, 
	CA_GetAgentExist, CA_GetProfessions, CA_GetPlayerNumber, CA_GetName, CA_GetHP, CA_GetRotation, 
	CA_GetSkill, CA_GetCoords, CA_GetWeaponSpeeds, CA_GetSpiritRange, CA_GetTeamId, CA_GetCombatMode, 
	CA_GetModelMode, CA_GetHpPips, CA_GetEffects, CA_GetHex, CA_GetModelAnimation, CA_GetEnergy, CA_GetAgentPtr, 
	CA_GetType, CA_GetLevel, CA_GetNameProperties, CA_GetMaxId, CA_GetSpeed, CA_GetAllegiance, CA_GetWeaponType, 
	CA_GetModelState, CA_GetIsAttacking, CA_GetIsKnockedDown, CA_GetIsMoving, CA_GetIsDead, CA_GetIsCasting, CA_GetIsAttackedMelee,
	CA_PlayerHasBuff, CA_Hero1HasBuff, CA_Hero2HasBuff, CA_Hero3HasBuff, 
	CA_GetFirstAgentByPlayerNumber, CA_GetNearestAgentToAgent, CA_GetDistanceFromAgentToAgent, 
	CA_GetNearestAgentToAgentEx, CA_GetNearestEnemyToAgentEx, CA_GetNearestItemToAgentEx, 
	CA_GetNearestAgentByPlayerNumber, CA_GetNearestEnemyToAgentByAllegiance, CA_GetNearestAliveEnemyToAgent, 
	CA_GetNearestSignpostToAgent, CA_GetNearestNpcToAgentByAllegiance, CA_GetNearestAgentToCoords,
	CA_GetNearestNpcToCoords, CA_GetLoginNumber, CA_GetNumberOfAgentsByPlayerNumber, CA_GetNumberOfAliveEnemyAgents, CA_GetNextItem, 
	CA_QuestCheck, CA_QuestCoords, CA_QuestActive, CA_AllocMem, CA_TraderCheck, CA_TraderBuy, CA_TraderSell,
	CA_GetItemExtraId, CA_GetItemExtraIdById,
	CA_RequestsEnd
};

struct BaseMessage
{
	unsigned short header;
	unsigned short type;
	Param_t wParam, lParam;
};

typedef struct 
{ 
	OVERLAPPED oOverlap; 
	HANDLE hPipeInst; 
	BaseMessage bmRequest; 
	DWORD cbRead;
	BaseMessage bmReply; 
	DWORD cbWritten;
} PIPEINST, *LPPIPEINST;
typedef void (*HandleMessageFunctionPointer)(WORD, Param_t, Param_t);

template <typename T>
class SimpleQueue {

	typedef T* Tptr;                                // should really be a smart_pointer<T>

private:
	SimpleQueue(const SimpleQueue &);               // Not copyable
	SimpleQueue & operator= (const SimpleQueue &); // Not assignable

	struct Node {
		Node( Tptr val ) : value(val), next(NULL) { }
		Node() : next(NULL) { value = NULL; }
		Tptr value;
		Node* next;
	};

	std::list<Node *> freeList;   // for producer only
	Node* first;                  // for producer only
	Node *divider, *last;         // shared -- Use explicit atomic compares only

	// Allocator/Deallocator for nodes -- 
	// only used in the producer thread
	// OR in the destructor.
	Node * Get(Tptr val)
	{
		if(!freeList.empty())
		{
			// Clean because of Release
			Node * next = freeList.front();
			freeList.pop_front();
			next->value = val;
			return next;
		}

		// clean by construction
		return new Node(val);     
	}

	// Avoids costly free() while running
	void Release(Node * node)
	{
		// reset the node to clean before shelving it
		node->value = NULL;
		node->next = NULL;
		freeList.push_back(node);
	}


public:
	SimpleQueue() {
		first = divider = last = Get(NULL);                         // add dummy separator
	}

	~SimpleQueue() {
		while( first != NULL ) {               // release the list
			Node* tmp = first;
			first = tmp->next;
			delete tmp;
		}

		// Require -- Producer thread calls this or is dead
		while(!freeList.empty())
		{
			delete Get(NULL);
		}
	}

	// Produce is called on the producer thread only:
	void Produce( Tptr t ) {
		last->next = Get(t);                            // add the new item
		InterlockedExchangePointer((void**)&last, last->next);  // publish it

		// Burn the consumed part of the queue
		for( PVOID looper = first;                     // non-null; pointer read is atomic
			InterlockedCompareExchangePointer(&looper, NULL, divider), looper;
			looper = first)
		{
			Node* tmp = first;
			first = first->next;
			Release(tmp);
		}
	}

	// Consume is called on the consumer thread only:
	bool Consume( Tptr & result ) {

		PVOID choice = divider;                                  // non-null; pointer read is atomic
		InterlockedCompareExchangePointer(&choice, NULL, last);

		if(choice)
		{
			result = divider->next->value;                        // C: copy it back
			choice = divider;

			InterlockedExchangePointer((void**)&divider, divider->next);  // D: publish that we took it
			reinterpret_cast<Node*>(choice)->next = NULL;
			return true;                                          // and report success
		}

		return false;                                             // else report empty
	}
};

class GWCAServer
{
public:
	GWCAServer(){
		sprintf_s(m_cPipeName, 30, "\\\\.\\pipe\\GWCA_%lu", GetCurrentProcessId());

		m_HandleRequestsFunction = NULL;

		CreateThread(0, 0, GWCAServer::TListenConnections, this, 0, 0);
		CreateThread(0, 0, GWCAServer::TListenConnectedPipes, this, 0, 0);
	}
	~GWCAServer(){
	};

	void AddCommand(BaseMessage& command)
	{
		BaseMessage* pMessage = new BaseMessage(command);
		m_CommandBuffer.Produce(pMessage);
	}
	bool GetCommand()
	{
		BaseMessage* pMessage;
		//BaseMessage _buffer;
		//if (try_receive<BaseMessage>(&m_CommandBuffer, _buffer))
		if (m_CommandBuffer.Consume(pMessage))
		{
			m_CurrCommand = *pMessage;
			delete pMessage;
			return true;
		}
		return false;
	}
	WORD GetCommandHeader() {return m_CurrCommand.header;}
	Param_t GetCommandWParam(){return m_CurrCommand.wParam;}
	Param_t GetCommandLParam(){return m_CurrCommand.lParam;}

	//void AddRequest(BaseMessage& request)
	//{
	//	BaseMessage* pMessage = new BaseMessage(request);
	//	m_RequestBuffer.Produce(pMessage);
	//}
	//bool GetRequest()
	//{
	//	BaseMessage* pMessage;
	//	//BaseMessage _buffer;
	//	//if (try_receive<BaseMessage>(&m_RequestBuffer, _buffer))
	//	if (m_RequestBuffer.Consume(pMessage))
	//	{
	//		m_CurrRequest = *pMessage;
	//		delete pMessage;
	//		return true;
	//	}
	//	return false;
	//}
	//WORD GetRequestHeader() {return m_CurrRequest.header;}
	//Param_t GetRequestWParam(){return m_CurrRequest.wParam;}
	//Param_t GetRequestLParam(){return m_CurrRequest.lParam;}
	//wchar_t* GetRequestStringParam(){return L"";/*m_CurrRequest.text;*/}

	void SetRequestFunction(HandleMessageFunctionPointer pFunc)
	{
		m_HandleRequestsFunction = pFunc;
	}
	
	void SetResponse( WORD header, Param_t wParam = Param_t(), Param_t lParam = Param_t())
	{
		SetResponseHeader(header);
		SetResponseWParam(wParam);
		SetResponseLParam(lParam);
		SetResponseFlag(IS_NUMERIC | IS_RESPONSE);
		//Respond();
	}

	void ShowPipeName(){MessageBox(0, m_cPipeName, "PIPENAME", MB_OK); }

private:
	//void Respond(){send(&m_ResponseBuffer, m_CurrResponse);}
	void SetResponseHeader( WORD header ){m_CurrResponse.header = header;}
	void SetResponseWParam( Param_t wParam ){m_CurrResponse.wParam = wParam;}
	void SetResponseLParam( Param_t lParam ){m_CurrResponse.lParam = lParam;}
	void SetResponseFlag( int flag ){m_CurrResponse.type = flag;}

	bool ListenConnections();
	HRESULT ListenConnectedPipes();

	static DWORD WINAPI TListenConnections(PVOID thisPointer)
	{
		reinterpret_cast<GWCAServer*>(thisPointer)->ListenConnections();

		return 0;
	}
	static DWORD WINAPI TListenConnectedPipes(PVOID thisPointer)
	{
		reinterpret_cast<GWCAServer*>(thisPointer)->ListenConnectedPipes();

		return 0;
	}

	BaseMessage m_CurrCommand;
	BaseMessage m_CurrRequest;
	BaseMessage m_CurrResponse;

	char m_cPipeName[30];
	HandleMessageFunctionPointer m_HandleRequestsFunction;

	SimpleQueue<BaseMessage> m_CommandBuffer;
};

extern GWCAServer* myGWCAServer;

#endif