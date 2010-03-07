; # -+-+-+-+-+-+-+ #
;   GWCA Constants
;	  By Sune C
; # +-+-+-+-+-+-+- #

#include-once

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
Global Enum $CA_GetCurrentTarget = 0x401, $CA_GetMyId, $CA_Casting, $CA_SkillRecharge, $CA_SkillAdrenaline, _
			$CA_StoreVar, $CA_SetLogAndHwnd, $CA_GetAgentAndTargetPtr, $CA_GetSkillbarSkillId, $CA_GetMyMaxHP, $CA_GetMyMaxEnergy, $CA_GetBuildNumber, _
			$CA_ChangeMaxZoom, $CA_GetLastDialogId, $CA_SetEngineHook, _
			$CA_Attack = 0x410, $CA_Move, $CA_UseSkill, _
			$CA_ChangeWeaponSet, $CA_ZoneMap, $CA_DropGold, $CA_GoNpc, $CA_GoPlayer, $CA_GoSignpost, $CA_UseAttackSkill, _
			$CA_EnterChallenge, $CA_OpenChest, $CA_SetEventSkillMode, $CA_UseSkillbarSkill, $CA_PickupItem, _
			$CA_UseSkillbarAttackSkill, $CA_Dialog, $CA_ChangeTarget, $CA_StatusBot, $CA_TargetNearestFoe, $CA_TargetNearestAlly, _
			$CA_TargetNearestItem, $CA_StatusDelay, $CA_TargetCalledTarget, $CA_UseHero1Skill, $CA_UseHero2Skill, $CA_UseHero3Skill, $CA_StatusMiss, _
			$CA_CancelAction, $CA_StatusTab, $CA_GetNamePtr, $CA_CommandHero1, $CA_CommandHero2, $CA_CommandHero3, $CA_CommandAll, $CA_ChangeDistrict, _
			$CA_Resign, $CA_ReturnToOutpost, $CA_GoAgent, $CA_DonateFaction, $CA_SetSkillbarSkill, $CA_ChangeSecondProfession, $CA_TargetNextPartyMember, _
			$CA_TargetNextFoe, $CA_SkipCinematic, $CA_DismissBuff, $CA_SendChat, _
			$CA_GetMapLoading = 0x440, $CA_GetMapId, $CA_GetPing, $CA_GetLoggedIn, $CA_GetDead, $CA_GetBalthFaction, $CA_GetKurzFaction, $CA_GetLuxonFaction, _
			$CA_GetTitleTreasure, $CA_GetTitleLucky, $CA_GetTitleUnlucky, $CA_GetTitleWisdom, $CA_GetTitleGamer, $CA_GetExperience, _
			$CA_GetAgentExist = 0x450, $CA_GetProfessions, $CA_GetPlayerNumber, _
			$CA_GetHP, $CA_GetRotation, _
			$CA_GetSkill, $CA_GetCoords, $CA_GetWeaponSpeeds, $CA_GetSpiritRange, $CA_GetTeamId, $CA_GetCombatMode, _
			$CA_GetModelMode, $CA_GetHpPips, $CA_GetEffects, $CA_GetHex, $CA_GetModelAnimation, $CA_GetEnergy, $CA_GetAgentPtr, _
			$CA_GetType, $CA_GetLevel, $CA_GetNameProperties, $CA_GetMaxId, $CA_GetMyNearestAgent, _
			$CA_GetMyDistanceToAgent, $CA_GetNearestAgentToAgent, $CA_GetDistanceFromAgentToAgent, $CA_GetNearestAgentToAgentEx, _
			$CA_GetModelState, $CA_GetIsAttacking, $CA_GetIsKnockedDown, $CA_GetIsMoving, $CA_GetIsDead, $CA_GetIsCasting, _
			$CA_GetFirstAgentByPlayerNumber, $CA_GetAllegiance, $CA_GetNearestEnemyToAgentEx, $CA_GetIsAttackedMelee, $CA_GetNearestItemToAgentEx, _
			$CA_GetNearestAgentByPlayerNumber, $CA_GetSpeed, $CA_GetNearestEnemyToAgentByAllegiance, $CA_GetNearestAliveEnemyToAgent, _
			$CA_GetWeaponType, $CA_GetNearestSignpostToAgent, $CA_GetNearestNpcToAgentByAllegiance, $CA_GetNearestAgentToCoords, $CA_GetVars, _
			$CA_GetNearestNpcToCoords, $CA_GetLoginNumber, $CA_GetNumberOfAgentsByPlayerNumber, $CA_GetNumberOfAliveEnemyAgents, $CA_GetNextItem, _
			$CA_GetTarget, $CA_SetAttribute, $CA_PlayerHasBuff, $CA_Hero1HasBuff, $CA_Hero2HasBuff, $CA_Hero3HasBuff, _
			$CA_GetGold = 0x510, $CA_GetBagSize, $CA_SetBag, $CA_GetItemId, _
			$CA_GetIdKit, $CA_IdentifyItem, $CA_IdentifyItemById, $CA_DepositGold, $CA_WithdrawGold, $CA_SellItem, $CA_SellItemById, _
			$CA_BuyIdKit, $CA_BuySuperiorIdKit, $CA_PrepareMoveItem, $CA_MoveItem, $CA_GetItemInfo, _
			$CA_UseItem, $CA_UseItemById, $CA_DropItem, $CA_DropItemById, _
			$CA_AcceptAllItems, $CA_GetItemLastModifier, $CA_FindItemByModelId, $CA_FindEmptySlot, $CA_FindGoldItem, _
			$CA_GetItemPositionByItemId, $CA_GetItemPositionByModelId, $CA_GetItemPositionByRarity, $CA_GetItemModelIdById, $CA_GetItemInfoById, _
			$CA_GetItemLastModifierById, $CA_EquipItem, $CA_EquipItemById, $CA_SalvageItem, $CA_GetSalvageKit, $CA_BuyItem, _
			$CA_GetItemIdByAgent, $CA_GetItemInfoByAgent, $CA_GetItemLastModifierByAgent, $CA_GetNearestItemByModelId, _
			$CA_GetTitleSunspear = 0x550, $CA_GetTitleLightbringer, $CA_GetTitleVanguard, $CA_GetTitleNorn, $CA_GetTitleAsura, $CA_GetTitleDeldrimor, _
			$CA_GetTitleNorthMastery, $CA_GetTitleDrunkard, $CA_GetTitleSweet, $CA_GetTitleParty, $CA_GetTitleCommander, $CA_GetTitleLuxon, $CA_GetTitleKurzick

