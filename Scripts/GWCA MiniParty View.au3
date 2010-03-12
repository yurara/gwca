#include <GUIConstantsEx.au3>
#include <GDIPlus.au3>
#include <WinAPI.au3>
#include <WindowsConstants.au3>
#include "GWCAConstants.au3"

Opt("GUIOnEventMode", True)
Opt("MouseCoordMode", 2)

_GDIPlus_Startup()

Global $numOfTeams = 2
Global $teamSize = 8

Global $gap = 5
Global $hpHeight = 22
Global $hpSize = 200
Global $height = $hpHeight * $teamSize + 1
Global $width = ($hpSize * $numOfTeams) + ($gap * ($numOfTeams - 1)) + 1

$cGUI = GUICreate("GWCA MiniParty View", $width, $height, -1, -1, BitOR($WS_CAPTION, $WS_SYSMENU))
$hGraphic = _GDIPlus_GraphicsCreateFromHWND($cGUI)
$hBitmap = _GDIPlus_BitmapCreateFromGraphics($width, $height, $hGraphic)
$hBackbuffer = _GDIPlus_ImageGetGraphicsContext($hBitmap)

$hBrushRed = _GDIPlus_BrushCreateSolid(0xFFCC0000)
$hBrushDarkRed = _GDIPlus_BrushCreateSolid(0xFF441414)
$hBrushWhite = _GDIPlus_BrushCreateSolid(0xFFFFFFFF)
$hBrushGreen = _GDIPlus_BrushCreateSolid(0xFF6A6A2E)
$hBrushLightRed = _GDIPlus_BrushCreateSolid(0xFFE67E6D)
$hBrushPurple = _GDIPlus_BrushCreateSolid(0xFFCA18BC)
$hBrushGray = _GDIPlus_BrushCreateSolid(0xFF999999)
$hBrushLightGreen = _GDIPlus_BrushCreateSolid(0xFFBAFF42)
$hBrushDarkGray = _GDIPlus_BrushCreateSolid(0xFF666666)
$hBrushLightPurple = _GDIPlus_BrushCreateSolid(0xFFE659DB)
$hBrushYellow = _GDIPlus_BrushCreateSolid(0xFFFFF27E)
$hBrushBlack = _GDIPlus_BrushCreateSolid(0xFF000000)
$hBrushGold = _GDIPlus_BrushCreateSolid(0xFFFFDD40)
$hBrushVeryDarkGray = _GDIPlus_BrushCreateSolid(0xFF222222)

$hPenBlack = _GDIPlus_PenCreate(0xFF000000, 1, 2)
$hPenLightGray = _GDIPlus_PenCreate(0x90FFFFFF, 1, 2)
$hPenThickBlack = _GDIPlus_PenCreate(0xFF000000, 2, 2)
$hPenGold = _GDIPlus_PenCreate(0xFFFFDD40, 1, 2)
$hPenTransparentGold = _GDIPlus_PenCreate(0xB0fff266, 1, 2)

$hFormat = _GDIPlus_StringFormatCreate()
$hFamily = _GDIPlus_FontFamilyCreate("Tahoma")
$hFont = _GDIPlus_FontCreate($hFamily, 8, 0, 3)

_AntiAlias($hBackbuffer, 4)

_GDIPlus_GraphicsClear($hBackbuffer, 0xFF333333)

GUIRegisterMsg(0x4A, "PartyCallback") ;Notice that the MsgId (WM_COPYDATA) is the same as SkillLogCallback

GUISetOnEvent($GUI_EVENT_CLOSE, "EventHandler")
GUISetOnEvent($GUI_EVENT_PRIMARYDOWN, "EventHandler")

GUISetState(True, $cGUI)

WinSetOnTop($cGUI, "", True)

$cbType = "int"

Global $bGotInfo = False
Global $aTarget[2] = [0, 0]
Global $aHover[2] = [0, 0]
Global $tNameUpdate = 0

Cmd($CA_SetTeamSize, $teamSize)

