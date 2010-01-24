#include <GUIConstantsEx.au3>
#include <GDIPlus.au3>
#include <WinAPI.au3>
#include "GWCAConstants.au3"

Opt("GUIOnEventMode", True)
Opt("MouseCoordMode", 2)

_GDIPlus_Startup()

Global $teamSize = 8
Global $gap = 20, $hpHeight = 21, $width = 421, $height = $hpHeight * $teamSize + 1, $hpSize = ($width - $gap - 1) / 2;170

$cGUI = GUICreate("GWCA MiniParty View", $width, $height)
$hGraphic = _GDIPlus_GraphicsCreateFromHWND($cGUI)
$hBitmap = _GDIPlus_BitmapCreateFromGraphics($width, $height, $hGraphic)
$hBackbuffer = _GDIPlus_ImageGetGraphicsContext($hBitmap)

$hBrushRed = _GDIPlus_BrushCreateSolid(0xFFCC0000)
$hBrushDarkRed = _GDIPlus_BrushCreateSolid(0xFF440000)
$hBrushWhite = _GDIPlus_BrushCreateSolid(0xFFFFFFFF)
$hBrushGreen = _GDIPlus_BrushCreateSolid(0xFF6a6a2e)
$hBrushLightRed = _GDIPlus_BrushCreateSolid(0xFFE67E6D)
$hBrushPurple = _GDIPlus_BrushCreateSolid(0xFFca18bc)
$hBrushGray = _GDIPlus_BrushCreateSolid(0xFF999999)
$hBrushLightGreen = _GDIPlus_BrushCreateSolid(0xFFBAFF42)
$hBrushDarkGray = _GDIPlus_BrushCreateSolid(0xFF666666)
$hBrushLightPurple = _GDIPlus_BrushCreateSolid(0xFFE659DB)
$hBrushYellow = _GDIPlus_BrushCreateSolid(0xFFFFF27E)
$hBrushBlack = _GDIPlus_BrushCreateSolid(0xFF000000)
$hBrushGold = _GDIPlus_BrushCreateSolid(0xFFFFCB2E)
$hBrushVeryDarkGray = _GDIPlus_BrushCreateSolid(0xFF222222)

$hPenBlack = _GDIPlus_PenCreate(0xFF000000, 1, 2)
$hPenLightGray = _GDIPlus_PenCreate(0x80FFFFFF, 1, 2)
$hPenThickBlack = _GDIPlus_PenCreate(0xFF000000, 2, 2)
$hPenGold = _GDIPlus_PenCreate(0xFFFFCB2E, 1, 2)

$hFormat = _GDIPlus_StringFormatCreate()
$hFamily = _GDIPlus_FontFamilyCreate("Tahoma")
$hFont = _GDIPlus_FontCreate($hFamily, 8, 0, 3)

_AntiAlias($hBackbuffer, 4)

_GDIPlus_GraphicsClear($hBackbuffer, 0xFF333333)

GUIRegisterMsg(0x4A, "PartyCallback") ;Notice that the MsgId (WM_COPYDATA) is the same as SkillLogCallback
GUIRegisterMsg(0x500, "WndCallback")

GUISetOnEvent($GUI_EVENT_CLOSE, "EventHandler")
GUISetOnEvent($GUI_EVENT_PRIMARYDOWN, "EventHandler")

GUISetState(True, $cGUI)

WinSetOnTop($cGUI, "", True)

$cbType = "int"

Global $bGotInfo = False
Global $aTarget[2] = [0, 0]
Global $tNameUpdate = 0

Cmd($CA_SetTeamSize, $teamSize)

Dim $aTeam1Prim[$teamSize], $aTeam2Prim[$teamSize]
Dim $aTeam1Seco[$teamSize], $aTeam2Seco[$teamSize]
Dim $aTeam1HP[$teamSize], $aTeam2HP[$teamSize]
Dim $aTeam1Target[$teamSize], $aTeam2Target[$teamSize]
Dim $aTeam1Skill[$teamSize], $aTeam2Skill[$teamSize]
Dim $aTeam1Id[$teamSize], $aTeam2Id[$teamSize]
Dim $aTeam1Effects[$teamSize], $aTeam2Effects[$teamSize]
Dim $aTeam1Weapon[$teamSize], $aTeam2Weapon[$teamSize]
Dim $aTeam1Name[$teamSize], $aTeam2Name[$teamSize]
Dim $aTeam1Read[$teamSize], $aTeam2Read[$teamSize]

