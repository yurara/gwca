; # -+-+-+-+-+-+-+ #
;   GWCA Constants
;	  By Sune C
; # +-+-+-+-+-+-+- #

#include-once

#include <NamedPipes.au3>
#include <WinAPI.au3>

; The SkillLog structure for use with the Skill Log feature
Global $tagSKILLLOGSTRUCT = "long AgentId;long MyId;long SkillId;float Activation;byte TeamId;ushort Allegiance;float Distance;long Ping;long TargetId"
Global $tagPARTYINFO = "hwnd Receiver;long TeamSize;long TeamId;" & _
	"long Agent1Id;float Agent1X;float Agent1Y;float Agent1HP;long Agent1Effects;byte Agent1Hex;dword Agent1Name;byte Agent1Primary;byte Agent1Secondary;long Agent1Target;word Agent1Skill;word Agent1Weapon;" & _
	"long Agent2Id;float Agent2X;float Agent2Y;float Agent2HP;long Agent2Effects;byte Agent2Hex;dword Agent2Name;byte Agent2Primary;byte Agent2Secondary;long Agent2Target;word Agent2Skill;word Agent2Weapon;" & _
	"long Agent3Id;float Agent3X;float Agent3Y;float Agent3HP;long Agent3Effects;byte Agent3Hex;dword Agent3Name;byte Agent3Primary;byte Agent3Secondary;long Agent3Target;word Agent3Skill;word Agent3Weapon;" & _
	"long Agent4Id;float Agent4X;float Agent4Y;float Agent4HP;long Agent4Effects;byte Agent4Hex;dword Agent4Name;byte Agent4Primary;byte Agent4Secondary;long Agent4Target;word Agent4Skill;word Agent4Weapon;" & _
	"long Agent5Id;float Agent5X;float Agent5Y;float Agent5HP;long Agent5Effects;byte Agent5Hex;dword Agent5Name;byte Agent5Primary;byte Agent5Secondary;long Agent5Target;word Agent5Skill;word Agent5Weapon;" & _
	"long Agent6Id;float Agent6X;float Agent6Y;float Agent6HP;long Agent6Effects;byte Agent6Hex;dword Agent6Name;byte Agent6Primary;byte Agent6Secondary;long Agent6Target;word Agent6Skill;word Agent6Weapon;" & _
	"long Agent7Id;float Agent7X;float Agent7Y;float Agent7HP;long Agent7Effects;byte Agent7Hex;dword Agent7Name;byte Agent7Primary;byte Agent7Secondary;long Agent7Target;word Agent7Skill;word Agent7Weapon;" & _
	"long Agent8Id;float Agent8X;float Agent8Y;float Agent8HP;long Agent8Effects;byte Agent8Hex;dword Agent8Name;byte Agent8Primary;byte Agent8Secondary;long Agent8Target;word Agent8Skill;word Agent8Weapon;"