Dim $aTeam1Prim[$teamSize], $aTeam2Prim[$teamSize], $aTeam3Prim[$teamSize]
Dim $aTeam1Seco[$teamSize], $aTeam2Seco[$teamSize], $aTeam3Seco[$teamSize]
Dim $aTeam1HP[$teamSize], $aTeam2HP[$teamSize], $aTeam3HP[$teamSize]
Dim $aTeam1Target[$teamSize], $aTeam2Target[$teamSize], $aTeam3Target[$teamSize]
Dim $aTeam1Skill[$teamSize], $aTeam2Skill[$teamSize], $aTeam3Skill[$teamSize]
Dim $aTeam1Id[$teamSize], $aTeam2Id[$teamSize], $aTeam3Id[$teamSize]
Dim $aTeam1Effects[$teamSize], $aTeam2Effects[$teamSize], $aTeam3Effects[$teamSize]
Dim $aTeam1Weapon[$teamSize], $aTeam2Weapon[$teamSize], $aTeam3Weapon[$teamSize]
Dim $aTeam1Name[$teamSize], $aTeam2Name[$teamSize], $aTeam3Name[$teamSize]
Dim $aTeam1Read[$teamSize], $aTeam2Read[$teamSize], $aTeam3Read[$teamSize]

While 1
	For $i = 1 To $numOfTeams Step 1
		GetPartyInfo($i)
	Next

	$aTarget = CmdCB($CA_GetCurrentTarget)

	$aPos = MouseGetPos()
	$iPl = Floor($aPos[1] / $hpHeight)
	Dim $aHover[2] = [0, 0]
	If _IsInRect($aPos[0], $aPos[1], 0, 0, $hpSize, $hpHeight * 8) AND $iPl >= 0 AND $iPl < $teamSize Then
		If $aTeam1Id[$iPl] <> 0 Then
			Dim $aHover[2] = [1, $iPl]
		EndIf
	ElseIf _IsInRect($aPos[0], $aPos[1], $hpSize + $gap, 0, $hpSize, $hpHeight * 8) AND $iPl >= 0 AND $iPl < $teamSize Then
		If $aTeam2Id[$iPl] <> 0 Then
			Dim $aHover[2] = [2, $iPl]
		EndIf
	ElseIf _IsInRect($aPos[0], $aPos[1], ($hpSize + $gap) * 2, 0, $hpSize, $hpHeight * 8) AND $iPl >= 0 AND $iPl < $teamSize Then
		If $aTeam3Id[$iPl] <> 0 Then
			Dim $aHover[2] = [3, $iPl]
		EndIf
	EndIf

	Draw()

	If TimerDiff($tNameUpdate) > 1000 Then
		$hProc = _WinAPI_OpenProcess(0x1F0FFF, False, WinGetProcess($sGW))
		If @error Then ContinueLoop

		$iRead = 0
		$tName = DllStructCreate("wchar[30]")

		For $i = 0 To $teamSize-1 Step 1
			If $aTeam1Name[$i] <> 0 Then
				_WinAPI_ReadProcessMemory($hProc, $aTeam1Name[$i], DllStructGetPtr($tName), DllStructGetSize($tName), $iRead)
				$aTeam1Read[$i] = DllStructGetData($tName, 1)
			Else
				$aTeam1Read[$i] = ""
			EndIf
			If $aTeam2Name[$i] <> 0 Then
				_WinAPI_ReadProcessMemory($hProc, $aTeam2Name[$i], DllStructGetPtr($tName), DllStructGetSize($tName), $iRead)
				$aTeam2Read[$i] = DllStructGetData($tName, 1)
			Else
				$aTeam2Read[$i] = ""
			EndIf
			If $aTeam3Name[$i] <> 0 Then
				_WinAPI_ReadProcessMemory($hProc, $aTeam3Name[$i], DllStructGetPtr($tName), DllStructGetSize($tName), $iRead)
				$aTeam3Read[$i] = DllStructGetData($tName, 1)
			Else
				$aTeam3Read[$i] = ""
			EndIf
		Next

		$tName = 0
		_WinAPI_CloseHandle($hProc)

		$tNameUpdate = TimerInit()
	EndIf
