; # -+-+-+-+-+-+-+ #
;   GWCA Constants
;	  By Sune C
; # +-+-+-+-+-+-+- #

#include-once

; The SkillLog structure for use with the Skill Log feature
Global $tagSKILLLOGSTRUCT = "long AgentId;long MyId;long SkillId;float Activation;byte TeamId;ushort Allegiance;float Distance;long Ping"

; The constants
Global Enum $CA_GETCURRENTTARGET = 0x401, $CA_GETMYID, $CA_CASTING, $CA_SKILLRECHARGE, $CA_SKILLADRENALINE, _
			$CA_STOREVAR, $CA_SETLOGANDHWND, $CA_GETAGENTANDTARGETPTR, $CA_GETSKILLBARSKILLID, $CA_GETMYMAXHP, $CA_GETMYMAXENERGY, $CA_GETBUILDNUMBER, _
			$CA_CHANGEMAXZOOM, _
			$CA_ATTACK = 0x410, $CA_MOVE, $CA_USESKILL, _
			$CA_CHANGEWEAPONSET, $CA_ZONEMAP, $CA_DROPGOLD, $CA_GONPC, $CA_GOPLAYER, $CA_GOSIGNPOST, $CA_USEATTACKSKILL, _
			$CA_ENTERCHALLENGE, $CA_OPENCHEST, $CA_SETEVENTSKILLMODE, $CA_USESKILLBARSKILL, $CA_PICKUPITEM, _
			$CA_USESKILLBARATTACKSKILL, $CA_DIALOG, $CA_CHANGETARGET, $CA_STATUSBOT, $CA_TARGETNEARESTFOE, $CA_TARGETNEARESTALLY, _
			$CA_TARGETNEARESTITEM, $CA_STATUSDELAY, $CA_TARGETCALLEDTARGET, $CA_USEHERO1SKILL, $CA_USEHERO2SKILL, $CA_USEHERO3SKILL, $CA_STATUSMISS, _
			$CA_CANCELACTION, $CA_STATUSTAB, $CA_GETNAMEPTR, _
			$CA_GETMAPLOADING = 0x440, $CA_GETMAPID, $CA_GETPING, $CA_GETLOGGEDIN, $CA_GETDEAD, $CA_GETBALTHFACTION, _
			$CA_GETAGENTEXIST = 0x450, $CA_GETPROFESSIONS, $CA_GETPLAYERNUMBER, _
			$CA_GETHP, $CA_GETROTATION, _
			$CA_GETSKILL, $CA_GETCOORDS, $CA_GETWEAPONSPEEDS, $CA_GETSPIRITRANGE, $CA_GETTEAMID, $CA_GETCOMBATMODE, _
			$CA_GETMODELMODE, $CA_GETHPPIPS, $CA_GETEFFECTS, $CA_GETHEX, $CA_GETMODELANIMATION, $CA_GETENERGY, $CA_GETAGENTPTR, _
			$CA_GETTYPE, $CA_GETLEVEL, $CA_GETNAMEPROPERTIES, $CA_GETMAXID, $CA_GETMYNEARESTAGENT, _
			$CA_GETMYDISTANCETOAGENT, $CA_GETNEARESTAGENTTOAGENT, $CA_GETDISTANCEFROMAGENTTOAGENT, $CA_GETNEARESTAGENTTOAGENTEX, _
			$CA_GETMODELSTATE, $CA_GETISATTACKING, $CA_GETISKNOCKEDDOWN, $CA_GETISMOVING, $CA_GETISDEAD, $CA_GETISCASTING, _
			$CA_GETFIRSTAGENTBYPLAYERNUMBER, $CA_GETALLEGIANCE, $CA_GETNEARESTENEMYTOAGENTEX, $CA_GETISATTACKEDMELEE, $CA_GETNEARESTITEMTOAGENTEX, _
			$CA_GETNEARESTAGENTBYPLAYERNUMBER, $CA_GETSPEED, $CA_GETNEARESTENEMYTOAGENTBYALLEGIANCE, $CA_GETNEARESTALIVEENEMYTOAGENT, _
			$CA_GETWEAPONTYPE, $CA_GETNEARESTSIGNPOSTTOAGENT, $CA_GETNEARESTNPCTOAGENTBYALLEGIANCE, $CA_GETNEARESTAGENTTOCOORDS, $CA_GETVARS, _
			$CA_GETNEARESTNPCTOCOORDS


; VARIABLES

Opt("WinTitleMatchMode", -1) ;Make sure that Guild Wars receives the messages (1 = match start, 2 = substring, 3 = exact)

Global $cbType = "int" ;What to read the callback values as
Global $cbVar[2] ;Array for callback wParam and lParam
Global $sGW = "Guild Wars -" ;Name of window
Global $cGUI = 0 ;Init GUI hwnd var

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
	$cbVar[0] = ""
	$cbVar[1] = ""
	DllCall("user32.dll", "lparam", "PostMessage", "hwnd", WinGetHandle($sGW), "int", $uMsg, "wparam", $wparam, "lparam", $lparam)
EndFunc

Func CmdCB($uMsg, $wparam = 0)
	$cbVar[0] = ""
	$cbVar[1] = ""
	DllCall("user32.dll", "lparam", "SendMessage", "hwnd", WinGetHandle($sGW), "int", $uMsg, "wparam", $wparam, "lparam", $cGUI)
	If $cbVar[0] = "" AND $cbVar[1] = "" Then SetError(-1)
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

; END OF FILE