; The constants
Global Enum $CA_DisconnectPipe = 0x001, $CA_AliveRequest, $CA_IsAlive, _
	$CA_Error = 0x050, _
	$CA_CommandsBegin = 0x101, _
	$CA_Attack, $CA_Dialog, $CA_CancelAction, $CA_ClearPacketQueue, $CA_QuestAbandon, $CA_DonateFaction, $CA_SetLogAndHwnd,  _
	$CA_Move, $CA_GoNpc, $CA_GoPlayer, $CA_GoSignpost, $CA_GoAgent,  _
	$CA_UseSkill, $CA_SetEventSkillMode, $CA_SetSkillbarSkill, $CA_SetAttribute, $CA_ChangeSecondProfession,  _
	$CA_ChangeWeaponSet, $CA_EquipItem, $CA_EquipItemById, $CA_UseItem, $CA_UseItemById,  _
	$CA_DropGold, $CA_DepositGold, $CA_WithdrawGold, _
	$CA_ChangeTarget, $CA_TargetNearestFoe, $CA_TargetNearestAlly, $CA_TargetNextPartyMember, $CA_TargetNextFoe,  _
	$CA_TargetNearestItem, $CA_TargetCalledTarget, _
	$CA_UseHero1Skill, $CA_UseHero2Skill, $CA_UseHero3Skill, _
	$CA_CommandHero1, $CA_CommandHero2, $CA_CommandHero3, $CA_CommandAll, $CA_SetHeroMode,  _
	$CA_AddHero, $CA_KickHero, $CA_AddNpc, $CA_KickNpc,  _
	$CA_ChangeDistrict, $CA_ZoneMap, $CA_SwitchMode, $CA_InitMapLoad, $CA_SkipCinematic,  _
	$CA_DismissBuff, $CA_SendChat, $CA_SetTeamSize, $CA_FreeMem,  _
	$CA_Resign, $CA_ReturnToOutpost, $CA_EnterChallenge, $CA_TravelGH, $CA_LeaveGH,  _
	$CA_SetBag, $CA_PrepareMoveItem, $CA_MoveItem,  _
	$CA_IdentifyItem, $CA_IdentifyItemById, $CA_SalvageItem,  _
	$CA_SellItem, $CA_SellItemById, $CA_BuyIdKit, $CA_BuySuperiorIdKit, _
	$CA_BuyItem, $CA_TraderRequest, $CA_TraderRequestSell, $CA_TraderRequestSellById, $CA_TraderBuy, $CA_TraderSell, _
	$CA_OpenChest, $CA_AcceptAllItems, $CA_PickupItem, $CA_DropItem, $CA_DropItemById, $CA_OpenStorage, _
	$CA_UpdateAgentPosition, $CA_MoveOld, _
	$CA_CommandsEnd
