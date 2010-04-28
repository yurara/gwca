;GWCA Interrupt Bot
#include <GUIConstantsEx.au3>
#include <EditConstants.au3>
#include <WindowsConstants.au3>
#include <StaticConstants.au3>
#include <ComboConstants.au3>
#include <Array.au3>
#include "GWCAConstants.au3"

Opt("GUIOnEventMode", 1)

Global $cGUI = GUICreate("GWCA Tool - Interrupt Bot", 300, 250) ;GUI for communication
Global $bEnabled = False ;Boolean for whether bot is enabled or not
Global $fSkillSet = "gwca-skillset.ini" ;Path to skillset file
Global $skillList ;Array that holds the list of normal skills
Global $specialList ;Array that holds the list of special skills
Global $lock = 0 ;Var that determines whether the Lock is on and who it's on
Global $skillDelay = 0 ;For use with delay.
Global $miss = 0 ;Miss chance thingy
Global $tAfterCast[2] = [0,0] ;For not using skills while in aftercast
Global $aSkillQueue[1] = [0] ;Skill Queue for the new mechanics
Global $aPrioritySkill[6] ;Data of the currently prioritized skill
Global $aPing[2] ;Array for saving current ping
Global $tUpdatePing = 0 ;Timer for renewing ping info
Global $bMiss = False ;Variable used by the Miss function
Global $fSkillNames = @ScriptDir & "\new list.txt" ;Path to skillnames file

;TAB GLOBALS
Global $iTab = 1 ;Number of the current tab
Global $aTabNormal[2] = ["", ""] ;Array of the normal skills
Global $aTabSpecial[2] = ["", ""] ;Array of the special skills
Global $aTabSkillSet[2] = ["", ""] ;Array of the current selected skill sets
Global $aTabSkillmode[2] = [4, 4] ;Array of the skill mode
Global $aTabSpecialLock[2] = [4, 4] ;Array of the special lock mode
Global $aTabSpecialSkill[2] = ["None", "None"] ;Array of the special skill
Global $aTabDelay[2] = ["", ""] ;Yeye
Global $aTabNormalSkills[16] = [4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4] ;Yup!

GUIRegisterMsg(0x4A, "SkillLogCallback")
GUIRegisterMsg(0x501, "SkillCancelCallback")

HotKeySet("{F5}", "LockOnOff")
HotKeySet("{F6}", "BotOnOff")
HotKeySet("{F7}", "DelayOnOff")
;HotKeySet("{F8}", "MissOnOff")
HotKeySet("{F8}", "SwapTab")

;# GUI SECTION #
GUISetOnEvent($GUI_EVENT_CLOSE, "EventHandler")

GUISetFont(8)
GUICtrlCreateMenu("")
GUICtrlSetState(-1, $GUI_DISABLE)
$itemSave = GUICtrlCreateMenuItem("&Save", -1)
GUICtrlSetOnEvent(-1, "EventHandler")
$itemDelete = GUICtrlCreateMenuItem("&Del", -1)
GUICtrlSetOnEvent(-1, "EventHandler")
$itemClear = GUICtrlCreateMenuItem("&Clr", -1)
GUICtrlSetOnEvent(-1, "EventHandler")
;GUICtrlCreateMenuItem("", -1)
$itemLock = GUICtrlCreateMenuitem("&Lock off", -1)
GUICtrlSetOnEvent(-1, "EventHandler")
$itemMiss = GUICtrlCreateMenuItem("&Miss off", -1)
GUICtrlSetOnEvent(-1, "EventHandler")
;GUICtrlCreateMenuItem("", -1)
;$itemInfo = GUICtrlCreateMenuItem("&Help", -1)
$itemTab = GUICtrlCreateMenuItem("&Swap (1)", -1)
GUICtrlSetOnEvent(-1, "EventHandler")
$editSkills = GUICtrlCreateEdit("", 0, 0, 300, 71, BitOR($ES_MULTILINE, $WS_VSCROLL))
$editSpecial = GUICtrlCreateEdit("", 0, 100, 300, 45, BitOR($ES_MULTILINE, $WS_VSCROLL))
$const_checkTop = 202
$check1 = GUICtrlCreateCheckbox("1",15, $const_checkTop)
$check2 = GUICtrlCreateCheckbox("2",50, $const_checkTop)
$check3 = GUICtrlCreateCheckbox("3",85, $const_checkTop)
$check4 = GUICtrlCreateCheckbox("4",120, $const_checkTop)
$check5 = GUICtrlCreateCheckbox("5",155, $const_checkTop)
$check6 = GUICtrlCreateCheckbox("6",190, $const_checkTop)
$check7 = GUICtrlCreateCheckbox("7", 225, $const_checkTop)
$check8 = GUICtrlCreateCheckbox("8", 260, $const_checkTop)
$labelDistance = GUICtrlCreateLabel("Max distance:", 7, 178)
$inputDistance = GUICtrlCreateInput("1250", 77, 175, 45, 20)
$labelActivation = GUICtrlCreateLabel("Min activation:", 180, 178)
$inputActivation = GUICtrlCreateInput("0.6", 253, 175, 35, 20)
$checkSkillmode = GUICtrlCreateCheckbox("Attack skills", 7, 150)
$checkLockmode = GUICtrlCreateCheckBox("Special lock", 90, 150)
$checkDelay = GUICtrlCreateCheckbox("Delay", 130, 174)
$btnOnOff = GUICtrlCreateButton("Enable bot", 225, 75, 70, 20)
GUICtrlSetOnEvent(-1, "EventHandler")
$comboSkillSet = GUICtrlCreateCombo("", 5, 75, 215, 20, BitOr($CBS_SORT, $GUI_SS_DEFAULT_COMBO, $CBS_DROPDOWNLIST))
GUICtrlSetOnEvent(-1, "EventHandler")
$labelSpecial = GUICtrlCreateLabel("Special skill: ", 176, 154)
$comboSpecial = GUICtrlCreateCombo("", 238, 150, 50, 20, BitOR($GUI_SS_DEFAULT_COMBO, $CBS_DROPDOWNLIST))
GUICtrlSetData(-1, "None|1|2|3|4|5|6|7|8", "None")
GUICtrlSetState($checkDelay, $GUI_CHECKED)

