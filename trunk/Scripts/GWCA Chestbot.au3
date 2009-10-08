; ----------------
; GWCA Chest Bot
; Started Feb 26th
; ----------------
#include <WindowsConstants.au3>
#include <GUIConstantsEx.au3>
#include "GWCAConstants.au3"

Global $cGUI = GUICreate("GWCA")
Global $runs = 0 ;Variable for number of runs completed
Global $chests = 0 ;Variable for number of chests opened
Global $maxChests = 35 ;Number of chests to run, -1 = infinity !!!!!!!!!!!!!

Global $SLOT_DWARVENSTABILITY = 1
Global $SLOT_DASH = 2

GUIRegisterMsg(0x500, "WndCallback")

; ---
; BOT
; ---
$cbType = "int"
CmdCB($CA_GETMAPID)
If $cbVar[0] <> 675 Then
	Debug("Not in Boreal Station! Exiting...")
	Exit
EndIf

While 1
	If $chests = $maxChests Then
		ConsoleWrite("> Reached maximum number of chests to open! Exiting..."&@CRLF)
		Exit
	EndIf

	Debug("Commencing new run...")
	$tTimer = TimerInit()
	$iRet = BotLoop()

	Cmd($CA_ZONEMAP, 675)
	Debug("Zoning back to Boreal Station, 675")
	Do
		Sleep(500)
		CmdCB($CA_GETMAPLOADING)
	Until $cbVar[0] = 0
	Debug("Loaded into outpost")
	Sleep(Random(1000,1300))

	$tTimer = TimerDiff($tTimer)
	Debug("Run completed!")

	$runs += 1
	If $iRet Then $chests += 1
	ConsoleWrite("+ CHESTBOT :: Last run time: "&Round($tTimer/1000,2)&"s, Runs = "&$runs&", Chests = "&$chests&@CRLF)

	Sleep(Random(1000,1300))
WEnd