Global Enum	$CA_AddHero = 0x580, $CA_KickHero, $CA_SwitchMode, $CA_AddNpc, $CA_KickNpc, $CA_TravelGH, $CA_LeaveGH, $CA_InitMapLoad, $CA_MapIsLoaded, _
			$CA_GetMapOverlayCoords, $CA_GetMapOverlayInfo, $CA_GetNearestMapOverlayToCoords, $CA_GetPartyInfo, $CA_ClearPacketQueue, $CA_SetHeroMode, _
			$CA_QuestCheck, $CA_QuestCoords, $CA_QuestActive, $CA_QuestAbandon, $CA_SetTeamSize, $CA_AllocMem, $CA_FreeMem, $CA_GetRegionAndLanguage, _
			$CA_TraderRequest, $CA_TraderCheck, $CA_TraderBuy


Global Enum $RARITY_WHITE = 0x3D, $RARITY_BLUE = 0x3F, $RARITY_PURPLE = 0x42, $RARITY_GOLD = 0x40, $RARITY_GREEN = 0x43

Global Enum $BAG_BackPack = 1, $BAG_BeltPouch, $BAG_Bag1, $BAG_Bag2, $BAG_EquipmentPack, $BAG_UnclaimedItems = 7, $BAG_Storage1, $BAG_Storage2, _
			$BAG_Storage3, $BAG_Storage4, $BAG_Storage5, $BAG_Storage6, $BAG_Storage7, $BAG_Storage8, $BAG_StorageAnniversary

Global Enum $HERO_Norgu = 1, $HERO_Goren, $HERO_Tahklora, $HERO_MasterOfWhispers, $HERO_AcolyteJin, $HERO_Koss, $HERO_Dunkoro, $HERO_AcolyteSousuke, $HERO_Melonni, _
			$HERO_ZhedShadowhoof, $HERO_GeneralMorgahn, $HERO_MargridTheSly, $HERO_Olias = 14, $HERO_Razah, $HERO_MOX, $HERO_Jora = 18, $HERO_PyreFierceshot, _
			$HERO_Livia = 21, $HERO_Kahmu, $HERO_Gwen, $HERO_Xandra, $HERO_Vekk, $HERO_Ogden

Global Enum $HEROMODE_Fight, $HEROMODE_Guard, $HEROMODE_Avoid

Global Const $FLAG_RESET = 0x7F800000

; VARIABLES

Opt("WinTitleMatchMode", -1) ;Make sure that Guild Wars receives the messages (1 = match start, 2 = substring, 3 = exact)

Global $cbType = "int" ;What to read the callback values as
Global $cbVar[2] ;Array for callback wParam and lParam
Global $sGW = "Guild Wars -" ;Name of window
Global $cGUI = 0 ;Init GUI hwnd var
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