GUISetBkColor(0xCCCCCC)
UpdateSkillsets()

GUISetState(True, $cGUI)
;# END OF GUI  #

;# LOOP #
$cbType = "int"

While 1
	If TimerDiff($aPrioritySkill[5]) >= ($aPrioritySkill[2] * 1000) AND $aPrioritySkill[0] <> "" Then ClearPrioritySkill()
	For $i = 1 To $aSkillQueue[0] Step 1 ;Queue handling and cleaning up
		If $i > $aSkillQueue[0] Then ExitLoop
		$aSkillInfo = SplitSkillInfo($aSkillQueue[$i])
		If Not IsArray($aSkillInfo) Then ContinueLoop

		If TimerDiff($aSkillInfo[5]) >= ($aSkillInfo[2] * 1000) Then DeleteSkillFromQueue($aSkillQueue, $i)
		If TimerDiff($aPrioritySkill[5]) >= ($aPrioritySkill[2] * 1000) Then
			MakePrioritySkill($aSkillInfo)
			DeleteSkillFromQueue($aSkillQueue, $i)
		EndIf
		If $aSkillInfo[4] > $aPrioritySkill[4] Then
			MakePrioritySkill($aSkillInfo)
			DeleteSkillFromQueue($aSkillQueue, $i)
		EndIf
	Next

	If $bEnabled Then
		If $aPrioritySkill[0] <> "" Then
			$bUsed = False

			If TimerDiff($tUpdatePing) > 500 Then ;Don't waste time updating ping all the time
				$tUpdatePing = TimerInit()
				$aPing = CmdCB($CA_GETPING)
			EndIf

			If GUICtrlRead($checkDelay) <> 1 Then
				$skillTime = ComputeSkillTime($aPrioritySkill, False)
				$skillTime -= $aPing[0] / 1.25

				If $skillTime > 375 AND TimerDiff($skillDelay) > GetDelayMode() Then
					CmdCB($CA_CASTING)
					If $cbVar[0] > 0 Then $bUsed = True

					If Not $bUsed Then
						If $aPrioritySkill[4] = 2 Then ;Special skill
							$specialSkill = GUICtrlRead($comboSpecial) ;Load in the special skill
							If $specialSkill <> "None" Then ;If no special skill simply skip
								CmdCB($CA_SKILLRECHARGE, $specialSkill)
								If $cbVar[0] = 0 Then ;Check if recharging
									Cmd($CA_UseSkill, $specialSkill, $aPrioritySkill[1]) ;Fire!
									$skillDelay = TimerInit() ;Reset the skill delay timer
									$bUsed = True
									ClearPrioritySkill()
								EndIf
							EndIf
						EndIf
					EndIf

					If Not $bUsed Then
						For $i = 1 To 8 Step 1 ;Step through all 8 skills
							If GUICtrlRead(Eval("check"&$i)) = 1 Then ;If enabled then use it
								$skillTime = (ComputeSkillTime($aPrioritySkill, False) - ($aPing[0] / 1.25))
								If $skillTime < 370 Then
									$bUsed = True
									ClearPrioritySkill()
									ExitLoop
								EndIf
								CmdCB($CA_SKILLRECHARGE, $i)
								If $cbVar[0] = 0 Then ;Check if recharging
									Cmd($CA_UseSkill, $i, $aPrioritySkill[1]) ;Fire!
									$skillDelay = TimerInit() ;Reset the skill delay timer
									$bUsed = True
									ClearPrioritySkill()
									ExitLoop ;You hopefully won't need to interrupt it more than once!
								EndIf
							EndIf
						Next
					EndIf
					$bMiss = False
				EndIf

			Else
				$skillTime = ComputeSkillTime($aPrioritySkill)
				$skillTime -= $aPing[0] / 1.25

				If $skillTime < ComputeRuptTime($aPrioritySkill) AND $skillTime > 375 AND TimerDiff($skillDelay) > GetDelayMode() Then
					If $miss Then
						If Random(1,7,1) = 1 Then
							$bMiss = True
							Do
								Sleep(10)
							Until (ComputeSkillTime($aPrioritySkill) - ($aPing[0] / 1.25)) < 200
						EndIf
					EndIf

					CmdCB($CA_CASTING)
					If $cbVar[0] > 0 Then $bUsed = True

					If Not $bUsed Then
						If $aPrioritySkill[4] = 2 Then ;Special skill
							$specialSkill = GUICtrlRead($comboSpecial) ;Load in the special skill
							If $specialSkill <> "None" Then ;If no special skill simply skip
								CmdCB($CA_SKILLRECHARGE, $specialSkill)
								If $cbVar[0] = 0 Then ;Check if recharging
									Cmd($CA_UseSkill, $specialSkill, $aPrioritySkill[1]) ;Fire!
									$skillDelay = TimerInit() ;Reset the skill delay timer
									$bUsed = True
									ClearPrioritySkill()
								EndIf
							EndIf
						EndIf
					EndIf

					If Not $bUsed Then
						For $i = 1 To 8 Step 1 ;Step through all 8 skills
							If GUICtrlRead(Eval("check"&$i)) = 1 Then ;If enabled then use it
								$skillTime = (ComputeSkillTime($aPrioritySkill) - ($aPing[0] / 1.25))
								If $skillTime < 370 AND Not $bMiss Then
									$bUsed = True
									ClearPrioritySkill()
									ExitLoop
								EndIf
								CmdCB($CA_SKILLRECHARGE, $i)
								If $cbVar[0] = 0 Then ;Check if recharging
									Cmd($CA_UseSkill, $i, $aPrioritySkill[1]) ;Fire!
									$skillDelay = TimerInit() ;Reset the skill delay timer
									$bUsed = True
									ClearPrioritySkill()
									ExitLoop ;You hopefully won't need to interrupt it more than once!
								EndIf
							EndIf
						Next
					EndIf
					$bMiss = False
				EndIf
			EndIf
		EndIf
	EndIf

	Sleep(10) ;Throttle CPU