; ---------
; FUNCTIONS
; ---------
Func BotLoop()
	Dim $wp[2] = [5492, -27888]
	Move($wp[0], $wp[1])

	$cbType = "float"
	Debug("Moving to spot before portal")
	CmdCB($CA_GETCOORDS, -2)
	Do
		Sleep(250)
		$oldCoords = $cbVar
		CmdCB($CA_GETCOORDS, -2)
		If $oldCoords[0] = $cbVar[0] AND $oldCoords[1] = $cbVar[1] Then
			Move($wp[0], $wp[1])
			Debug("Possibly stuck! Retrying...")
		EndIf
	Until ComputeDistance($cbVar[0], $cbVar[1], $wp[0], $wp[1]) < 300

	Switch(Random(1,5,1))
		Case 1
			Move(4733.52, -27842.97)
		Case 2
			Move(4730.40, -27788.09)
		Case 3
			Move(4700.77, -27976.07)
		Case 4
			Move(4720.88, -27892.77)
		Case 5
			Move(4720.72, -27858.23)
	EndSwitch

	$cbType = "int"
	Debug("Going through portal")
	Do
		Sleep(250)
		CmdCB($CA_GETMAPLOADING)
	Until $cbVar[0] = 1

	Debug("Loaded into instance")
	Sleep(Random(700,900))
	Cmd($CA_USESKILLBARSKILL, $SLOT_DWARVENSTABILITY)
	Sleep(Random(500,800))
	Cmd($CA_USESKILLBARSKILL, $SLOT_DASH)

	Switch(Random(1,3,1))
		Case 1
			Dim $wp[2] = [2942, -25733]
		Case 2
			Dim $wp[2] = [2884.05, -25826.86]
		Case 3
			Dim $wp[2] = [2876.13, -25690.62]
	EndSwitch
	Move($wp[0], $wp[1])

	Debug("Moving to spot outside cave")
	$cbType = "float"
	CmdCB($CA_GETCOORDS, -2)
	Do
		$oldCoords = $cbVar
		Sleep(250)
		$cbType = "int"
		CmdCBEx($CA_SKILLRECHARGE, $SLOT_DWARVENSTABILITY)
		If Not @error AND $cbVar[0] = 0 Then Cmd($CA_USESKILLBARSKILL, $SLOT_DWARVENSTABILITY)
		CmdCBEx($CA_SKILLRECHARGE, $SLOT_DASH)
		If Not @error AND $cbVar[0] = 0 Then Cmd($CA_USESKILLBARSKILL, $SLOT_DASH)
		$cbType = "float"
		CmdCB($CA_GETCOORDS, -2)
		If $oldCoords[0] = $cbVar[0] AND $oldCoords[1] = $cbVar[1] Then
			Move($wp[0], $wp[1])
			Debug("Possibly stuck! Retrying...")
		EndIf
	Until ComputeDistance($cbVar[0], $cbVar[1], $wp[0], $wp[1]) < 300

	Dim $wp[2] = [434, -20716]
	Move($wp[0], $wp[1])
	Debug("Moving to the big wide area")
	$cbType = "float"
	CmdCB($CA_GETCOORDS, -2)
	Do
		$oldCoords = $cbVar
		Sleep(250)
		$cbType = "int"
		CmdCBEx($CA_SKILLRECHARGE, $SLOT_DWARVENSTABILITY)
		If Not @error AND $cbVar[0] = 0 Then Cmd($CA_USESKILLBARSKILL, $SLOT_DWARVENSTABILITY)
		CmdCBEx($CA_SKILLRECHARGE, $SLOT_DASH)
		If Not @error AND $cbVar[0] = 0 Then Cmd($CA_USESKILLBARSKILL, $SLOT_DASH)
		$cbType = "float"
		CmdCB($CA_GETCOORDS, -2)
		If $oldCoords[0] = $cbVar[0] AND $oldCoords[1] = $cbVar[1] Then
			Move($wp[0], $wp[1])
			Debug("Possibly stuck! Retrying...")
		EndIf
	Until ComputeDistance($cbVar[0], $cbVar[1], $wp[0], $wp[1]) < 400

	Dim $wp[2] = [-3391, -18043]
	Move($wp[0], $wp[1])
	Debug("Moving to first chest spot")
	$cbType = "float"
	CmdCB($CA_GETCOORDS, -2)
	Do
		$oldCoords = $cbVar
		Sleep(250)
		$cbType = "int"
		CmdCBEx($CA_SKILLRECHARGE, $SLOT_DWARVENSTABILITY)
		If Not @error AND $cbVar[0] = 0 Then Cmd($CA_USESKILLBARSKILL, $SLOT_DWARVENSTABILITY)
		CmdCBEx($CA_SKILLRECHARGE, $SLOT_DASH)
		If Not @error AND $cbVar[0] = 0 Then Cmd($CA_USESKILLBARSKILL, $SLOT_DASH)
		$cbType = "float"
		CmdCB($CA_GETCOORDS, -2)
		If $oldCoords[0] = $cbVar[0] AND $oldCoords[1] = $cbVar[1] Then
			Move($wp[0], $wp[1])
			Debug("Possibly stuck! Retrying...")
		EndIf
	Until ComputeDistance($cbVar[0], $cbVar[1], $wp[0], $wp[1]) < 300

	Debug("Checking for chest")
	Cmd($CA_TARGETNEARESTITEM)
	Sleep(Random(500,600))
	$cbType = "int"
	CmdCB($CA_GETTYPE, -1)
	If $cbVar[0] = 512 Then
		Debug("Found chest")
		DoChest()
		Sleep(Random(800,1100))
		Return 1
	EndIf

	$cbType = "float"
	Dim $wp[2] = [-4950, -14890]
	Move($wp[0], $wp[1])
	Debug("Moving to second chest spot")
	$cbType = "float"
	CmdCB($CA_GETCOORDS, -2)
	Do
		$oldCoords = $cbVar
		Sleep(250)
		$cbType = "int"
		CmdCBEx($CA_SKILLRECHARGE, $SLOT_DWARVENSTABILITY)
		If Not @error AND $cbVar[0] = 0 Then Cmd($CA_USESKILLBARSKILL, $SLOT_DWARVENSTABILITY)
		CmdCBEx($CA_SKILLRECHARGE, $SLOT_DASH)
		If Not @error AND $cbVar[0] = 0 Then Cmd($CA_USESKILLBARSKILL, $SLOT_DASH)
		$cbType = "float"
		CmdCB($CA_GETCOORDS, -2)
		If $oldCoords[0] = $cbVar[0] AND $oldCoords[1] = $cbVar[1] Then
			Move($wp[0], $wp[1])
			Debug("Possibly stuck! Retrying...")
		EndIf
	Until ComputeDistance($cbVar[0], $cbVar[1], $wp[0], $wp[1]) < 200

	Debug("Checking for second chest")
	Cmd($CA_TARGETNEARESTITEM)
	Sleep(Random(500,600))
	$cbType = "int"
	CmdCB($CA_GETTYPE, -1)
	If $cbVar[0] = 512 Then
		Debug("Second chest found")
		Cmd($CA_USESKILLBARSKILL, 1)
		DoChest()
		Sleep(Random(800,1100))
		Return 1
	EndIf

	Debug("No chests found.")
	Sleep(Random(500,1000))
	Return 0
