/*	Classes used for communication with the client.
	
	Created by Cronos
	Credits:
		GWCA Project for the base to build on
		Google for many nice codesnippets to use
		Gamedeception for the hooking tutorial*/

#if !defined COMMUNICATOR_H
#define COMMUNICATOR_H

#include <Windows.h>
#include <queue>

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
	CA_BuyItem, CA_TraderRequest, CA_TraderRequestSell, CA_TraderRequestSellById, CA_TraderBuy, CA_TraderSell,
	CA_OpenChest, CA_AcceptAllItems, CA_PickupItem, CA_DropItem, CA_DropItemById, CA_OpenStorage,
	CA_UpdateAgentPosition, CA_MoveOld, CA_TradePlayer, CA_SubmitOffer, CA_ChangeOffer, CA_OfferItem,
	CA_CancelTrade, CA_AcceptTrade, CA_ResetAttributes, CA_SetEquipmentAgent, CA_SetEquipmentModelId, CA_SetEquipmentDye,
	CA_SetEquipmentShinyness,
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
	CA_QuestCheck, CA_QuestCoords, CA_QuestActive, CA_AllocMem, CA_TraderCheck,
	CA_GetItemExtraId, CA_GetItemExtraIdById, CA_GetConnection,
	CA_GetItemExtraIdByAgent, CA_GetItemReq, CA_GetItemReqById, CA_GetItemReqByAgent, CA_GetDyePositionByColor,
	CA_GetNumberOfFoesInRangeOfAgent, CA_GetNumberOfAlliesInRangeOfAgent, CA_GetNumberOfItemsInRangeOfAgent,
	CA_GetAgentMovementPtr, CA_GetMapBoundariesPtr, CA_GetEffectCount, CA_GetEffect, CA_GetEffectByIndex, CA_GetEffectDuration, CA_GetTimeStamp,
	CA_GetAgentDanger, CA_GetTypeMap, CA_GetAgentWeapons, CA_GetMatchStatus,
	CA_GetNextAgent, CA_GetNextAlly, CA_GetNextFoe, CA_GetItemDmgMod, CA_GetItemDmgModById, CA_GetItemDmgModByAgent,
	CA_GetEquipmentModelId, CA_GetEquipmentDyeInfo, CA_GetExtraType, 
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

class AutoHandle
{
public:
	AutoHandle()
		: m_h(NULL)
	{
	}
	AutoHandle(AutoHandle& other)
		: m_h(NULL)
	{
		Attach(other.Detach());
	}
	AutoHandle(HANDLE h)
		: m_h(h)
	{
	}
	~AutoHandle()
	{
		if (m_h != NULL)
		{
			Close();
		}
	}

	void Attach(HANDLE h)
	{
		m_h = h;
	}
	HANDLE Detach()
	{
		HANDLE h = m_h;

		m_h = NULL;

		return h;
	}
	void Close()
	{
		if (m_h != NULL)
		{
			CloseHandle(m_h);
		}
	}
protected:
	bool m_closeHandle;
	HANDLE m_h;
};

template<typename Data>
class concurrent_queue
{
private:
    std::queue<Data> the_queue;
	mutable HANDLE the_mutex;
public:
	concurrent_queue()
	{
		the_mutex = CreateMutex(NULL, false, NULL);
	}
	~concurrent_queue()
	{
		if (the_mutex != NULL)
		{
			CloseHandle(the_mutex);
		}
	}

    void push(Data const& data)
    {
		WaitForSingleObject(the_mutex, INFINITE);
        the_queue.push(data);
		ReleaseMutex(the_mutex);
    }

    bool empty() const
    {
		WaitForSingleObject(the_mutex, INFINITE);
		bool empty = the_queue.empty();
		ReleaseMutex(the_mutex);
        return empty;
    }

    bool try_pop(Data& popped_value)
    {
		WaitForSingleObject(the_mutex, INFINITE);
        if(the_queue.empty())
        {
			ReleaseMutex(the_mutex);
            return false;
        }
        
        popped_value = the_queue.front();
        the_queue.pop();
		ReleaseMutex(the_mutex);
        return true;
    }

private :
	concurrent_queue(concurrent_queue& other);
	concurrent_queue & operator= (concurrent_queue & other);
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
		m_CommandBuffer.push(command);
	}
	bool GetCommand()
	{
		if (m_CommandBuffer.try_pop(m_CurrCommand))
		{
			return true;
		}
		return false;
	}
	WORD GetCommandHeader() {return m_CurrCommand.header;}
	Param_t GetCommandWParam(){return m_CurrCommand.wParam;}
	Param_t GetCommandLParam(){return m_CurrCommand.lParam;}

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
	}

	void ShowPipeName(){MessageBox(0, m_cPipeName, "PIPENAME", MB_OK); }

private:
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

	concurrent_queue<BaseMessage> m_CommandBuffer;
};

extern GWCAServer* myGWCAServer;

#endif