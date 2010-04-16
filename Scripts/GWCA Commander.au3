;Msg stuff!
#include <GUIConstantsEx.au3>
#include <EditConstants.au3>
#include <ComboConstants.au3>
#include <StaticConstants.au3>
#include "GWCAConstants.au3"

$myGUI = GUICreate("GWCA Commander", 300, 100)
$inputCmd = GUICtrlCreateCombo("", 5, 5, 290, 20, BitOr($GUI_SS_DEFAULT_COMBO, $CBS_SORT, $CBS_DROPDOWNLIST))
GUICtrlSetFont(-1, 7.5)
GUICtrlSetData(-1,  "$CA_GetCurrentTarget|$CA_GetMyId|$CA_Casting|$CA_SkillRecharge|$CA_SkillAdrenaline|"& _
					"$CA_SetLogAndHwnd|$CA_GetAgentAndTargetPtr|$CA_GetSkillbarSkillId|$CA_GetMyMaxHP|$CA_GetMyMaxEnergy|$CA_GetBuildNumber|"& _
					"$CA_ChangeMaxZoom|$CA_GetLastDialogId|$CA_SetEngineHook|"& _
					"$CA_Attack|$CA_Move|$CA_UseSkill|"& _
					"$CA_ChangeWeaponSet|$CA_ZoneMap|$CA_DropGold|$CA_GoNpc|$CA_GoPlayer|$CA_GoSignpost|"& _
					"$CA_EnterChallenge|$CA_OpenChest|$CA_SetEventSkillMode|$CA_PickupItem|"& _
					"$CA_Dialog|$CA_ChangeTarget|$CA_TargetNearestFoe|$CA_TargetNearestAlly|"& _
					"$CA_TargetNearestItem|$CA_TargetCalledTarget|$CA_UseHero1Skill|$CA_UseHero2Skill|$CA_UseHero3Skill|"& _
					"$CA_CancelAction|$CA_GetName|$CA_CommandHero1|$CA_CommandHero2|$CA_CommandHero3|$CA_CommandAll|$CA_ChangeDistrict|"& _
					"$CA_Resign|$CA_ReturnToOutpost|"& _
					"$CA_GetMapLoading|$CA_GetMapId|$CA_GetPing|$CA_GetLoggedIn|$CA_GetDead|$CA_GetBalthFaction|$CA_GetKurzFaction|$CA_GetLuxonFaction|"& _
					"$CA_GetTitleTreasure|$CA_GetTitleLucky|$CA_GetTitleUnlucky|$CA_GetTitleWisdom|"& _
					"$CA_GetAgentExist|$CA_GetProfessions|$CA_GetPlayerNumber|"& _
					"$CA_GetHP|$CA_GetRotation|"& _
					"$CA_GetSkill|$CA_GetCoords|$CA_GetWeaponSpeeds|$CA_GetSpiritRange|$CA_GetTeamId|$CA_GetCombatMode|"& _
					"$CA_GetModelMode|$CA_GetHpPips|$CA_GetEffects|$CA_GetHex|$CA_GetModelAnimation|$CA_GetEnergy|$CA_GetAgentPtr|"& _
					"$CA_GetType|$CA_GetLevel|$CA_GetNameProperties|$CA_GetMaxId|$CA_GetMyNearestAgent|"& _
					"$CA_GetMyDistanceToAgent|$CA_GetNearestAgentToAgent|$CA_GetDistanceFromAgentToAgent|$CA_GetNearestAgentToAgentEx|"& _
					"$CA_GetModelState|$CA_GetIsAttacking|$CA_GetIsKnockedDown|$CA_GetIsMoving|$CA_GetIsDead|$CA_GetIsCasting|"& _
					"$CA_GetFirstAgentByPlayerNumber|$CA_GetAllegiance|$CA_GetNearestEnemyToAgentEx|$CA_GetIsAttackedMelee|$CA_GetNearestItemToAgentEx|"& _
					"$CA_GetNearestAgentByPlayerNumber|$CA_GetSpeed|$CA_GetNearestEnemyToAgentByAllegiance|$CA_GetNearestAliveEnemyToAgent|"& _
					"$CA_GetWeaponType|$CA_GetNearestSignpostToAgent|$CA_GetNearestNpcToAgentByAllegiance|$CA_GetNearestAgentToCoords|$CA_GetVars|"& _
					"$CA_GetNearestNpcToCoords|$CA_GetLoginNumber|"& _
					"$CA_GetGold|$CA_GetBagSize|$CA_SetBag|$CA_GetItemId|"& _
					"$CA_GetIdKit|$CA_IdentifyItem|$CA_IdentifyItemById|$CA_DepositGold|$CA_WithdrawGold|$CA_SellItem|$CA_SellItemById|"& _
					"$CA_BuyIdKit|$CA_BuySuperiorIdKit|$CA_PrepareMoveItem|$CA_MoveItem|$CA_GetItemInfo|"& _
					"$CA_UseItem|$CA_UseItemById|$CA_DropItem|$CA_DropItemById|"& _
					"$CA_AcceptAllItems|$CA_GetItemLastModifier|"& _
					"$CA_GetTitleSunspear|$CA_GetTitleLightbringer|$CA_GetTitleVanguard|$CA_GetTitleNorn|$CA_GetTitleAsura|$CA_GetTitleDeldrimor|"& _
					"$CA_GetTitleNorthMastery|$CA_GetTitleDrunkard|$CA_GetTitleSweet|$CA_GetTitleParty|$CA_GetTitleCommander|$CA_GetTitleLuxon|$CA_GetTitleKurzick|"& _
					"$CA_AddHero|$CA_KickHero|$CA_SwitchMode|$CA_AddNpc|$CA_KickNpc|$CA_TravelGH|$CA_LeaveGH|$CA_GoAgent|$CA_DonateFaction|$CA_SetSkillbarSkill|"& _
					"$CA_FindItemByModelId|$CA_ChangeSecondProfession|$CA_GetNumberOfAgentsByPlayerNumber|$CA_FindEmptySlot|$CA_FindGoldItem|"& _
					"$CA_GetNumberOfAliveEnemyAgents|$CA_GetItemPositionByItemId|$CA_GetItemPositionByModelId|$CA_GetItemPositionByRarity|"& _
					"$CA_GetItemModelIdById|$CA_GetItemInfoById|$CA_GetItemLastModifierById|$CA_InitMapLoad|$CA_MapIsLoaded|$CA_GetNextItem|$CA_TargetNextPartyMember|"& _
					"$CA_TargetNextFoe|$CA_EquipItem|$CA_EquipItemById|$CA_GetTarget|$CA_GetMapOverlayCoords|$CA_GetMapOverlayInfo|$CA_GetNearestMapOverlayToCoords|"& _
					"$CA_SkipCinematic|$CA_GetPartyInfo|$CA_ClearPacketQueue|$CA_SetHeroMode|"& _
					"$CA_QuestCheck|$CA_QuestCoords|$CA_QuestActive|$CA_QuestAbandon|$CA_SalvageItem|$CA_GetSalvageKit|$CA_SetAttribute|$CA_SetTeamSize|$CA_BuyItem|"& _
					"$CA_DismissBuff|$CA_PlayerHasBuff|$CA_Hero1HasBuff|$CA_Hero2HasBuff|$CA_Hero3HasBuff|$CA_SendChat|$CA_AllocMem|$CA_FreeMem|"& _
					"$CA_GetTitleGamer|$CA_GetExperience|$CA_GetItemIdByAgent|$CA_GetItemInfoByAgent|$CA_GetItemLastModifierByAgent|$CA_GetNearestItemByModelId|")