While 1
	$aTarget = CmdCB($CA_GetCurrentTarget)

	GetPartyInfo(1)
	GetPartyInfo(2)
	Draw(2)

	If TimerDiff($tNameUpdate) > 1000 Then
		$hProc = _WinAPI_OpenProcess(0x1F0FFF, False, WinGetProcess($sGW))
		If @error Then ContinueLoop

		$iRead = 0
		$tName = DllStructCreate("wchar[30]")
		For $i = 0 To UBound($aTeam1Name)-1 Step 1
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
		Next

		_WinAPI_CloseHandle($hProc)

		$tNameUpdate = TimerInit()
	EndIf
WEnd

Func Draw($players = $teamSize)
	Local $aSel[2] = [0, 0], $sText

	_GDIPlus_GraphicsClear($hBackbuffer, 0xFF333333)

	For $i = 0 To $teamSize-1 Step 1
		If $aTeam1Id[$i] = $aTarget[0] AND $aTeam1Id[$i] <> 0 Then
			$aSel[0] = 1
			$aSel[1] = $i
		ElseIf $aTeam2Id[$i] = $aTarget[0] AND $aTeam2Id[$i] <> 0 Then
			$aSel[0] = 2
			$aSel[1] = $i
		EndIf

		If $aTeam1Id[$i] = 0 Then
			$sText = "Empty"
		Else
			$sText = GetProf($aTeam1Prim[$i])&"/"&GetProf($aTeam1Seco[$i])&"20 "&$aTeam1Read[$i]
		EndIf
		DrawHealthBar($aTeam1Effects[$i], $aTeam1Id[$i], 0, $hpHeight * $i, $hpSize, $hpHeight, $sText, $aTeam1HP[$i])

		If $aTeam2Id[$i] = 0 Then
			$sText = "Empty"
		Else
			$sText = GetProf($aTeam2Prim[$i])&"/"&GetProf($aTeam2Seco[$i])&"20 "&$aTeam2Read[$i]
		EndIf
		DrawHealthBar($aTeam2Effects[$i], $aTeam2Id[$i], $hpSize + $gap, $hpHeight * $i, $hpSize, $hpHeight, $sText, $aTeam2HP[$i])
	Next

	If $aSel[0] = 1 Then
		_GDIPlus_GraphicsDrawRect($hBackbuffer, 0, $hpHeight * $aSel[1], $hpSize, $hpHeight, $hPenGold)
	ElseIf $aSel[0] = 2 Then
		_GDIPlus_GraphicsDrawRect($hBackbuffer, $hpSize + $gap, $hpHeight * $aSel[1], $hpSize, $hpHeight, $hPenGold)
	EndIf

	_GDIPlus_GraphicsDrawImageRect($hGraphic, $hBitmap, 0, 0, $width, $height)
EndFunc

