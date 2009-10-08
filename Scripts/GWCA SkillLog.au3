;##-----------------------##
; GWCA Mini Bot For Testing
; Work started on Dec 26th
;##-----------------------##
#include <GUIConstantsEx.au3>
#include <EditConstants.au3>
#include <WindowsConstants.au3>
#include "GWCAConstants.au3"

Global $cGUI = GUICreate("GWCA - Skill log", 320, 220) ;GUI for communication
Global $bLog = True

GUIRegisterMsg(0x500, "WndCallback")
GUIRegisterMsg(0x4A, "SkillLogCallback")
GUIRegisterMsg(0x501, "SkillCancelCallback")

$editLog = GUICtrlCreateEdit("", 0, 0, 320, 190, BitOR($ES_MULTILINE, $ES_READONLY, $WS_VSCROLL))
$btnLogOnOff = GUICtrlCreateButton("Disable log", 5, 195, 60, 20)
$btnClear = GUICtrlCreateButton("Clear log", 70, 195, 60, 20)

GUISetBkColor(0xAAAAAA)
GUISetState(True, $cGUI)

;## - BOT CODE - ##
Cmd($CA_SETLOGANDHWND, 1, $cGUI)

$spellRange = 1012*1.2

While 1
	$guiMsg = GUIGetMsg()
	Switch($guiMsg)
		Case $GUI_EVENT_CLOSE
			Exit

		Case $btnLogOnOff
			$bLog = Not $bLog
			If $bLog Then
				Cmd($CA_SETLOGANDHWND, 1, $cGUI)
				_LogAdd("Enabled log")
				GUICtrlSetData($btnLogOnoff, "Disable log")
			Else
				Cmd($CA_SETLOGANDHWND, 0, 0)
				_LogAdd("Disabled log")
				GUICtrlSetData($btnLogOnoff, "Enable log")
			EndIf

		Case $btnClear
			GUICtrlSetData($editLog, "")

	EndSwitch
WEnd

;## - END OF BOT CODE - ##

Func SkillLogCallback($hwnd, $msg, $wparam, $lparam)
	If $bLog Then
		$recvCDS = DllStructCreate("ptr;dword;ptr", $LParam)

		$skillStruct = DllStructCreate($tagSKILLLOGSTRUCT, DllStructGetData($recvCDS, 3))

		_LogAdd("Id "&DllStructGetData($skillStruct, "AgentId")&" team "&DllStructGetData($skillStruct, "TeamId")& _
				" used "&ReadSkillName(@ScriptDir&"\new list.txt", DllStructGetData($skillStruct, "SkillId"))& _
				" ("&Round(DllStructGetData($skillStruct, "Activation"), 2)&")")
	EndIf
EndFunc

Func SkillCancelCallback($hwnd, $msg, $wparam, $lparam)
	If $bLog Then
		_LogAdd("Id "&$wparam&" cancelled "&ReadSkillName(@ScriptDir&"\new list.txt", $lparam))
	EndIf
EndFunc

Func _LogAdd($msg)
	GUICtrlSetData($editLog, @HOUR&":"&@MIN&":"&@SEC&" - "&$msg&@CRLF&GUICtrlRead($editLog))
EndFunc

Func ReadSkillName($fSkillFile, $SkillId)
	$SkillId = Number($SkillId)
	If FileExists($fSkillFile) Then
		$fRead = FileRead($fSkillFile)
	Else
		$fRead = $fSkillFile
	EndIf
	$fPos = StringInStr($fRead, $SkillId & "|")
	If Not $fPos Then Return $SkillId

	$iNameStartPos = $fPos + StringLen($SkillId) + 1
	$nextLine = StringInStr($fRead, @CRLF, 0, 1, $iNameStartPos)
	$count =  $nextLine - $iNameStartPos
	$sName = StringMid($fRead, $iNameStartPos, $count)
	Return $sName
EndFunc