Global Enum $CA_RequestsBegin = 0x301,  _
	$CA_GetCurrentTarget,  _
	$CA_GetMyId, $CA_GetMyMaxHP, $CA_GetMyMaxEnergy, $CA_GetMyNearestAgent, $CA_GetMyDistanceToAgent,  _
	$CA_Casting, $CA_SkillRecharge, $CA_SkillAdrenaline, $CA_GetSkillbarSkillId,  _
	$CA_GetTarget, $CA_GetAgentAndTargetPtr,  _
	$CA_GetBuildNumber, $CA_ChangeMaxZoom, $CA_GetLastDialogId, $CA_SetEngineHook,  _
	$CA_GetGold, $CA_GetBagSize, $CA_GetItemId,  _
	$CA_GetIdKit, $CA_GetSalvageKit,  _
	$CA_GetItemInfo, $CA_GetItemLastModifier, $CA_GetItemLastModifierById,  _
	$CA_FindItemByModelId, $CA_FindEmptySlot, $CA_FindGoldItem,  _
	$CA_GetItemPositionByItemId, $CA_GetItemPositionByModelId, $CA_GetItemPositionByRarity,  _
	$CA_GetItemModelIdById, $CA_GetItemInfoById,  _
	$CA_GetItemIdByAgent, $CA_GetItemInfoByAgent, $CA_GetItemLastModifierByAgent, $CA_GetNearestItemByModelId, _
	$CA_GetMapLoading, $CA_GetMapId, $CA_MapIsLoaded, $CA_GetRegionAndLanguage, $CA_GetPing, $CA_GetLoggedIn, $CA_GetDead,  _
	$CA_GetBalthFaction, $CA_GetKurzFaction, $CA_GetLuxonFaction,  _
	$CA_GetTitleTreasure, $CA_GetTitleLucky, $CA_GetTitleUnlucky, $CA_GetTitleWisdom, $CA_GetTitleGamer, $CA_GetExperience, _
	$CA_GetTitleSunspear, $CA_GetTitleLightbringer, $CA_GetTitleVanguard, $CA_GetTitleNorn, $CA_GetTitleAsura, $CA_GetTitleDeldrimor,  _
	$CA_GetTitleNorthMastery, $CA_GetTitleDrunkard, $CA_GetTitleSweet, $CA_GetTitleParty, $CA_GetTitleCommander, $CA_GetTitleLuxon, $CA_GetTitleKurzick,  _
	$CA_GetMapOverlayCoords, $CA_GetMapOverlayInfo, $CA_GetNearestMapOverlayToCoords, $CA_GetPartyInfo,  _
	$CA_GetAgentExist, $CA_GetProfessions, $CA_GetPlayerNumber, $CA_GetName, $CA_GetHP, $CA_GetRotation,  _
	$CA_GetSkill, $CA_GetCoords, $CA_GetWeaponSpeeds, $CA_GetSpiritRange, $CA_GetTeamId, $CA_GetCombatMode,  _
	$CA_GetModelMode, $CA_GetHpPips, $CA_GetEffects, $CA_GetHex, $CA_GetModelAnimation, $CA_GetEnergy, $CA_GetAgentPtr,  _
	$CA_GetType, $CA_GetLevel, $CA_GetNameProperties, $CA_GetMaxId, $CA_GetSpeed, $CA_GetAllegiance, $CA_GetWeaponType,  _
	$CA_GetModelState, $CA_GetIsAttacking, $CA_GetIsKnockedDown, $CA_GetIsMoving, $CA_GetIsDead, $CA_GetIsCasting, $CA_GetIsAttackedMelee, _
	$CA_PlayerHasBuff, $CA_Hero1HasBuff, $CA_Hero2HasBuff, $CA_Hero3HasBuff,  _
	$CA_GetFirstAgentByPlayerNumber, $CA_GetNearestAgentToAgent, $CA_GetDistanceFromAgentToAgent,  _
	$CA_GetNearestAgentToAgentEx, $CA_GetNearestEnemyToAgentEx, $CA_GetNearestItemToAgentEx,  _
	$CA_GetNearestAgentByPlayerNumber, $CA_GetNearestEnemyToAgentByAllegiance, $CA_GetNearestAliveEnemyToAgent,  _
	$CA_GetNearestSignpostToAgent, $CA_GetNearestNpcToAgentByAllegiance, $CA_GetNearestAgentToCoords, _
	$CA_GetNearestNpcToCoords, $CA_GetLoginNumber, $CA_GetNumberOfAgentsByPlayerNumber, $CA_GetNumberOfAliveEnemyAgents, $CA_GetNextItem,  _
	$CA_QuestCheck, $CA_QuestCoords, $CA_QuestActive, $CA_AllocMem, $CA_TraderCheck, _
	$CA_GetItemExtraId, $CA_GetItemExtraIdById, $CA_GetConnection, _
	$CA_GetItemExtraIdByAgent, $CA_GetItemReq, $CA_GetItemReqById, $CA_GetItemReqByAgent, $CA_GetDyePositionByColor, _
	$CA_GetNumberOfFoesInRangeOfAgent, $CA_GetNumberOfAlliesInRangeOfAgent, $CA_GetNumberOfItemsInRangeOfAgent, _
	$CA_GetAgentMovementPtr, _
	$CA_RequestsEnd


Global Enum $RARITY_WHITE = 0x3D, $RARITY_BLUE = 0x3F, $RARITY_PURPLE = 0x42, $RARITY_GOLD = 0x40, $RARITY_GREEN = 0x43

Global Enum $BAG_Backpack = 1, $BAG_BeltPouch, $BAG_Bag1, $BAG_Bag2, $BAG_EquipmentPack, $BAG_UnclaimedItems = 7, $BAG_Storage1, $BAG_Storage2, _
			$BAG_Storage3, $BAG_Storage4, $BAG_Storage5, $BAG_Storage6, $BAG_Storage7, $BAG_Storage8, $BAG_StorageAnniversary

Global Enum $HERO_Norgu = 1, $HERO_Goren, $HERO_Tahklora, $HERO_MasterOfWhispers, $HERO_AcolyteJin, $HERO_Koss, $HERO_Dunkoro, $HERO_AcolyteSousuke, $HERO_Melonni, _
			$HERO_ZhedShadowhoof, $HERO_GeneralMorgahn, $HERO_MargridTheSly, $HERO_Olias = 14, $HERO_Razah, $HERO_MOX, $HERO_Jora = 18, $HERO_PyreFierceshot, _
			$HERO_Livia = 21, $HERO_Kahmu, $HERO_Gwen, $HERO_Xandra, $HERO_Vekk, $HERO_Ogden

