;Msg stuff!
#include <GUIConstantsEx.au3>
#include <EditConstants.au3>
#include <ComboConstants.au3>
#include <StaticConstants.au3>
#include "GWCAConstants.au3"

$myGUI = GUICreate("GWCA Commander", 250, 100)
$inputCmd = GUICtrlCreateCombo("", 5, 5, 240, 20, BitOr($GUI_SS_DEFAULT_COMBO, $CBS_SORT, $CBS_DROPDOWNLIST))
GUICtrlSetFont(-1, 7.5)
GUICtrlSetData(-1, "$CA_GETCURRENTTARGET|$CA_GETMYID|$CA_CASTING|$CA_SKILLRECHARGE|$CA_SKILLADRENALINE|$CA_STOREVAR|$CA_SETLOGANDHWND|"& _
					"$CA_GETAGENTANDTARGETPTR|$CA_ATTACK|$CA_MOVE|"& _
					"$CA_USESKILL|$CA_CHANGEWEAPONSET|$CA_ZONEMAP|$CA_DROPGOLD|$CA_GONPC|$CA_GOPLAYER|$CA_GOSIGNPOST|$CA_USEATTACKSKILL|"& _
					"$CA_ENTERCHALLENGE|$CA_OPENCHEST|$CA_SETEVENTSKILLMODE|$CA_USESKILLBARSKILL|$CA_PICKUPITEM|$CA_USESKILLBARATTACKSKILL|"& _
					"$CA_DIALOG|$CA_CHANGETARGET|$CA_STATUSBOT|$CA_TARGETNEARESTFOE|$CA_TARGETNEARESTALLY|$CA_TARGETNEARESTITEM|$CA_GETMAPLOADING|"& _
					"$CA_GETMAPID|$CA_GETPING|$CA_GETLOGGEDIN|$CA_GETDEAD|"& _
					"$CA_GETAGENTEXIST|$CA_GETPROFESSIONS|$CA_GETPLAYERNUMBER|$CA_GETHP|$CA_GETROTATION|$CA_GETSKILL|$CA_GETCOORDS|$CA_GETWEAPONSPEEDS|"& _
					"$CA_GETSPIRITRANGE|$CA_GETTEAMID|$CA_GETCOMBATMODE|$CA_GETMODELMODE|$CA_GETHPPIPS|$CA_GETEFFECTS|$CA_GETHEX|$CA_GETTOWN|"& _
					"$CA_GETENERGY|$CA_GETAGENTPTR|$CA_GETTYPE|$CA_GETLEVEL|$CA_GETNAMEPROPERTIES|$CA_GETMAXID|$CA_GETMYNEARESTAGENT|"& _
					"$CA_GETMYDISTANCETOAGENT|$CA_GETNEARESTAGENTTOAGENT|$CA_GETDISTANCEFROMAGENTTOAGENT|$CA_GETNEARESTAGENTTOAGENTEX|$CA_GETMODELSTATE|"& _
					"$CA_GETISATTACKING|$CA_GETISKNOCKEDDOWN|$CA_GETISMOVING|$CA_GETISDEAD|$CA_GETISCASTING|$CA_GETFIRSTAGENTBYPLAYERNUMBER|$CA_GETALLEGIANCE|"& _
					"$CA_GETNEARESTENEMYTOAGENTEX|$CA_GETISATTACKEDMELEE|$CA_STATUSDELAY|$CA_TARGETCALLEDTARGET|$CA_USEHERO1SKILL|$CA_USEHERO2SKILL|"& _
					"$CA_USEHERO3SKILL|$CA_GETMYMAXHP|$CA_GETMYMAXENERGY|$CA_STATUSMISS|$CA_GETSKILLBARSKILLID|$CA_CANCELACTION|$CA_GETNEARESTITEMTOAGENTEX|"& _
					"$CA_GETNEARESTAGENTBYPLAYERNUMBER|$CA_GETSPEED|$CA_GETBUILDNUMBER|$CA_GETNEARESTENEMYTOAGENTBYALLEGIANCE|$CA_GETWEAPONTYPE|"& _
					"$CA_GETNEARESTSIGNPOSTTOAGENT|$CA_CHANGEMAXZOOM|$CA_GETNEARESTNPCTOAGENTBYALLEGIANCE|$CA_GETBALTHFACTION|$CA_GETNAMEPTR|"& _
					"$CA_GETNEARESTAGENTTOCOORDS|$CA_GETVARS|$CA_GETNEARESTNPCTOCOORDS|$CA_GETKURZFACTION|$CA_GETLUXONFACTION|"& _
					"$CA_COMMANDHERO1|$CA_COMMANDHERO2|$CA_COMMANDHERO3|$CA_COMMANDALL|$CA_CHANGEDISTRICT|$CA_GETNEARESTALIVEENEMYTOAGENT|"& _
					"$CA_GETGOLD|$CA_GETBAGSIZE|$CA_GETBACKPACKITEMID|$CA_GETBELTPOUCHITEMID|$CA_GETBAG1ITEMID|"& _
					"$CA_GETBAG2ITEMID|$CA_GETEQUIPMENTPACKITEMID|$CA_GETIDKIT|$CA_IDENTIFYITEM|$CA_IDENTIFYITEMBYID|$CA_DEPOSITGOLD|$CA_WITHDRAWGOLD|"& _
					"$CA_SELLITEM|$CA_SELLITEMBYID|$CA_BUYIDKIT|$CA_BUYSUPERIORIDKIT|$CA_PREPAREMOVEITEM|$CA_MOVEITEM|"& _
					"$CA_GETTITLETREASURE|$CA_GETTITLELUCKY|$CA_GETTITLEUNLUCKY|$CA_GETTITLEWISDOM|$CA_GETBACKPACKITEMINFO|$CA_GETBELTPOUCHITEMINFO|"& _
					"$CA_GETBAG1ITEMINFO|$CA_GETBAG2ITEMINFO|$CA_GETEQUIPMENTPACKITEMINFO"& _
					"$CA_GETTITLESUNSPEAR|$CA_GETTITLELIGHTBRINGER|$CA_GETTITLEVANGUARD|$CA_GETTITLENORN|$CA_GETTITLEASURA|$CA_GETTITLEDELRIMOR|"& _
					"$CA_GETTITLENORTHMASTERY|$CA_GETTITLEDRUNKARD|$CA_GETTITLESWEET|$CA_GETTITLEPARTY|$CA_GETTITLECOMMANDER|$CA_GETTITLELUXON|"& _
					"$CA_GETTITLEKURZICK|$CA_RESIGN|$CA_RETURNTOOUTPOST|$CA_USEITEM|$CA_USEITEMBYID")