GUICtrlSetData(-1,	"$CA_GetRegionAndLanguage|$CA_TraderRequest|$CA_TraderCheck|$CA_TraderBuy|$CA_TraderRequestSell|$CA_TraderRequestSellById|$CA_TraderSell|"& _
					"$CA_GetItemExtraId|$CA_GetItemExtraIdById|$CA_GetConnection|$CA_OpenStorage|$CA_GetItemExtraIdByAgent|$CA_GetItemReq|$CA_GetItemReqById|"& _
					"$CA_GetItemReqByAgent|$CA_GetDyePositionByColor|$CA_GetNumberOfFoesInRangeOfAgent|$CA_GetNumberOfAlliesInRangeOfAgent|"& _
					"$CA_GetNumberOfItemsInRangeOfAgent|$CA_UpdateAgentPosition|$CA_MoveOld|$CA_GetAgentMovementPtr|$CA_GetMapBoundariesPtr|"& _
					"$CA_TradePlayer|$CA_SubmitOffer|$CA_ChangeOffer|$CA_OfferItem|$CA_CancelTrade|$CA_AcceptTrade|$CA_GetAgentWeapons|$CA_GetMatchStatus|" & _
					"$CA_GetEffectCount|$CA_GetEffect|$CA_GetEffectByIndex|$CA_GetEffectDuration|$CA_GetTimeStamp|$CA_GetAgentDanger|$CA_GetTypeMap|" & _
					"$CA_GetNextAgent|$CA_GetNextAlly|$CA_GetNextFoe|$CA_ResetAttributes|$CA_GetItemDmgMod|$CA_GetItemDmgModById|$CA_GetItemDmgModByAgent|" & _
					"$CA_SetEquipmentAgent|$CA_SetEquipmentModelId|$CA_SetEquipmentDye|$CA_GetEquipmentModelId|$CA_SetEquipmentShinyness|$CA_GetEquipmentDyeInfo")