EndFunc

Func Move($x, $y)
	Cmd($CA_MOVE, _FloatToInt($x+Random(-50,50)), _FloatToInt($y+Random(-50,50)))
EndFunc

Func ComputeDistance($x1, $y1, $x2, $y2)
	Return Sqrt(($y2 - $y1)^2 + ($x2 - $x1)^2)
EndFunc

Func DoChest()
	Cmd($CA_GOSIGNPOST, -1)

	$cbType = "float"
	$chestCoords = CmdCB($CA_GETCOORDS, -1)
	CmdCB($CA_GETCOORDS, -2)
	Do
		$oldCoords = $cbVar
		Sleep(250)
		$cbType = "int"
		CmdCBEx($CA_SKILLRECHARGE, $SLOT_DWARVENSTABILITY)
		If Not @error AND $cbVar[0] = 0 Then Cmd($CA_USESKILLBARSKILL, $SLOT_DWARVENSTABILITY)
		CmdCBEx($CA_SKILLRECHARGE, $SLOT_DASH)
		If Not @error AND $cbVar[0] = 0 Then Cmd($CA_USESKILLBARSKILL, $SLOT_DASH)

		$cbType = "float"
		CmdCB($CA_GETCOORDS, -2)
		If $oldCoords[0] = $cbVar[0] AND $oldCoords[1] = $cbVar[1] Then
			Cmd($CA_GOSIGNPOST, -1)
			Debug("Possibly stuck! Retrying...")
		EndIf
	Until ComputeDistance($cbVar[0], $cbVar[1], $chestCoords[0], $chestCoords[1]) < 200

	Sleep(Random(700,1000))
	Cmd($CA_OPENCHEST)
	Sleep(Random(700,900))
	Cmd($CA_TARGETNEARESTITEM)
	Sleep(Random(400,500))
	$cbType = "int"
	$item = CmdCB($CA_GETCURRENTTARGET)
	Cmd($CA_PICKUPITEM, -1)
	$tDeadlock = TimerInit()
	Do
		Sleep(500)
		Cmd($CA_PICKUPITEM, $item[0])
		CmdCB($CA_GETAGENTEXIST, $item[0])
	Until $cbVar[0] = 0 OR TimerDiff($tDeadlock) > 10000
EndFunc

Func Debug($text)
	ConsoleWrite("> DEBUG: "&$text&@CRLF)
EndFunc