WEnd

;# FUNCTIONS #
Func SkillLogCallback($hwnd, $msg, $wparam, $lparam)
	Local $skillName, $bFound = 0, $specialSkill, $skillTime

	$recvCDS = DllStructCreate("ptr;dword;ptr", $lparam) ;COPYDATASTRUCT

	If DllStructGetData($recvCDS, 1) = 1 AND $bEnabled Then
		$skillStruct = DllStructCreate($tagSKILLLOGSTRUCT, DllStructGetData($recvCDS, 3))

		If DllStructGetData($skillStruct, "Allegiance") <> 0x300 AND DllStructGetData($skillStruct, "Allegiance") <> 0x301 Then Return
		If DllStructGetData($skillStruct, "Distance") > GUICtrlRead($inputDistance) Then Return
		If DllStructGetData($skillStruct, "Activation") < GUICtrlRead($inputActivation) Then Return

		$skillName = ReadSkillName($fSkillNames, DllStructGetData($skillStruct, "SkillId"))
		For $i = 1 To $specialList[0] Step 1
			If $skillName = $specialList[$i] Then ;Check the name against special list first (fool-proof if you have duplicates!)
				$bFound = 2
				ExitLoop
			EndIf
		Next
		If Not $bFound Then ;Make sure we don't overwrite the special skill if there's a duplicate :O
			For $i = 1 To $skillList[0] Step 1
				If $skillName = $skillList[$i] Then ;Check the name of the skill against your list
					$bFound = 1
					ExitLoop
				EndIf
			Next
		EndIf

		;If $lock <> 0 AND GUICtrlRead($checkLockmode) = 4 AND $bFound = 1 Then
		;	If $lock <> DllStructGetData($skillStruct, "AgentId") Then $bFound = 0
		;ElseIf $lock = 0 AND GUICtrlRead($checkLockmode) = 4 AND $bFound = 1 Then
		;	$bFound = 0
		;ElseIf GUICtrlRead($checkLockmode) = 1 AND $bFound = 2 Then
		;	If $lock = 0 OR $lock <> DllStructGetData($skillStruct, "AgentId") Then $bFound = 0
		;EndIf

		If GUICtrlRead($checkLockmode) = 1 AND $bFound = 2 Then
			If $lock = 0 OR $lock <> DllStructGetData($skillStruct, "AgentId") Then $bFound = 0
		ElseIf GUICtrlRead($checkLockmode) = 4 Then
			If $lock <> 0 AND $lock = DllStructGetData($skillStruct, "AgentId") Then $bFound = 2
		EndIf

		If $bFound Then
			AddSkillToQueue($aSkillQueue, $skillName, DllStructGetData($skillStruct, "AgentId"), _
				DllStructGetData($skillStruct, "Activation"), DllStructGetData($skillStruct, "Distance"), _
				$bFound, DllStructGetData($skillStruct, "Ping"))
		EndIf
	EndIf

	#cs
	If DllStructGetData($recvCDS, 1) = 1 AND $bEnabled Then ;Check that bot is on and the struct is meant for our handling
		$skillStruct = DllStructCreate($tagSKILLLOGSTRUCT, DllStructGetData($recvCDS, 3)) ;LOGGEDSKILLSTRUCT

		;If DllStructGetData($skillStruct, "MyId") = DllStructGetData($skillStruct, "AgentId") AND DllStructGetData($skillStruct, "Activation") > 0 Then
		;	$tAfterCast[0] = TimerInit()
		;	$tAfterCast[1] = DllStructGetData($skillStruct, "Activation") + 750
		;EndIf
		If DllStructGetData($skillStruct, "Casting") == 1 Then Return ;Don't fire interrupts while you're using skills youself
		If DllStructGetData($skillStruct, "Allegiance") == 0x300 Then ;Only fire interrupts at enemies (foes) doh!
			If DllStructGetData($skillStruct, "Distance") <= GUICtrlRead($inputDistance) Then ;Check that they're in the desired range
				If DllStructGetData($skillStruct, "Activation") >= GUICtrlRead($inputActivation) Then ;Check that it's not faster than the desired activation
					$skillName = ReadSkillName($fSkillNames, DllStructGetData($skillStruct, "SkillId")) ;Load the skill name using skill id
					For $i = 1 To $specialList[0] Step 1
						If $skillName = $specialList[$i] Then ;Check the name against special list first (fool-proof if you have duplicates!)
							$bFound = 2
							ExitLoop
						EndIf
					Next
					If Not $bFound Then ;Make sure we don't overwrite the special skill if there's a duplicate :O
						For $i = 1 To $skillList[0] Step 1
							If $skillName = $skillList[$i] Then ;Check the name of the skill against your list
								$bFound = 1
								ExitLoop
							EndIf
						Next
					EndIf

					If GUICtrlRead($checkLockmode) = 1 AND $bFound = 2 Then
						If $lock = 0 OR $lock <> DllStructGetData($skillStruct, "AgentId") Then $bFound = 0
					ElseIf GUICtrlRead($checkLockmode) = 4 Then
						If $lock <> 0 AND $lock = DllStructGetData($skillStruct, "AgentId") Then $bFound = 2
					EndIf

					If $bFound > 0 Then
						$skillTime = DllStructGetData($skillStruct, "Activation") * 1000
						$skillTime -= DllStructGetData($skillStruct, "Ping")

						;$sleepTime = CastDiff($tAfterCast)
						;If $sleepTime < 0 Then
						;	$sleepTime = Abs($sleepTime)
						;Else
						;	$sleepTime = 0
						;EndIf

						;$skillTime -= $sleepTime

						If $skillTime <= 250 Then Return

						;If $sleepTime > 0 Then Sleep($sleepTime)

						If GUICtrlRead($checkDelay) = 1 Then ;Delay on
							If TimerDiff($skillDelay) < 1250 Then Return ;2700 Then Return ;Dont spam interrupts

							If GUICtrlRead($checkSkillmode) = 1 Then $skillTime -= (FlightTime(DllStructGetData($skillStruct, "Distance"), 0.42) * 1000)

							If $skillTime < 370 Then
								Return
							ElseIf $skillTime > 800 Then
								Sleep(450)

							Else
								Sleep($skillTime-370)
							EndIf
						Else
							If TimerDiff($skillDelay) < 1000 Then Return
						EndIf

						If $miss Then
							If Random(1,7,1) = 1 Then Sleep(300)
						EndIf

						If $bFound = 2 Then
							$specialSkill = GUICtrlRead($comboSpecial) ;Load in the special skill
							If $specialSkill <> "None" Then ;If no special skill simply skip
								If DllStructGetData($skillStruct, "Recharge"&$specialSkill) = 0 Then ;Check if recharging
									Cmd(GetSkillMode(), $specialSkill, DllStructGetData($skillStruct, "AgentId")) ;Fire!
									$skillDelay = TimerInit() ;Reset the skill delay timer
									Return ;Remember to return as we don't want to interrupt something twice
								EndIf
							EndIf
						EndIf

						For $i = 1 To 8 Step 1 ;Step through all 8 skills
							If GUICtrlRead(Eval("check"&$i)) = 1 Then ;If enabled then use it
								If DllStructGetData($skillStruct, "Recharge"&$i) = 0 Then ;Check if recharging
									Cmd(GetSkillMode(), $i, DllStructGetData($skillStruct, "AgentId")) ;Fire!
									$skillDelay = TimerInit() ;Reset the skill delay timer
									Return ;You hopefully won't need to interrupt it more than once!
								EndIf
							EndIf
						Next
					EndIf
				EndIf
			EndIf
		EndIf
	EndIf
	#ce