WEnd

Func Draw()
	Local $aSel[2] = [0, 0], $sText

	_GDIPlus_GraphicsClear($hBackbuffer, 0xFF333333)

	For $i = 0 To $teamSize-1 Step 1
		If $aTeam1Id[$i] = $aTarget[0] AND $aTeam1Id[$i] <> 0 Then
			$aSel[0] = 1
			$aSel[1] = $i
		ElseIf $aTeam2Id[$i] = $aTarget[0] AND $aTeam2Id[$i] <> 0 Then
			$aSel[0] = 2
			$aSel[1] = $i
		ElseIf $aTeam3Id[$i] = $aTarget[0] AND $aTeam3Id[$i] <> 0 Then
			$aSel[0] = 3
			$aSel[1] = $i
		EndIf

		If $numOfTeams >= 1 Then
			If $aTeam1Id[$i] = 0 Then
				$sText = "Empty"
			Else
				$sText = GetProf($aTeam1Prim[$i])&"/"&GetProf($aTeam1Seco[$i])&"20 "&$aTeam1Read[$i]
			EndIf
			DrawHealthBar($aTeam1Effects[$i], $aTeam1Id[$i], 0, $hpHeight * $i, $hpSize, $hpHeight, $sText, $aTeam1HP[$i])
		EndIf

		If $numOfTeams >= 2 Then
			If $aTeam2Id[$i] = 0 Then
				$sText = "Empty"
			Else
				$sText = GetProf($aTeam2Prim[$i])&"/"&GetProf($aTeam2Seco[$i])&"20 "&$aTeam2Read[$i]
			EndIf
			DrawHealthBar($aTeam2Effects[$i], $aTeam2Id[$i], $hpSize + $gap, $hpHeight * $i, $hpSize, $hpHeight, $sText, $aTeam2HP[$i])
		EndIf

		If $numOfTeams >= 3 Then
			If $aTeam3Id[$i] = 0 Then
				$sText = "Empty"
			Else
				$sText = GetProf($aTeam3Prim[$i])&"/"&GetProf($aTeam3Seco[$i])&"20 "&$aTeam3Read[$i]
			EndIf
			DrawHealthBar($aTeam3Effects[$i], $aTeam3Id[$i], ($hpSize + $gap) * 2, $hpHeight * $i, $hpSize, $hpHeight, $sText, $aTeam3HP[$i])
		EndIf
	Next

	_GDIPlus_GraphicsDrawRect($hBackbuffer, ($hpSize + $gap) * ($aHover[0] - 1), $hpHeight * $aHover[1], $hpSize, $hpHeight, $hPenTransparentGold)
	_GDIPlus_GraphicsDrawRect($hBackbuffer, ($hpSize + $gap) * ($aSel[0] - 1), $hpHeight * $aSel[1], $hpSize, $hpHeight, $hPenGold)

	_GDIPlus_GraphicsDrawImageRect($hGraphic, $hBitmap, 0, 0, $width, $height)
EndFunc