$inputWparam = GUICtrlCreateInput("", 5, 28, 150, 20)
$inputLparam = GUICtrlCreateInput("", 5, 51, 150, 20)
$btnSend = GUICtrlCreateButton("Send MSG", 4, 73)
GUICtrlSetFont(-1, 7.5)
$btnSendReceive = GUICtrlCreateButton("Send && Receive", 65, 73)
GUICtrlSetFont(-1, 7.5)
$inputType = GUICtrlCreateCombo("", 160, 75, 85, 20, BitOr($GUI_SS_DEFAULT_COMBO, $CBS_SORT, $CBS_DROPDOWNLIST))
GUICtrlSetFont(-1, 7.5)
GUICtrlSetData(-1, "hex|float|int", "hex")
$labelWparam = GUICtrlCreateLabel("", 160, 30, 85, 20)
GUICtrlSetColor(-1, 0xFFFFFF)
$labelLparam = GUICtrlCreateLabel("", 160, 53, 85, 20)
GUICtrlSetColor(-1, 0xFFFFFF)

GUISetBkColor(0x000000, $myGUI)

GUISetState(True, $myGUI)

GUIRegisterMsg(0x500, "HeheTest")

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
				$sendCmd = $CA_COMMANDALL Then
				$sendW = _FloatToInt($sendW)
				$sendL = _FloatToInt($sendL)
			EndIf

			CmdGW($sendCmd, $sendW, $sendL)

		Case $btnSendReceive
			$sendCmd = GUICtrlRead($inputCmd)
			If StringLeft($sendCmd, 2) = "0x" Then
				$sendCmd = BinaryToString($sendCmd)
			ElseIf StringLeft($sendCmd,1) = "$" Then
				$sendCmd = Eval(StringMid($sendCmd, 2))
			EndIf

			$sendW = Number(GUICtrlRead($inputWparam))

			GUICtrlSetData($labelWparam, "")
			GUICtrlSetData($labelLparam, "")

			CmdGW($sendCmd, $sendW, $myGUI)
	EndSwitch
WEnd

Func HeheTest($hwnd, $msg, $wparam, $lparam)
	Switch(GUICtrlRead($inputType))
		Case "float"
			$wparam = _IntToFloat($wparam)
			$lparam = _IntToFloat($lparam)
			ConsoleWrite("Float - 0x"&Hex($msg)&" - "&$wparam&" - "&$lparam&@LF)
		Case "int"
			$wparam = Number($wparam)
			$lparam = Number($lparam)
			ConsoleWrite("Int - 0x"&Hex($msg)&" - "&($wparam)&" - "&($lparam)&@LF)
		Case "hex"
			ConsoleWrite("Hex - 0x"&Hex($msg)&" - "&$wparam&" - "&$lparam&@LF)
		Case "degree"
			$wparam = _IntToFloat($wparam)
			$lparam = _IntToFloat($lparam)
			ConsoleWrite("Degrees - 0x"&Hex($msg)&" - "&$wparam&" - "&$lparam&@LF)
	EndSwitch
	GUICtrlSetData($labelWparam, $wparam)
	GUICtrlSetData($labelLparam, $lparam)
EndFunc

Func CmdGW($uMsg, $wparam, $lparam)
	DllCall("user32.dll", "lparam", "SendMessage", "hwnd", WinGetHandle($sGW), "int", $uMsg, "wparam", $wparam, "lparam", $lparam)
EndFunc