EndFunc

Func SkillCancelCallback($hwnd, $msg, $wparam, $lparam)
	Local $skillName = "", $bDeleted = 0

	If $bEnabled Then
		$skillName = ReadSkillName($fSkillNames, $lparam)
		$wparam = Number($wparam)

		If $aPrioritySkill[0] = $skillName AND $aPrioritySkill[1] = $wparam Then
			ClearPrioritySkill()
			$bDeleted += 1
			Return
		EndIf

		For $i = 1 To $aSkillQueue[0] Step 1
			$aSkillSplit = SplitSkillInfo($aSkillQueue[$i])
			If $aSkillSplit[0] = $skillName AND $aSkillSplit[1] = $wparam Then
				DeleteSkillFromQueue($aSkillQueue, $i)
				$bDeleted += 1
				Return
			EndIf
		Next
	EndIf
EndFunc

Func ReadSkillName($fSkillFile, $SkillId)
	$SkillId = Number($SkillId) ;Convert the Id to number (from Hex, String, etc.)
	If StringMid($fSkillFile, 2, 2) = ":\" OR StringMid($fSkillFile, 2, 2) = ":/" Then ;If it's a file
		$fRead = FileRead($fSkillFile) ;Read it up
	Else
		$fRead = $fSkillFile ;If not, presume it's a cached list
	EndIf
	$fPos = StringInStr($fRead, $SkillId & "|") ;Check for existance
	If Not $fPos Then Return $SkillId ;If not, return the Id

	$iNameStartPos = $fPos + StringLen($SkillId) + 1 ;Find out where the skillname starts
	$nextLine = StringInStr($fRead, @CRLF, 0, 1, $iNameStartPos) ;Find out where the next line begins
	$count =  $nextLine - $iNameStartPos ;Compute length of name
	$sName = StringMid($fRead, $iNameStartPos, $count) ;Load it in
	Return $sName