Func DrawHealthBar($Effects, $AgentId, $X, $Y, $Width, $Height, $Name, $HP)
	Local $hpBrush, $hpLength, $aSel[2] = [0, 0], $hpXOffset, $aPoly[4][2], $hBrushHPOverlay1, $hBrushHPOverlay2, $iEffects = 0

	If BitAND($Effects, 0x0040) Then ;Poison/Disease
		$hpBrush = $hBrushGreen
	ElseIf BitAND($Effects, 0x0001) Then ;Bleeding
		$hpBrush = $hBrushLightRed
	ElseIf BitAND($Effects, 0x0400) Then ;Hex
		$hpBrush = $hBrushPurple
	Else ;Normal
		$hpBrush = $hBrushRed
	EndIf

	If BitAND($Effects, 0x0020) Then ;Deep Wound
		$hpLength = $Width - ($Width / 5)
	Else
		$hpLength = $Width
	EndIf

	$hpXOffset = $X
	$polyBase = $X + $Width

	$hBrushHPOverlay1 = GDIPlus_CreateLineBrush(0, $Y, 0, $Y + $Height / 2, 0x00000000, 0x50FFFFFF)
	$hBrushHPOverlay2 = GDIPlus_CreateLineBrush(0, $Y, 0, $Y + $Height / 2, 0x00000000, 0x80000000)

	_GDIPlus_GraphicsFillRect($hBackbuffer, $hpXOffset, $Y, $Width, $hpHeight, $hBrushDarkRed)
	_GDIPlus_GraphicsFillRect($hBackbuffer, $hpXOffset, $Y, $hpLength * $HP, $hpHeight, $hpBrush)
	_GDIPlus_GraphicsDrawLine($hBackbuffer, $hpXOffset + $hpLength * $HP, $Y, $hpXOffset + $hpLength * $HP, $Y + $Height, $hPenLightGray)

	If BitAND($Effects, 0x0020) Then
		_GDIPlus_GraphicsFillRect($hBackbuffer, $hpXOffset + $hpLength, $Y, $Width / 5, $Height, $hBrushGray)
		_GDIPlus_GraphicsDrawLine($hBackbuffer, $hpXOffset + $hpLength, $Y, $hpXOffset + $hpLength, $Y + $Height, $hPenLightGray)
	EndIf

	If $AgentId = 0 Then
		_GDIPlus_GraphicsFillRect($hBackbuffer, $hpXOffset, $Y, $Width, $Height, $hBrushVeryDarkGray)
		_GDIPlus_GraphicsDrawRect($hBackbuffer, $hpXOffset, $Y, $Width, $Height, $hPenBlack)
	EndIf

	_GDIPlus_GraphicsFillRect($hBackbuffer, $hpXOffset, $Y, $hpLength * $HP, $Height / 2, $hBrushHPOverlay1)
	_GDIPlus_GraphicsFillRect($hBackbuffer, $hpXOffset, 1 + ($Height / 2) + $Y, $Width, $Height / 2, $hBrushHPOverlay2)

	_GDIPlus_GraphicsDrawRect($hBackbuffer, $hpXOffset, $Y, $Width, $Height, $hPenBlack)

	If BitAND($Effects, 0x0080) Then ;Enchantment
		$aPoly[0][0] = 3
		$aPoly[1][0] = $polyBase - 14
		$aPoly[1][1] = ($Height / 2) + 4 + $Y
		$aPoly[2][0] = $polyBase - 8
		$aPoly[2][1] = ($Height / 2) - 5 + $Y
		$aPoly[3][0] = $polyBase - 2
		$aPoly[3][1] = ($Height / 2) + 4 + $Y
		_GDIPlus_GraphicsFillPolygon($hBackbuffer, $aPoly, $hBrushLightGreen)
		_GDIPlus_GraphicsDrawPolygon($hBackbuffer, $aPoly, $hPenBlack)

		$polyBase -= 9
		$iEffects += 1
	EndIf

	If BitAND($Effects, 0x0002) Then ;Condition
		$aPoly[0][0] = 3
		$aPoly[1][0] = $polyBase - 14
		$aPoly[1][1] = ($Height / 2) - 5 + $Y
		$aPoly[2][0] = $polyBase - 8
		$aPoly[2][1] = ($Height / 2) + 4 + $Y
		$aPoly[3][0] = $polyBase - 2
		$aPoly[3][1] = ($Height / 2) - 5 + $Y
		_GDIPlus_GraphicsFillPolygon($hBackbuffer, $aPoly, $hBrushDarkGray)
		_GDIPlus_GraphicsDrawPolygon($hBackbuffer, $aPoly, $hPenBlack)

		$polyBase -= 12
		$iEffects += 1
	EndIf

	If BitAND($Effects, 0x0800) Then ;Hex
		$aPoly[0][0] = 3
		$aPoly[1][0] = $polyBase - 14
		$aPoly[1][1] = ($Height / 2) - 5 + $Y
		$aPoly[2][0] = $polyBase - 8
		$aPoly[2][1] = ($Height / 2) + 4 + $Y
		$aPoly[3][0] = $polyBase - 2
		$aPoly[3][1] = ($Height / 2) - 5 + $Y
		_GDIPlus_GraphicsFillPolygon($hBackbuffer, $aPoly, $hBrushLightPurple)
		_GDIPlus_GraphicsDrawPolygon($hBackbuffer, $aPoly, $hPenBlack)

		$polyBase -= 12
		$iEffects += 1
	EndIf

	If BitAND($Effects, 0x8000) Then ;Weapon spell
		_GDIPlus_GraphicsFillEllipse($hBackbuffer, $polyBase - 15, ($Height / 2) - 6 + $Y, 11, 11, $hBrushYellow)
		_GDIPlus_GraphicsDrawEllipse($hBackbuffer, $polyBase - 15, ($Height / 2) - 6 + $Y, 11, 11, $hPenBlack)
		_GDIPlus_GraphicsDrawLine($hBackbuffer, $polyBase - 5, ($Height / 2) + 4 + $Y, $polyBase - 14, ($Height / 2) - 5 + $Y, $hPenThickBlack)
		_GDIPlus_GraphicsDrawLine($hBackbuffer, $polyBase - 11, ($Height / 2) + 3 + $Y, $polyBase - 6, ($Height / 2) - 2 + $Y, $hPenThickBlack)

		$iEffects += 1
	EndIf

	If BitAND($Effects, 0x0010) OR $AgentId = 0 Then
		$textBrush = $hBrushGray
	ElseIf $AgentId = $aTarget[0] Then
		$textBrush = $hBrushGold
	Else
		$textBrush = $hBrushWhite
	EndIf

	$tLayout = _GDIPlus_RectFCreate($hpXOffset, $Height / 6 + $Y, $Width - ($iEffects * 12), $Height - ($Height - 15))
	$tShadow = _GDIPlus_RectFCreate($hpXOffset + 1, $Height / 6 + $Y + 1, $Width - ($iEffects * 12), $Height - ($Height - 15))

	$aMeasure = _GDIPlus_GraphicsMeasureString($hBackbuffer, $Name, $hFont, $tLayout, $hBrushBlack)
	If $aMeasure[1] < StringLen($Name) Then StringTrimRight($Name, (StringLen($Name) - $aMeasure[1]))

	_GDIPlus_GraphicsDrawStringEx($hBackbuffer, $Name, $hFont, $tShadow, $hFormat, $hBrushBlack)
	_GDIPlus_GraphicsDrawStringEx($hBackbuffer, $Name, $hFont, $tLayout, $hFormat, $textBrush)

	_GDIPlus_BrushDispose($hBrushHPOverlay1)
	_GDIPlus_BrushDispose($hBrushHPOverlay2)
