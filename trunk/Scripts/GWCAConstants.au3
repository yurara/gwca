; # -+-+-+-+-+-+-+ #
;   GWCA Constants
;	  By Sune C
; # +-+-+-+-+-+-+- #

#include-once

; The SkillLog structure for use with the Skill Log feature
Global $tagSKILLLOGSTRUCT = "long AgentId;long MyId;long SkillId;float Activation;byte TeamId;ushort Allegiance;float Distance;long Ping"

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
			$CA_Resign, $CA_ReturnToOutpost, $CA_GoAgent, $CA_DonateFaction, $CA_SetSkillbarSkill, $CA_ChangeSecondProfession, _
			$CA_GetMapLoading = 0x440, $CA_GetMapId, $CA_GetPing, $CA_GetLoggedIn, $CA_GetDead, $CA_GetBalthFaction, $CA_GetKurzFaction, $CA_GetLuxonFaction, _
			$CA_GetTitleTreasure, $CA_GetTitleLucky, $CA_GetTitleUnlucky, $CA_GetTitleWisdom, _
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
			$CA_GetNearestNpcToCoords, $CA_GetLoginNumber, $CA_GetNumberOfAgentsByPlayerNumber, _
			$CA_GetGold = 0x510, $CA_GetBagSize, $CA_SetBag, $CA_GetItemId, _
			$CA_GetIdKit, $CA_IdentifyItem, $CA_IdentifyItemById, $CA_DepositGold, $CA_WithdrawGold, $CA_SellItem, $CA_SellItemById, _
			$CA_BuyIdKit, $CA_BuySuperiorIdKit, $CA_PrepareMoveItem, $CA_MoveItem, $CA_GetItemInfo, _
			$CA_UseItem, $CA_UseItemById, $CA_DropItem, $CA_DropItemById, _
			$CA_AcceptAllItems, $CA_GetItemLastModifier, $CA_FindItemByModelId, _
			$CA_GetTitleSunspear = 0x550, $CA_GetTitleLightbringer, $CA_GetTitleVanguard, $CA_GetTitleNorn, $CA_GetTitleAsura, $CA_GetTitleDeldrimor, _
			$CA_GetTitleNorthMastery, $CA_GetTitleDrunkard, $CA_GetTitleSweet, $CA_GetTitleParty, $CA_GetTitleCommander, $CA_GetTitleLuxon, $CA_GetTitleKurzick, _
			$CA_AddHero = 0x580, $CA_KickHero, $CA_SwitchMode, $CA_AddNpc, $CA_KickNpc, $CA_TravelGH, $CA_LeaveGH


Global Enum $RARITY_WHITE = 0x3D, $RARITY_BLUE = 0x3F, $RARITY_PURPLE = 0x42, $RARITY_GOLD = 0x40, $RARITY_GREEN = 0x43

Global Enum $BAG_BackPack = 1, $BAG_BeltPouch, $BAG_Bag1, $BAG_Bag2, $BAG_EquipmentPack, $BAG_UnclaimedItems = 7, $BAG_Storage1, $BAG_Storage2, _
			$BAG_Storage3, $BAG_Storage4, $BAG_Storage5, $BAG_Storage6, $BAG_Storage7, $BAG_Storage8, $BAG_StorageAnniversary

Global Enum $HERO_Norgu = 1, $HERO_Goren, $HERO_Tahklora, $HERO_MasterOfWhispers, $HERO_AcolyteJin, $HERO_Koss, $HERO_Dunkoro, $HERO_AcolyteSousuke, $HERO_Melonni, _
			$HERO_ZhedShadowhoof, $HERO_GeneralMorgahn, $HERO_MargridTheSly, $HERO_Olias = 14, $HERO_Razah, $HERO_MOX, $HERO_Jora = 18, $HERO_PyreFierceshot, _
			$HERO_Livia = 21, $HERO_Kahmu, $HERO_Gwen, $HERO_Xandra, $HERO_Vekk, $HERO_Ogden

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
	Until TimerDiff($tEscape) > $msTimeout OR $cbVar[0] <> ""
EndFunc

Func MoveEx($x, $y, $random = 50)
	Cmd($CA_MOVE, _FloatToInt($x+Random(-$random,$random)), _FloatToInt($y+Random(-$random,$random)))
EndFunc

Func GetNearestAgentToCoords($x, $y)
	$oldCbType = $cbType

	$cbType = "int"
	Cmd($CA_GETNEARESTAGENTTOCOORDS, _FloatToInt($x), _FloatToInt($y))
	Sleep(50)
	CmdCB($CA_GETVARS)

	$cbType = $oldCbType

	Return $cbVar[1]
EndFunc

Func GetNearestNPCToCoords($x, $y)
	$oldCbType = $cbType

	$cbType = "int"
	Cmd($CA_GETNEARESTNPCTOCOORDS, _FloatToInt($x), _FloatToInt($y))
	Sleep(50)
	CmdCB($CA_GETVARS)

	$cbType = $oldCbType

	Return $cbVar[1]
EndFunc

Func TravelTo($iMapId)
	$tMap = TimerInit()
	Cmd($CA_ZONEMAP, $iMapId)
	Do
		Sleep(200)
		$aLoad = CmdCB($CA_GETMAPLOADING)
		If TimerDiff($tMap) > 20000 AND $aLoad[0] <> 2 Then Cmd($CA_ZONEMAP, $iMapId)
		$aTmpMap = CmdCB($CA_GETMAPID)
	Until $aLoad[0] = 0 AND $aTmpMap[0] = $iMapId
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
		If $cbVar[0] = 1 Then Return
		CmdCB($CA_SKILLRECHARGE, $iSkillSlot)
	Until $cbVar[0] <> 0 OR TimerDiff($tDeadlock) > 15000

	$cbType = $oldCbType
EndFunc

; END OF FILE