$inputWparam = GUICtrlCreateInput("", 5, 28, 150, 20)
$inputLparam = GUICtrlCreateInput("", 5, 51, 150, 20)
$btnSend = GUICtrlCreateButton("Send MSG", 4, 73)
GUICtrlSetFont(-1, 7.5)
$btnSendReceive = GUICtrlCreateButton("Send && Receive", 65, 73)
GUICtrlSetFont(-1, 7.5)
$inputType = GUICtrlCreateCombo("", 160, 75, 135, 20, BitOr($GUI_SS_DEFAULT_COMBO, $CBS_SORT, $CBS_DROPDOWNLIST))
GUICtrlSetFont(-1, 7.5)
GUICtrlSetData(-1, "hex|float|int", "hex")
$labelWparam = GUICtrlCreateLabel("", 160, 30, 135, 20)
GUICtrlSetColor(-1, 0xFFFFFF)
$labelLparam = GUICtrlCreateLabel("", 160, 53, 135, 20)
GUICtrlSetColor(-1, 0xFFFFFF)

GUISetBkColor(0x000000, $myGUI)

GUISetState(True, $myGUI)

While 1
	Switch(GUIGetMsg())
		Case $GUI_EVENT_CLOSE
			Exit
		Case $btnSend
			$sendCmd = GUICtrlRead($inputCmd)
			If StringLeft($sendCmd, 2) = "0x" Then
				$sendCmd = BinaryToString($sendCmd)
			ElseIf StringLeft($sendCmd,1) = "$" Then
				$sendCmd = Eval(StringMid($sendCmd, 2))
			EndIf

			$sendW = Number(GUICtrlRead($inputWparam))

			$sendL = Number(GUICtrlRead($inputLparam))

			If $sendCmd = $CA_MOVE OR $sendCmd = $CA_GETNEARESTAGENTTOCOORDS OR $sendCmd = $CA_GETNEARESTNPCTOCOORDS OR _
				$sendCmd = $CA_COMMANDHERO1 OR $sendCmd = $CA_COMMANDHERO2 OR $sendCmd = $CA_COMMANDHERO3 OR _
				$sendCmd = $CA_COMMANDALL OR $sendCmd = $CA_MoveOld Then
				$sendW = _FloatToInt($sendW)
				$sendL = _FloatToInt($sendL)
			EndIf

			Cmd($sendCmd, $sendW, $sendL)
			_GWCA_CloseStream()

		Case $btnSendReceive
			$sendCmd = GUICtrlRead($inputCmd)
			If StringLeft($sendCmd, 2) = "0x" Then
				$sendCmd = BinaryToString($sendCmd)
			ElseIf StringLeft($sendCmd,1) = "$" Then
				$sendCmd = Eval(StringMid($sendCmd, 2))
			EndIf

			$sendW = Number(GUICtrlRead($inputWparam))

			$sendL = Number(GUICtrlRead($inputLparam))

			GUICtrlSetData($labelWparam, "")
			GUICtrlSetData($labelLparam, "")

			If $sendCmd = $CA_GETNEARESTNPCTOCOORDS OR $sendCmd = $CA_GETNEARESTAGENTTOCOORDS OR $sendCmd = $CA_GETNEARESTMAPOVERLAYTOCOORDS Then
				$sendW = _FloatToInt($sendW)
				$sendL = _FloatToInt($sendL)
			EndIf

			$cbType = GUICtrlRead($inputType)
			CmdCB($sendCmd, $sendW, $sendL)
			_GWCA_CloseStream()
			ConsoleWrite($cbVar[0]&" - "&$cbVar[1]&@CRLF)
			GUICtrlSetData($labelWparam, $cbVar[0])
			GUICtrlSetData($labelLparam, $cbVar[1])
	EndSwitch
WEnd