Global Enum $HEROMODE_Fight, $HEROMODE_Guard, $HEROMODE_Avoid

Global Enum $DYE_BLUE = 2, $DYE_GREEN, $DYE_PURPLE, $DYE_RED, $DYE_YELLOW, $DYE_BROWN, $DYE_ORANGE, $DYE_SILVER, $DYE_BLACK, $DYE_GRAY, $DYE_WHITE

Global Enum $IS_NUMERIC = 0x00, _
			$IS_TEXT = 0x01, _
			$IS_COMMAND = 0x02, _
			$IS_REQUEST = 0x04, _
			$IS_RESPONSE = 0x08

Global Const $FLAG_RESET = 0x7F800000

; VARIABLES

Opt("WinTitleMatchMode", -1) ;Make sure that Guild Wars receives the messages (1 = match start, 2 = substring, 3 = exact)

Global $cbType = "int" ;What to read the callback values as
Global $cbVar[2] ;Array for callback wParam and lParam
Global $sGW = "Guild Wars -" ;Name of window
Global $hGWCA_STREAM = 0 ;Handle to the stream
Global $bGWCA_INTERNAL = False ;Is set to True when inside a Cmd() or CmdCB() call

; FUNCTIONS

Func _IntToFloat($iInt)
	Local $tFloat, $tInt

	$tInt = DllStructCreate("int")
	$tFloat = DllStructCreate("float", DllStructGetPtr($tInt))
	DllStructSetData($tInt, 1, $iInt)
	Return DllStructGetData($tFloat, 1)
EndFunc

Func _FloatToInt($fFloat)
	Local $tFloat, $tInt

	$tFloat = DllStructCreate("float")
	$tInt = DllStructCreate("int", DllStructGetPtr($tFloat))
	DllStructSetData($tFloat, 1, $fFloat)
	Return DllStructGetData($tInt, 1)
EndFunc

Func Cmd($uMsg, $wparam = 0, $lparam = 0)
	Local $hPipe
	Local $iRead = 0
	Local $OutBuffer = DllStructCreate("WORD header;WORD type;BYTE wparam[4];BYTE lparam[4]") ;Pipes sending buffer
	Local $InBuffer = DllStructCreate("WORD header;WORD type;BYTE wparam[4];BYTE lparam[4]") ;Pipes incoming buffer
	$bGWCA_INTERNAL = True
	DllStructSetData($OutBuffer,"header",$uMsg)
	DllStructSetData($OutBuffer,"type",BitOR($IS_COMMAND, $IS_NUMERIC))
	DllStructSetData($OutBuffer,"wparam",$wparam) ; set wparam
	DllStructSetData($OutBuffer,"lparam",$lparam) ; set lparam

	If Not $hGWCA_STREAM Then
		If Not _NamedPipes_WaitNamedPipe("\\.\pipe\GWCA_"&WinGetProcess($sGW), 1000) Then
			Return
		EndIf
		$hGWCA_STREAM = _WinAPI_CreateFile("\\.\pipe\GWCA_"&WinGetProcess($sGW), 2, 6)
	EndIf

	_WinAPI_WriteFile($hGWCA_STREAM,DllStructGetPtr($OutBuffer),12,$iRead)

	$bGWCA_INTERNAL = False
EndFunc