EndFunc

Func UpdateSkillsets()
	GUICtrlSetData($comboSkillSet, "") ;Clear the old list of skillsets

	$setNames = IniReadSectionNames($fSkillSet)
	If Not @error Then
		For $i = 1 To $setNames[0] Step 1
			GUICtrlSetData($comboSkillSet, $setNames[$i])
		Next
	EndIf
EndFunc

Func GetSkillMode()
	Return $CA_UseSkill
EndFunc

Func LockOnOff()
	If WinActive($sGW) OR WinActive("GWCA Tool - Interrupt Bot") Then
		#cs
		$cbType = "int"
		CmdCB($CA_GETCURRENTTARGET)
		If @error OR $cbVar[0] = 0 Then
			;MsgBox(48, "No target", "Either Guild Wars ain't running, or you're simply not targetting anything!")
			$lock = 0
			GUICtrlSetData($itemLock, "Lock off")
		Else
			$lock = $cbVar[0]
			GUICtrlSetData($itemLock, "Lock on")
			Cmd($CA_STATUSBOT, 0, 1)
		EndIf
		#ce
		Switch($lock)
			Case 0
				$cbType = "int"
				CmdCB($CA_GETCURRENTTARGET)
				If @error OR $cbVar[0] = 0 Then
					;MsgBox(48, "No target", "Either Guild Wars ain't running, or you're simply not targetting anything!")
					$lock = 0
					GUICtrlSetData($itemLock, "Lock off")
				Else
					$lock = $cbVar[0]
					GUICtrlSetData($itemLock, "Lock on")
					WriteWhisper(WinGetProcess($sGW), "Bot", "Locked on "&$lock)
				EndIf
			Case Else
				$lock = 0
				GUICtrlSetData($itemLock, "Lock off")
				WriteWhisper(WinGetProcess($sGW), "Bot", "Lock is off")
		EndSwitch
	Else
		HotKeySet(@HotKeyPressed)
		Send(@HotKeyPressed)
		HotKeySet(@HotKeyPressed, "LockOnOff")
	EndIf
EndFunc