EndFunc

Func PartyCallback($hwnd, $msg, $wparam, $lparam)
	$recvCDS = DllStructCreate("ULONG_PTR;DWORD;PTR", $lparam) ;COPYDATASTRUCT

	If DllStructGetData($recvCDS, 1) = 2 Then
		$partyInfo = DllStructCreate($tagPARTYINFO, DllStructGetData($recvCDS, 3)) ;See Struct definition in GWCAConstants.au3

		If DllStructGetData($partyInfo, "TeamId") = 1 Then
			For $i = 0 To $teamSize-1 Step 1
				$aTeam1Prim[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Primary")
				$aTeam1Seco[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Secondary")
				$aTeam1HP[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"HP")
				$aTeam1Target[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Target")
				$aTeam1Skill[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Skill")
				$aTeam1Id[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Id")
				$aTeam1Effects[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Effects")
				$aTeam1Weapon[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Weapon")
				$aTeam1Name[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Name")
			Next
		ElseIf DllStructGetData($partyInfo, "TeamId") = 2 Then
			For $i = 0 To $teamSize-1 Step 1
				$aTeam2Prim[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Primary")
				$aTeam2Seco[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Secondary")
				$aTeam2HP[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"HP")
				$aTeam2Target[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Target")
				$aTeam2Skill[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Skill")
				$aTeam2Id[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Id")
				$aTeam2Effects[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Effects")
				$aTeam2Weapon[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Weapon")
				$aTeam2Name[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Name")
			Next
		ElseIf DllStructGetData($partyInfo, "TeamId") = 3 Then
			For $i = 0 To $teamSize-1 Step 1
				$aTeam3Prim[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Primary")
				$aTeam3Seco[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Secondary")
				$aTeam3HP[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"HP")
				$aTeam3Target[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Target")
				$aTeam3Skill[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Skill")
				$aTeam3Id[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Id")
				$aTeam3Effects[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Effects")
				$aTeam3Weapon[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Weapon")
				$aTeam3Name[$i] = DllStructGetData($partyInfo, "Agent"&$i+1&"Name")
			Next
		EndIf

		$bGotInfo = True
	EndIf
EndFunc

Func GetPartyInfo($iParty)
	$bGotInfo = False ;Variable to check when the return comes

	If $iParty = 1 Then
		For $i = 0 To $teamSize-1 Step 1
			$aTeam1Prim[$i] = 0
			$aTeam1Seco[$i] = 0
			$aTeam1HP[$i] = 0
			$aTeam1Target[$i] = 0
			$aTeam1Skill[$i] = 0
			$aTeam1Id[$i] = 0
			$aTeam1Effects[$i] = 0
			$aTeam1Weapon[$i] = 0
			$aTeam1Name[$i] = 0
		Next
	ElseIf $iParty = 2 Then
		For $i = 0 To $teamSize-1 Step 1
			$aTeam2Prim[$i] = 0
			$aTeam2Seco[$i] = 0
			$aTeam2HP[$i] = 0
			$aTeam2Target[$i] = 0
			$aTeam2Skill[$i] = 0
			$aTeam2Id[$i] = 0
			$aTeam2Effects[$i] = 0
			$aTeam2Weapon[$i] = 0
			$aTeam2Name[$i] = 0
		Next
	ElseIf $iParty = 3 Then
		For $i = 0 To $teamSize-1 Step 1
			$aTeam3Prim[$i] = 0
			$aTeam3Seco[$i] = 0
			$aTeam3HP[$i] = 0
			$aTeam3Target[$i] = 0
			$aTeam3Skill[$i] = 0
			$aTeam3Id[$i] = 0
			$aTeam3Effects[$i] = 0
			$aTeam3Weapon[$i] = 0
			$aTeam3Name[$i] = 0
		Next
	EndIf

	$tDeadlock = TimerInit()
	Cmd($CA_GetPartyInfo, $iParty, $cGUI) ;Get party info
	Do
		Sleep(10)
	Until $bGotInfo = True OR TimerDiff($tDeadlock) > 1000

	Return $bGotInfo
EndFunc

Func GetTeam($iTeam)
	Switch $iTeam
		Case 1
			Return "Blue"
		Case 2
			Return "Red"
		Case 3
			Return "Yellow"
		Case Else
			Return ""
	EndSwitch
EndFunc

Func GetProf($iProf)
	Switch $iProf
		Case 1
			Return "W"
		Case 2
			Return "R"
		Case 3
			Return "Mo"
		Case 4
			Return "N"
		Case 5
			Return "Me"
		Case 6
			Return "E"
		Case 7
			Return "A"
		Case 8
			Return "Rt"
		Case 9
			Return "P"
		Case 10
			Return "D"
		Case Else
			Return ""
	EndSwitch
EndFunc

Func GetWeapon($iWeap)
	Switch $iWeap
	Case 0
		Return "Bundle"
	Case 1
		Return "Bow"
	Case 2
		Return "Axe"
	Case 3
		Return "Hammer"
	Case 4
		Return "Daggers"
	Case 5
		Return "Scythe"
	Case 6
		Return "Spear"
	Case 7
		Return "Sword"
	Case 8
		Return "Wand"
	Case 9
		Return "Wand"
	Case 10
		Return "Wand"
	Case 11
		Return "Wand"
	Case 12
		Return "Wand"
	Case 13
		Return "Staff"
	Case 14
		Return "Staff"
	Case 513
		Return "Bow"
	Case Else
		Return $iWeap
	EndSwitch
EndFunc

Func GetLowest($aHP)
	Local $iLowest = 0

	If Not IsArray($aHP) Then Return SetError(1, 0, 0)

	For $i = 0 To UBound($aHP)-1 Step 1
		If $aHP[$i] < $aHP[$iLowest] AND $aHP[$i] <> 0 Then $iLowest = $i
	Next

	Return $iLowest
EndFunc

Func _AntiAlias($hGraphics, $iMode)
	Local $aResult

	$aResult = DllCall($ghGDIPDll, "int", "GdipSetSmoothingMode", "hwnd", $hGraphics, "int", $iMode)
	If @error Then Return SetError(@error, @extended, False)
	Return SetError($aResult[0], 0, $aResult[0] = 0)
EndFunc

Func _IsInRect($pX, $pY, $iX, $iY, $iWidth, $iHeight)
	If $pX >= $iX AND $pX <= ($iX + $iWidth) AND $pY >= $iY AND $pY <= ($iY + $iHeight) Then Return True
	Return False
EndFunc

Func EventHandler()
	Switch @GUI_CtrlId
	Case $GUI_EVENT_CLOSE
		Exit

	Case $GUI_EVENT_PRIMARYDOWN
		$iPl = Floor(MouseGetPos(1) / $hpHeight)
		If _IsInRect(MouseGetPos(0), MouseGetPos(1), 0, 0, $hpSize, $hpHeight * 8) AND $iPl >= 0 AND $iPl < $teamSize Then
			If $aTeam1Id[$iPl] <> 0 Then
				Cmd($CA_ChangeTarget, $aTeam1Id[$iPl])
			EndIf
		ElseIf _IsInRect(MouseGetPos(0), MouseGetPos(1), $hpSize + $gap, 0, $hpSize, $hpHeight * 8) AND $iPl >= 0 AND $iPl < $teamSize Then
			If $aTeam2Id[$iPl] <> 0 Then
				Cmd($CA_ChangeTarget, $aTeam2Id[$iPl])
			EndIf
		ElseIf _IsInRect(MouseGetPos(0), MouseGetPos(1), ($hpSize + $gap) * 2, 0, $hpSize, $hpHeight * 8) AND $iPl >= 0 AND $iPl < $teamSize Then
			If $aTeam3Id[$iPl] <> 0 Then
				Cmd($CA_ChangeTarget, $aTeam3Id[$iPl])
			EndIf
		EndIf
	EndSwitch
EndFunc

Func GDIPlus_CreateLineBrush($iPoint1X, $iPoint1Y, $iPoint2X, $iPoint2Y, $iArgb1 = 0xFF0000FF, _
        $iArgb2 = 0xFFFF0000, $WrapMode = 0)

    Local $tPoint1, $pPoint1, $tPoint2, $pPoint2, $aRet, $res

    $tPoint1 = DllStructCreate("float X;float Y")
    $pPoint1 = DllStructGetPtr($tPoint1)
    DllStructSetData($tPoint1, "X", $iPoint1X)
    DllStructSetData($tPoint1, "Y", $iPoint1Y)

    $tPoint2 = DllStructCreate("float X;float Y")
    $pPoint2 = DllStructGetPtr($tPoint2)
    DllStructSetData($tPoint2, "X", $iPoint2X)
    DllStructSetData($tPoint2, "Y", $iPoint2Y)

    $aRet = DllCall($ghGDIPDll, "int", "GdipCreateLineBrush", "ptr", $pPoint1, "ptr", $pPoint2, "int", $iArgb1, _
            "int", $iArgb2, "int", $WrapMode, "int*", 0)

    Return $aRet[6]
EndFunc   ;==>GDIPlus_CreateLineBrush