Func CmdCB($uMsg, $wparam = 0, $lparam = 0)
	Local $hPipe
	Local $sPipe = "\\.\pipe\GWCA_"&WinGetProcess($sGW)
	Local $iRead = 0
	Local $OutBuffer = DllStructCreate("WORD header;WORD type;BYTE wparam[4];BYTE lparam[4]") ;Pipes sending buffer
	Local $InBuffer = DllStructCreate("WORD header;WORD type;BYTE wparam[4];BYTE lparam[4]") ;Pipes incoming buffer
	$bGWCA_INTERNAL = True
	$cbVar[0] = ""
	$cbVar[1] = ""
	DllStructSetData($OutBuffer,"header",$uMsg)
	DllStructSetData($OutBuffer,"type",BitOR($IS_REQUEST, $IS_NUMERIC))
	DllStructSetData($OutBuffer,"wparam",$wparam) ; set wparam
	DllStructSetData($OutBuffer,"lparam",$lparam) ; set lparam

	If Not $hGWCA_STREAM Then
		If Not _NamedPipes_WaitNamedPipe("\\.\pipe\GWCA_"&WinGetProcess($sGW), 1000) Then
			Return
		EndIf
		$hGWCA_STREAM = _WinAPI_CreateFile("\\.\pipe\GWCA_"&WinGetProcess($sGW), 2, 6)
	EndIf

	_WinAPI_WriteFile($hGWCA_STREAM,DllStructGetPtr($OutBuffer),12,$iRead)
	_WinAPI_ReadFile($hGWCA_STREAM,DllStructGetPtr($InBuffer),12,$iRead)

	Switch $cbType
		Case "float"
			$cbVar[0] = DllStructGetData(DllStructCreate("float", DllStructGetPtr($InBuffer, "wparam")), 1)
			$cbVar[1] = DllStructGetData(DllStructCreate("float", DllStructGetPtr($InBuffer, "lparam")), 1)
		Case "int"
			$cbVar[0] = DllStructGetData(DllStructCreate("int", DllStructGetPtr($InBuffer, "wparam")), 1)
			$cbVar[1] = DllStructGetData(DllStructCreate("int", DllStructGetPtr($InBuffer, "lparam")), 1)
		Case "hex"
			$cbVar[0] = "0x"&Hex(DllStructGetData(DllStructCreate("int", DllStructGetPtr($InBuffer, "wparam")), 1))
			$cbVar[1] = "0x"&Hex(DllStructGetData(DllStructCreate("int", DllStructGetPtr($InBuffer, "lparam")), 1))
		Case Else
			$cbVar[0] = DllStructGetData(DllStructCreate("int", DllStructGetPtr($InBuffer, "wparam")), 1)
			$cbVar[1] = DllStructGetData(DllStructCreate("int", DllStructGetPtr($InBuffer, "lparam")), 1)
	EndSwitch


	$bGWCA_INTERNAL = False

	Return $cbVar
EndFunc

Func _GWCA_CloseStream()
	_WinAPI_CloseHandle($hGWCA_STREAM)
	$hGWCA_STREAM = 0
EndFunc

Func MoveEx($x, $y, $random = 50)
	Cmd($CA_MOVE, _FloatToInt($x+Random(-$random,$random)), _FloatToInt($y+Random(-$random,$random)))
EndFunc

Func GetNearestAgentToCoords($x, $y)
	$oldCbType = $cbType

	$cbType = "int"
	CmdCB($CA_GetNearestAgentToCoords, _FloatToInt($x), _FloatToInt($y))

	$cbType = $oldCbType

	Return $cbVar[0]
EndFunc

Func GetNearestNPCToCoords($x, $y)
	$oldCbType = $cbType

	$cbType = "int"
	CmdCB($CA_GetNearestNpcToCoords, _FloatToInt($x), _FloatToInt($y))

	$cbType = $oldCbType

	Return $cbVar[0]
EndFunc

Func GetNearestMapOverlayToCoords($x, $y)
	$oldCbType = $cbType

	$cbType = "int"
	CmdCB($CA_GetNearestMapOverlayToCoords, _FloatToInt($x), _FloatToInt($y))

	$cbType = $oldCbType

	Return $cbVar[0]
EndFunc