Func BotOnOff()
	If WinActive($sGW) OR WinActive("GWCA Tool - Interrupt Bot") Then
		$bEnabled = Not $bEnabled
		If $bEnabled Then
			WriteWhisper(WinGetProcess($sGW), "Bot", "Interrupt Bot enabled!")
			GUICtrlSetData($btnOnOff, "Disable bot") ;Disable most of the GUI and update button text
			GUICtrlSetState($editSkills, $GUI_DISABLE)
			GUICtrlSetState($editSpecial, $GUI_DISABLE)
			GUICtrlSetState($check1, $GUI_DISABLE)
			GUICtrlSetState($check2, $GUI_DISABLE)
			GUICtrlSetState($check3, $GUI_DISABLE)
			GUICtrlSetState($check4, $GUI_DISABLE)
			GUICtrlSetState($check5, $GUI_DISABLE)
			GUICtrlSetState($check6, $GUI_DISABLE)
			GUICtrlSetState($check7, $GUI_DISABLE)
			GUICtrlSetState($check8, $GUI_DISABLE)
			GUICtrlSetState($inputActivation, $GUI_DISABLE)
			GUICtrlSetState($inputDistance, $GUI_DISABLE)
			GUICtrlSetState($comboSkillSet, $GUI_DISABLE)
			GUICtrlSetState($itemSave, $GUI_DISABLE)
			GUICtrlSetState($itemDelete, $GUI_DISABLE)
			GUICtrlSetState($itemClear, $GUI_DISABLE)
			;GUICtrlSetState($itemInfo, $GUI_DISABLE)
			GUICtrlSetState($checkSkillmode, $GUI_DISABLE)
			GUICtrlSetState($comboSpecial, $GUI_DISABLE)
			GUICtrlSetState($checkLockmode, $GUI_DISABLE)
			$skillList = StringSplit(GUICtrlRead($editSkills), ",") ;Split skills by delimiter ,
			$specialList = StringSplit(GUICtrlRead($editSpecial), ",") ;Same
			For $i = 1 To $skillList[0] Step 1 ;Loop through to remove all odd spaces
				Do
					If StringLeft($skillList[$i], 1) = " " Then $skillList[$i] = StringTrimLeft($skillList[$i], 1)
					If StringRight($skillList[$i], 1) = " " Then $skillList[$i] = StringTrimRight($skillList[$i], 1)
				Until StringLeft($skillList[$i], 1) <> " " AND StringRight($skillList[$i], 1) <> " "
			Next
			For $i = 1 To $specialList[0] Step 1 ;Exactly the same
				Do
					If StringLeft($specialList[$i], 1) = " " Then $specialList[$i] = StringTrimLeft($specialList[$i], 1)
					If StringRight($specialList[$i], 1) = " " Then $specialList[$i] = StringTrimRight($specialList[$i], 1)
				Until StringLeft($specialList[$i], 1) <> " " AND StringRight($specialList[$i], 1) <> " "
			Next
			Cmd($CA_SETLOGANDHWND, 1, $cGUI) ;Turn the baby on!

		Else
			Cmd($CA_SETLOGANDHWND, 0, 0) ;Turn her off :\
			WriteWhisper(WinGetProcess($sGW), "Bot", "Interrupt Bot disabled!")
			GUICtrlSetData($btnOnOff, "Enable bot") ;Enable the GUI and update button text
			GUICtrlSetState($editSkills, $GUI_ENABLE)
			GUICtrlSetState($editSpecial, $GUI_ENABLE)
			GUICtrlSetState($check1, $GUI_ENABLE)
			GUICtrlSetState($check2, $GUI_ENABLE)
			GUICtrlSetState($check3, $GUI_ENABLE)
			GUICtrlSetState($check4, $GUI_ENABLE)
			GUICtrlSetState($check5, $GUI_ENABLE)
			GUICtrlSetState($check6, $GUI_ENABLE)
			GUICtrlSetState($check7, $GUI_ENABLE)
			GUICtrlSetState($check8, $GUI_ENABLE)
			GUICtrlSetState($inputActivation, $GUI_ENABLE)
			GUICtrlSetState($inputDistance, $GUI_ENABLE)
			GUICtrlSetState($comboSkillSet, $GUI_ENABLE)
			GUICtrlSetState($itemSave, $GUI_ENABLE)
			GUICtrlSetState($itemDelete, $GUI_ENABLE)
			GUICtrlSetState($itemClear, $GUI_ENABLE)
			;GUICtrlSetState($itemInfo, $GUI_ENABLE)
			GUICtrlSetState($checkSkillmode, $GUI_ENABLE)
			GUICtrlSetState($comboSpecial, $GUI_ENABLE)
			GUICtrlSetState($checkLockmode, $GUI_ENABLE)
		EndIf
	Else
		HotKeySet(@HotKeyPressed)
		Send(@HotKeyPressed)
		HotKeySet(@HotKeyPressed, "BotOnOff")
	EndIf
EndFunc

Func DelayOnOff()
	If WinActive($sGW) OR WinActive("GWCA Tool - Interrupt Bot") Then
		If GUICtrlRead($checkDelay) = 1 Then
			WriteWhisper(WinGetProcess($sGW), "Bot", "Delay is off")
			GUICtrlSetState($checkDelay, $GUI_UNCHECKED)
		Else
			WriteWhisper(WinGetProcess($sGW), "Bot", "Delay is on")
			GUICtrlSetState($checkDelay, $GUI_CHECKED)
		EndIf
	Else
		HotKeySet(@HotKeyPressed)
		Send(@HotKeyPressed)
		HotKeySet(@HotKeyPressed, "BotOnOff")
	EndIf