Func DrawHealthBar($Effects, $AgentId, $X, $Y, $Width, $Height, $Name, $HP)
	Local $hpBrush, $hpLength, $aSel[2] = [0, 0], $hpXOffset, $aPoly[4][2], $hBrushHPOverlay1, $hBrushHPOverlay2

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

	$hBrushHPOverlay1 = GDIPlus_CreateLineBrush(0, 0, 0, $Height / 2, 0x00000000, 0x40FFFFFF)
	$hBrushHPOverlay2 = GDIPlus_CreateLineBrush(0, 0, 0, $Height / 2, 0x00000000, 0x80000000)

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

	$tLayout = _GDIPlus_RectFCreate($hpXOffset, $Height / 6 + $Y)
	$tShadow = _GDIPlus_RectFCreate($hpXOffset + 1, $Height / 6 + $Y + 1)

	If BitAND($Effects, 0x0080) Then ;Enchantment
		$aPoly[0][0] = 3
		$aPoly[1][0] = $polyBase - 14
		$aPoly[1][1] = 14 + ($Y)
		$aPoly[2][0] = $polyBase - 8
		$aPoly[2][1] = 5 + ($Y)
		$aPoly[3][0] = $polyBase - 2
		$aPoly[3][1] = 14 + ($Y)
		_GDIPlus_GraphicsFillPolygon($hBackbuffer, $aPoly, $hBrushLightGreen)
		_GDIPlus_GraphicsDrawPolygon($hBackbuffer, $aPoly, $hPenBlack)

		$polyBase -= 9
	EndIf

	If BitAND($Effects, 0x0002) Then ;Condition
		$aPoly[0][0] = 3
		$aPoly[1][0] = $polyBase - 14
		$aPoly[1][1] = 5 + ($Y)
		$aPoly[2][0] = $polyBase - 8
		$aPoly[2][1] = 14 + ($Y)
		$aPoly[3][0] = $polyBase - 2
		$aPoly[3][1] = 5 + ($Y)
		_GDIPlus_GraphicsFillPolygon($hBackbuffer, $aPoly, $hBrushDarkGray)
		_GDIPlus_GraphicsDrawPolygon($hBackbuffer, $aPoly, $hPenBlack)

		$polyBase -= 12
	EndIf

	If BitAND($Effects, 0x0800) Then ;Hex
		$aPoly[0][0] = 3
		$aPoly[1][0] = $polyBase - 14
		$aPoly[1][1] = 5 + ($Y)
		$aPoly[2][0] = $polyBase - 8
		$aPoly[2][1] = 14 + ($Y)
		$aPoly[3][0] = $polyBase - 2
		$aPoly[3][1] = 5 + ($Y)
		_GDIPlus_GraphicsFillPolygon($hBackbuffer, $aPoly, $hBrushLightPurple)
		_GDIPlus_GraphicsDrawPolygon($hBackbuffer, $aPoly, $hPenBlack)

		$polyBase -= 12
	EndIf

	If BitAND($Effects, 0x8000) Then ;Weapon spell
		_GDIPlus_GraphicsFillEllipse($hBackbuffer, $polyBase - 15, 4 + $Y, 11, 11, $hBrushYellow)
		_GDIPlus_GraphicsDrawEllipse($hBackbuffer, $polyBase - 15, 4 + $Y, 11, 11, $hPenBlack)
		_GDIPlus_GraphicsDrawLine($hBackbuffer, $polyBase - 5, 14 + $Y, $polyBase - 14, 5 + $Y, $hPenThickBlack)
		_GDIPlus_GraphicsDrawLine($hBackbuffer, $polyBase - 11, 13 + $Y, $polyBase - 6, 8 + $Y, $hPenThickBlack)
	EndIf

	If BitAND($Effects, 0x0010) OR $AgentId = 0 Then
		$textBrush = $hBrushGray
	ElseIf $AgentId = $aTarget[0] Then
		$textBrush = $hBrushGold
	Else
		$textBrush = $hBrushWhite
	EndIf

	_GDIPlus_GraphicsDrawStringEx($hBackbuffer, $Name, $hFont, $tShadow, $hFormat, $hBrushBlack)
	_GDIPlus_GraphicsDrawStringEx($hBackbuffer, $Name, $hFont, $tLayout, $hFormat, $textBrush)

	_GDIPlus_BrushDispose($hBrushHPOverlay1)
	_GDIPlus_BrushDispose($hBrushHPOverlay2)
EndFunc

Func PartyCallback($hwnd, $msg, $wparam, $lparam)
	$recvCDS = DllStructCreate("ptr;dword;ptr", $lparam) ;COPYDATASTRUCT

	If DllStructGetData($recvCDS, 1) = 2 Then
		$partyInfo = DllStructCreate($tagPARTYINFO, DllStructGetData($recvCDS, 3)) ;See Struct definition in GWCAConstants.au3

		If DllStructGetData($partyInfo, "TeamId") = 1 Then
			For $i = 0 To DllStructGetData($partyInfo, "TeamSize")-1 Step 1
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
			For $i = 0 To DllStructGetData($partyInfo, "TeamSize")-1 Step 1
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
		EndIf

		$bGotInfo = True
	EndIf
EndFunc

Func GetPartyInfo($iParty)
	$bGotInfo = False ;Variable to check when the return comes
	$tDeadlock = TimerInit()
	CmdCB($CA_GetPartyInfo, $iParty) ;Get party info
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

Func OnAutoItExit()
	_GDIPlus_GraphicsDispose($hGraphic)
	_GDIPlus_Shutdown()
EndFunc

Func EventHandler()
	Switch @GUI_CtrlId
	Case $GUI_EVENT_CLOSE
		Exit

	Case $GUI_EVENT_PRIMARYDOWN
		$iPl = Floor(MouseGetPos(1) / $hpHeight)
		If _IsInRect(MouseGetPos(0), MouseGetPos(1), 0, 0, $hpSize, $hpHeight * 8) AND $iPl >= 0 AND $iPl <= 7 Then
			If $aTeam1Id[$iPl] <> 0 Then
				CmdCB($CA_GetAgentExist, $aTeam1Id[$iPl])
				If $cbVar[0] = 1 Then Cmd($CA_ChangeTarget, $aTeam1Id[$iPl])
			EndIf
		ElseIf _IsInRect(MouseGetPos(0), MouseGetPos(1), $hpSize + $gap, 0, $hpSize, $hpHeight * 8) AND $iPl >= 0 AND $iPl <= 7 Then
			If $aTeam2Id[$iPl] <> 0 Then
				CmdCB($CA_GetAgentExist, $aTeam2Id[$iPl])
				If $cbVar[0] = 1 Then Cmd($CA_ChangeTarget, $aTeam2Id[$iPl])
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