Func TravelTo($iMapId, $iDis = 0)
	$tMap = TimerInit()
	Cmd($CA_InitMapLoad)
	Cmd($CA_ZoneMap, $iMapId, $iDis)
	Do
		Sleep(200)
		$aLoad = CmdCB($CA_GetMapLoading)
		If TimerDiff($tMap) > 15000 AND $aLoad[0] <> 2 Then Cmd($CA_ZoneMap, $iMapId, $iDis)
		$aLoaded = CmdCB($CA_MapIsLoaded)
	Until $aLoad[0] = 0 AND $aLoaded[0] = 1
EndFunc

Func MoveItem($itemBag, $itemSlot, $newBag, $newSlot)
	Cmd($CA_PREPAREMOVEITEM, $itemBag, $itemSlot)
	Sleep(20)
	Cmd($CA_MOVEITEM, $newBag, $newSlot)
EndFunc

Func MoveItemById($itemId, $newBag, $newSlot)
	Cmd($CA_PREPAREMOVEITEM, $itemId)
	Sleep(20)
	Cmd($CA_MOVEITEM, $newBag, $newSlot)
EndFunc

Func PickupItems($iItems = -1, $fMaxDistance = 1012)
	Local $iItemsPicked = 0

	$oldCbType = $cbType

	$cbType = "int"

	$tDeadlock = TimerInit()
	Do
		$aItem = CmdCB($CA_GETNEARESTITEMTOAGENTEX, -2)
		If $aItem[0] = 0 OR _IntToFloat($aItem[1]) > $fMaxDistance OR TimerDiff($tDeadlock) > 30000 Then ExitLoop

		Cmd($CA_PICKUPITEM, $aItem[0])
		$tDeadlock2 = TimerInit()
		Do
			Sleep(500)
			CmdCB($CA_GETAGENTEXIST, $aItem[0])
			If TimerDiff($tDeadlock2) > 5000 Then ContinueLoop 2
		Until $cbVar[0] = 0

		$iItemsPicked += 1
	Until $iItemsPicked = $iItems

	$cbType = $oldCbType
EndFunc

Func GetItemId($iBag, $iSlot)
	$oldCbType = $cbType

	$cbType = "int"

	CmdCB($CA_SetBag, $iBag)
	CmdCB($CA_GetItemId, $iSlot)

	$cbType = $oldCbType

	Return $cbVar
EndFunc

Func GetItemInfo($iBag, $iSlot)
	$oldCbType = $cbType

	$cbType = "int"

	CmdCB($CA_SetBag, $iBag)
	CmdCB($CA_GetItemInfo, $iSlot)

	$cbType = $oldCbType

	Return $cbVar
EndFunc

Func GetItemLastModifier($iBag, $iSlot)
	$oldCbType = $cbType

	$cbType = "int"

	CmdCB($CA_SetBag, $iBag)
	CmdCB($CA_GetItemLastModifier, $iSlot)

	$cbType = $oldCbType

	Return $cbVar
EndFunc

Func MoveToEx($x, $y, $random = 50)
	Local $iBlocked = 0

	$cbType = "int"
	$mState = CmdCB($CA_GetMapLoading)

	$cbType = "float"
	MoveEx($x, $y, $random)

	CmdCB($CA_GETCOORDS, -2)
	Do
		Sleep(250)
		$cbType = "int"
		CmdCB($CA_GETDEAD)
		If $cbVar[0] = 1 Then Return

		$mStateOld = $mState
		$mState = CmdCB($CA_GetMapLoading)
		If $mState[0] <> $mStateOld[0] Then Return

		CmdCB($CA_GetIsMoving, -2)
		If $cbVar[0] = 0 Then
			$iBlocked += 1
			Cmd($CA_UpdateClientPosition)
			MoveEx($x, $y, $random)
		EndIf

		$cbType = "float"
		CmdCB($CA_GETCOORDS, -2)
	Until ComputeDistanceEx($cbVar[0], $cbVar[1], $x, $y) < 250 OR $iBlocked > 10
EndFunc

Func ComputeDistanceEx($x1, $y1, $x2, $y2)
	Return Sqrt(($y2 - $y1)^2 + ($x2 - $x1)^2)
EndFunc