EndFunc

Func MissOnOff()
	If WinActive($sGW) OR WinActive("GWCA Tool - Interrupt Bot") Then
		If $miss Then
			$miss = Not $miss
			WriteWhisper(WinGetProcess($sGW), "Bot", "Random miss is off")
			GUICtrlSetData($itemMiss, "Miss off")
		Else
			$miss = Not $miss
			WriteWhisper(WinGetProcess($sGW), "Bot", "Random miss is on")
			GUICtrlSetData($itemMiss, "Miss on")
		EndIf
	Else
		HotKeySet(@HotKeyPressed)
		Send(@HotKeyPressed)
		HotKeySet(@HotKeyPressed, "BotOnOff")
	EndIf
EndFunc

Func FlightTime($distance, $bowTime = 0.42)
	Return $distance / 1000 * $bowTime
EndFunc

Func CastDiff($aCastTime)
	If Not IsArray($aCastTime) Then Return 0
	Return TimerDiff($aCastTime[0]) - $aCastTime[1]
EndFunc

Func ClearQueue(ByRef $aQueue)
	If Not IsArray($aQueue) Then Return 0

	Dim $aQueue[1] = [0]

	Return 1
EndFunc

Func AddSkillToQueue(ByRef $aQueue, $iSkill, $iAgent, $fActivation, $fDistance, $iPriority, $iPing)
	If Not IsArray($aQueue) Then Return 0

	$aQueue[0] += 1
	ReDim $aQueue[$aQueue[0]+1]
	$aQueue[$aQueue[0]] = $iSkill&","&$iAgent&","&$fActivation&","&$fDistance&","&$iPriority&","&TimerInit()&","&$iPing

	Return 1
EndFunc

Func SplitSkillInfo($sSkillInfo)
	Return StringSplit($sSkillInfo, ",", 2)
EndFunc

Func DeleteSkillFromQueue(ByRef $aQueue, $iIndex)
	If Not IsArray($aQueue) Then Return 0
	If $iIndex > $aQueue[0] Then Return 0
	$aQueue[0] -= 1
	Return _ArrayDelete($aQueue, $iIndex)
EndFunc

Func MakePrioritySkill($aSkillInfo)
	$aPrioritySkill = $aSkillInfo
EndFunc

Func ClearPrioritySkill()
	$aPrioritySkill[0] = ""
	For $i = 1 To UBound($aPrioritySkill)-1 Step 1
		$aPrioritySkill[$i] = 0
	Next
EndFunc

Func GetDelayMode()
	Switch(GUICtrlRead($checkDelay))
		Case 1
			Return 1600
		Case Else
			Return 1400
	EndSwitch
EndFunc

Func ComputeSkillTime($aSkill, $bReduceLong = True)
	If Not IsArray($aSkill) Then Return -1
	Local $iTime

	$iTime = ($aSkill[2] * 1000) - 50
	$iTime -= TimerDiff($aSkill[5])
	If GUICtrlRead($checkSkillmode) = 1 Then $iTime -= (FlightTime($aSkill[3], 0.41) * 1000)

	Return $iTime
EndFunc

Func ComputeRuptTime($aSkill)
	Select
		Case ($aSkill[2] * 1000) < 800
			Return 425
		Case ($aSkill[2] * 1000) < 1100
			Return 520
		Case ($aSkill[2] * 1000) < 2100
			Return 700
		Case Else
			Return 800
	EndSelect
EndFunc

Func LoadTab($iTab)
	Local $i = $iTab -1

	GUICtrlSetData($editSkills, $aTabNormal[$i])
	GUICtrlSetData($editSpecial, $aTabSpecial[$i])
	If $aTabSkillSet[$i] <> "" Then GUICtrlSetData($comboSkillSet, $aTabSkillSet[$i])
	GUICtrlSetState($checkSkillmode, $aTabSkillmode[$i])
	GUICtrlSetState($checkLockmode, $aTabSpecialLock[$i])
	GUICtrlSetData($comboSpecial, $aTabSpecialSkill[$i])
	GUICtrlSetState($checkDelay, $aTabDelay[$i])

	For $j = 1 To 8 Step 1
		GUICtrlSetState(Eval("check"&$j), $aTabNormalSkills[$j + ($i * 8) - 1])
	Next
EndFunc