Func WndCallback($hwnd, $msg, $wparam, $lparam)
	Switch($cbType)
		Case "float"
			$wparam = _IntToFloat($wparam)
			$lparam = _IntToFloat($lparam)
		Case "int"
			$wparam = Number($wparam)
			$lparam = Number($lparam)
		Case "hex" ;Do nothing
	EndSwitch

	$cbVar[0] = $wparam
	$cbVar[1] = $lparam
EndFunc

Func Cmd($uMsg, $wparam = 0, $lparam = 0)
	$bGWCA_INTERNAL = True
	$cbVar[0] = ""
	$cbVar[1] = ""
	DllCall("user32.dll", "lparam", "PostMessage", "hwnd", WinGetHandle($sGW), "int", $uMsg, "wparam", $wparam, "lparam", $lparam)
	$bGWCA_INTERNAL = False
EndFunc

Func CmdEx($uMsg, $wparam = 0, $lparam = 0)
	$bGWCA_INTERNAL = True
	$cbVar[0] = ""
	$cbVar[1] = ""
	DllCall("user32.dll", "lparam", "SendMessage", "hwnd", WinGetHandle($sGW), "int", $uMsg, "wparam", $wparam, "lparam", $lparam)
	$bGWCA_INTERNAL = False
EndFunc

Func CmdCB($uMsg, $wparam = 0)
	$bGWCA_INTERNAL = True
	$cbVar[0] = ""
	$cbVar[1] = ""
	DllCall("user32.dll", "lparam", "SendMessage", "hwnd", WinGetHandle($sGW), "int", $uMsg, "wparam", $wparam, "lparam", $cGUI)
	If $cbVar[0] = "" AND $cbVar[1] = "" Then SetError(-1)
	$bGWCA_INTERNAL = False
	Return $cbVar
EndFunc

Func CmdCBEx($uMsg, $wParam = 0, $msTimeout = 60)
	CmdCB($uMsg, $wParam)
	$tEscape = TimerInit()
	Do
		Sleep(0)
	Until TimerDiff($tEscape) > $msTimeout OR String($cbVar[0]) <> ""
EndFunc

Func MoveEx($x, $y, $random = 50)
	Cmd($CA_MOVE, _FloatToInt($x+Random(-$random,$random)), _FloatToInt($y+Random(-$random,$random)))
EndFunc

Func GetNearestAgentToCoords($x, $y)
	$oldCbType = $cbType

	$cbType = "int"
	CmdEx($CA_GetNearestAgentToCoords, _FloatToInt($x), _FloatToInt($y))
	CmdCB($CA_GetVars)

	$cbType = $oldCbType

	Return $cbVar[1]
EndFunc

Func GetNearestNPCToCoords($x, $y)
	$oldCbType = $cbType

	$cbType = "int"
	CmdEx($CA_GetNearestNpcToCoords, _FloatToInt($x), _FloatToInt($y))
	CmdCB($CA_GetVars)

	$cbType = $oldCbType

	Return $cbVar[1]
EndFunc

Func GetNearestMapOverlayToCoords($x, $y)
	$oldCbType = $cbType

	$cbType = "int"
	CmdEx($CA_GetNearestMapOverlayToCoords, _FloatToInt($x), _FloatToInt($y))
	CmdCB($CA_GetVars)

	$cbType = $oldCbType

	Return $cbVar[1]
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
		$oldCoords = $cbVar
		$cbType = "int"
		CmdCB($CA_GETDEAD)
		If $cbVar[0] = 1 Then Return

		$mStateOld = $mState
		$mState = CmdCB($CA_GetMapLoading)
		If $mState[0] <> $mStateOld[0] Then Return

		$cbType = "float"
		CmdCB($CA_GETCOORDS, -2)
		If $oldCoords[0] = $cbVar[0] AND $oldCoords[1] = $cbVar[1] Then
			$iBlocked += 1
			MoveEx($x, $y, $random)
		EndIf
	Until ComputeDistanceEx($cbVar[0], $cbVar[1], $x, $y) < 250 OR $iBlocked > 20
EndFunc

Func ComputeDistanceEx($x1, $y1, $x2, $y2)
	Return Sqrt(($y2 - $y1)^2 + ($x2 - $x1)^2)
EndFunc

Func UseSkillEx($iSkillSlot, $iTarget = 0)
	$oldCbType = $cbType

	$tDeadlock = TimerInit()
	$cbType = "int"
	Cmd($CA_USESKILLBARSKILL, $iSkillSlot, $iTarget)
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