Func UseSkillEx($iSkillSlot, $iTarget = 0)
	$oldCbType = $cbType

	$tDeadlock = TimerInit()
	$cbType = "int"
	Cmd($CA_USESKILL, $iSkillSlot, $iTarget)
	Do
		Sleep(250)
		CmdCB($CA_GETDEAD)
		If $cbVar[0] = 1 Then ExitLoop
		CmdCB($CA_GETSKILL, -2)
		If $cbVar[0] = 0 AND TimerDiff($tDeadlock) > 1000 Then ExitLoop
		CmdCB($CA_SKILLRECHARGE, $iSkillSlot)
	Until $cbVar[0] <> 0 OR TimerDiff($tDeadlock) > 15000

	$cbType = $oldCbType
EndFunc

Func _GWCAMemOpen($iv_Pid, $iv_DesiredAccess = 0x1F0FFF, $iv_InheritHandle = 1)
	If Not ProcessExists($iv_Pid) Then
		SetError(1)
        Return 0
	EndIf

	Local $ah_Handle[2] = [DllOpen('kernel32.dll')]

	If @Error Then
        SetError(2)
        Return 0
    EndIf

	Local $av_OpenProcess = DllCall($ah_Handle[0], 'int', 'OpenProcess', 'int', $iv_DesiredAccess, 'int', $iv_InheritHandle, 'int', $iv_Pid)

	If @Error Then
        DllClose($ah_Handle[0])
        SetError(3)
        Return 0
    EndIf

	$ah_Handle[1] = $av_OpenProcess[0]
	Return $ah_Handle
EndFunc

Func _GWCAMemClose($ah_Handle)
	If Not IsArray($ah_Handle) Then
		SetError(1)
        Return 0
	EndIf

	DllCall($ah_Handle[0], 'int', 'CloseHandle', 'int', $ah_Handle[1])
	If Not @Error Then
		DllClose($ah_Handle[0])
		Return 1
	Else
		DllClose($ah_Handle[0])
		SetError(2)
        Return 0
	EndIf
EndFunc


Func _GWCAMemWrite($iv_Address, $ah_Handle, $v_Data, $sv_Type = 'dword')
	If Not IsArray($ah_Handle) Then
		SetError(1)
        Return 0
	EndIf

	Local $v_Buffer = DllStructCreate($sv_Type)

	If @Error Then
		SetError(@Error + 1)
		Return 0
	Else
		DllStructSetData($v_Buffer, 1, $v_Data)
		If @Error Then
			SetError(6)
			Return 0
		EndIf
	EndIf

	DllCall($ah_Handle[0], 'int', 'WriteProcessMemory', 'int', $ah_Handle[1], 'int', $iv_Address, 'ptr', DllStructGetPtr($v_Buffer), 'int', DllStructGetSize($v_Buffer), 'int', '')

	If Not @Error Then
		Return 1
	Else
		SetError(7)
        Return 0
	EndIf
EndFunc

Func SendChat($hprocess, $ChatNr, $Message, $MemPtr = "create")
	If Not IsArray($hprocess) Then
		$hprocess = _GWCAMemOpen($hprocess)
		If Not IsArray($hprocess) Then Return
	EndIf

	$MemPtrCreated = False

	$StringInString = StringInStr($Message, "/")
	If $StringInString = 1 Then
		$ChatNr = 14
		$Message = StringSplit($Message, "/")
		$Message = $Message[2]
	EndIf

	$StringLen = StringLen($Message)
	If $MemPtr = "create" Then
		$MemPtrCreated = True
		$MemPtr = CmdCB($CA_AllocMem, ($StringLen + 1) * 2)
		$MemPtr = $MemPtr[1]
	EndIf

	_GWCAMemWrite($MemPtr, $hprocess, $Message, "wchar[" & $StringLen + 1 & "]")

	Cmd($CA_SendChat, $ChatNr, $MemPtr)

	If $MemPtrCreated = True Then Cmd($CA_FreeMem, 0, $MemPtr)
EndFunc
; END OF FILE