Func SwapTab()
	If WinActive($sGW) OR WinActive("GWCA Tool - Interrupt Bot") Then
		If $iTab = 1 Then
			$iTab = 2
			$aTabNormal[0] = GUICtrlRead($editSkills)
			$aTabSpecial[0] = GUICtrlRead($editSpecial)
			$aTabSkillSet[0] = GUICtrlRead($comboSkillSet)
			$aTabSkillmode[0] = GUICtrlRead($checkSkillmode)
			$aTabSpecialLock[0] = GUICtrlRead($checkLockmode)
			$aTabSpecialSkill[0] = GUICtrlRead($comboSpecial)
			$aTabDelay[0] = GUICtrlRead($checkDelay)
			For $i = 1 To 8 Step 1
				$aTabNormalSkills[$i-1] = GUICtrlRead(Eval("check"&$i))
			Next

		ElseIf $iTab = 2 Then
			$iTab = 1
			$aTabNormal[1] = GUICtrlRead($editSkills)
			$aTabSpecial[1] = GUICtrlRead($editSpecial)
			$aTabSkillSet[1] = GUICtrlRead($comboSkillSet)
			$aTabSkillmode[1] = GUICtrlRead($checkSkillmode)
			$aTabSpecialLock[1] = GUICtrlRead($checkLockmode)
			$aTabSpecialSkill[1] = GUICtrlRead($comboSpecial)
			$aTabDelay[1] = GUICtrlRead($checkDelay)
			For $i = 1 To 8 Step 1
				$aTabNormalSkills[$i+7] = GUICtrlRead(Eval("check"&$i))
			Next
		EndIf

		For $i = 1 To $aSkillQueue[0] Step 1
			DeleteSkillFromQueue($aSkillQueue, 1)
		Next

		GUICtrlSetData($itemTab, "Swap ("&$iTab&")")
		LoadTab($iTab)
		WriteWhisper(WinGetProcess($sGW), "Bot", "Switched to tab "&$iTab)
	Else
		HotKeySet(@HotKeyPressed)
		Send(@HotKeyPressed)
		HotKeySet(@HotKeyPressed, "BotOnOff")
	EndIf
EndFunc

Func EventHandler()
	Switch(@GUI_CtrlId)
		Case $GUI_EVENT_CLOSE
			Exit

		Case $itemLock
			LockOnOff()

		Case $itemMiss
			MissOnOff()

		Case $itemClear
			Select
			Case GUICtrlRead($editSkills) <> "" OR GUICtrlRead($editSpecial) <> ""
				$bGoThrough = MsgBox(1, "Clear skills", "Are you sure you want to clear the current skills?")
				If $bGoThrough = 1 Then
					GUICtrlSetData($editSkills, "")
					GUICtrlSetData($editSpecial, "")
					UpdateSkillsets()
				EndIf
			Case Else
				MsgBox(48, "No skills to clear", "You cannot clear the current skills when there are none!")
			EndSelect

		Case $itemSave
			Select
			Case GUICtrlRead($editSkills) <> "" OR GUICtrlRead($editSpecial) <> ""
				$sSetName = InputBox("Save skillset", "Please enter a name for the skillset you're saving", GUICtrlRead($comboSkillSet))
				Select
				Case Not @error AND $sSetName <> ""
					IniWrite($fSkillSet, $sSetName, "Skills", GUICtrlRead($editSkills))
					IniWrite($fSkillSet, $sSetName, "Special", GUICtrlRead($editSpecial))
					UpdateSkillsets()
					GUICtrlSetData($comboSkillSet, $sSetName)
				Case $sSetName = "" AND Not @error
					MsgBox(48, "No name", "You forgot to specify a name for the skillset!")
				EndSelect
			Case Else
				MsgBox(48, "No skills written", "You didn't write any skills into any of the lists!")
			EndSelect

		Case $itemDelete
			Select
			Case GUICtrlRead($comboSkillSet) <> ""
				$bGoThrough = MsgBox(1, "Delete skillset", "Are you sure you want to delete skillset "&GUICtrlRead($comboSkillSet)&"?")
				If $bGoThrough = 1 Then
					IniDelete($fSkillSet, GUICtrlRead($comboSkillSet))
					UpdateSkillsets()
				EndIf
			Case Else
				MsgBox(48, "No skillset selected", "You have to select the skillset you wish to delete!")
			EndSelect

		;Case $itemInfo
		;	MsgBox(0, "Help", "If you really fail so much that you need help for understanding this tool then you fail!"&@LF& _
		;			"Only thing that you really need to know is that the upper editbox (the biggest) is for the ""normal"" skills and the lower one (the smallest)"& _
		;			" is for the ""special"" skills that need special treatment xD"&@LF&@LF& _
		;			"Common distances:"&@LF&"Melee = 150, Aggro = 1012, Spell = 1247, Recurve = 1250, Longbow = 1450"&@LF&@LF& _
		;			"The lock mechanism is very advanced and has two modes. Use wisely.")

		Case $itemTab
			SwapTab()

		Case $comboSkillSet
			GUICtrlSetData($editSkills, IniRead($fSkillSet, GUICtrlRead($comboSkillSet), "Skills", ""))
			GUICtrlSetData($editSpecial, IniRead($fSkillSet, GUICtrlRead($comboSkillSet), "Special", ""))

		Case $btnOnOff
			BotOnOff()
	EndSwitch
EndFunc
