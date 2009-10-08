; # -+-+-+-+-+-+-+-+-+-+ #
;   GWCA Backbreaker Bot
; # +-+-+-+-+-+-+-+-+-+- #
#include "GWCAConstants.au3"

;###########################
;#### GUI AND CONSTANTS ####
Global $cGUI = GUICreate("GWCA")

Global $iMinEnergyFullCombo = 26 ;Energy needed for the full combo
Global $iMinEnergyHalfCombo = 16 ;Energy needed for Spider + Blades
Global $tFullCombo = 0 ;Time since last full combo
Global $iAttackMode = 0 ;1 = spear, 2 = half combo, 3 = full combo
Global $aLastEnergy[2] = [0, 0] ;[0] = max, [1] = current

Global $SKILL_FLURRY = 1
Global $SKILL_BACKBREAKER = 2
Global $SKILL_FALLINGSPIDER = 3
Global $SKILL_TWISTINGFANGS = 4
Global $SKILL_FALLINGLOTUS = 5
Global $SKILL_BLADESOFSTEEL = 6
Global $SKILL_DASH = 7
Global $SKILL_RESURRECTIONSIGNET = 8

Global $WEAPON_ZEALOUSDAGGERS = 1
Global $WEAPON_VAMPIRICDAGGERS = 2
Global $WEAPON_HAMMER = 3
Global $WEAPON_SPEAR = 4

Global $ADRENALINE_BACKBREAKER = 240

GUIRegisterMsg(0x500, "WndCallback")

;##################
;#### BOT CODE ####

While 1
	;- Decide which attack mode to do based on Backbreaker's Adrenaline
	$cbType = "int"

	CmdCB($CA_SKILLADRENALINE, $SKILL_BACKBREAKER)
	If $cbVar[0] < $ADRENALINE_BACKBREAKER Then ;If not charged yet
		$iAttackMode = 1 ;Go spear mode
	Else
		$aLastEnergy = CmdCB($CA_GETMYMAXENERGY) ;Check energy
		CmdCB($CA_SKILLRECHARGE, $SKILL_TWISTINGFANGS) ;Check if Twisting is recharging
		If $cbVar[0] = 0 AND $aLastEnergy[1] >= $iMinEnergyFullCombo Then ;If it isn't and you have enough energy
			$iAttackMode = 3 ;Go full combo mode
		ElseIf $cbVar[0] <> 0 AND $aLastEnergy[1] >= $iMinEnergyHalfCombo Then ;If it is but you got energy for half combo
			CmdCB($CA_SKILLRECHARGE, $SKILL_FALLINGSPIDER) ;And attack skills are recharged :)
			If $cbVar[0] = 0 Then $iAttackMode = 2 ;Go half combo mode
		EndIf
	EndIf

	Switch($iAttackMode)
		Case 1
			$cbType = "int"

			CmdCB($CA_GETISATTACKING, -2)
			If Not $cbVar[0] Then
				ConsoleWrite("..."&@CRLF)
				Cmd($CA_CANCELACTION)
				Sleep(100)
				Cmd($CA_CHANGEWEAPONSET, $WEAPON_SPEAR)
				$aTarget = CmdCB($CA_GETNEARESTALIVEENEMYTOAGENT, -2) ;Find the nearest enemy who's alive
				If Not @error Then
					CmdCB($CA_GETISDEAD, $aTarget[0])
					If $cbVar[0] <> 1 Then Cmd($CA_ATTACK, $aTarget[0]) ;Start auto-attacking it
				EndIf
			EndIf

			$aLastEnergy = CmdCB($CA_GETMYMAXENERGY) ;Refresh energy
			If $aLastEnergy[1] >= $iMinEnergyFullCombo Then ;If you got plenty of energy
				CmdCB($CA_SKILLRECHARGE, $SKILL_FLURRY)
				If $cbVar[0] = 0 Then Cmd($CA_USESKILLBARSKILL, $SKILL_FLURRY) ;Use flurry
			EndIf

		Case 2
			$cbType = "int"
			;Swap to hammer
			Sleep(800) ;Make sure Blades is recharged
			Cmd($CA_CANCELACTION)
			Sleep(100)
			Cmd($CA_CHANGEWEAPONSET, $WEAPON_HAMMER)

			$aTarget = CmdCB($CA_GETNEARESTALIVEENEMYTOAGENT, -2)
			ConsoleWrite($aTarget[0]&@CRLF)
			If Not @error Then
				$aDead = CmdCB($CA_GETISDEAD, $aTarget[0])
				If $aDead[0] <> 1 Then
					Cmd($CA_USESKILLBARATTACKSKILL, $SKILL_BACKBREAKER, $aTarget[0])
					Do
						$aDead = CmdCB($CA_GETISDEAD, $aTarget[0])
						CmdCB($CA_SKILLADRENALINE, $SKILL_BACKBREAKER)
					Until $cbVar[0] <> $ADRENALINE_BACKBREAKER OR $aDead[0]
					Do
						$aDead = CmdCB($CA_GETISDEAD, $aTarget[0])
						CmdCB($CA_GETSKILL, -2)
					Until $cbVar[0] = 0 OR $aDead[0]
					Cmd($CA_USESKILLBARSKILL, $SKILL_FLURRY)
					Sleep(400)
					Cmd($CA_CANCELACTION)
					Sleep(100)
					Cmd($CA_CHANGEWEAPONSET, $WEAPON_VAMPIRICDAGGERS)
					Sleep(100)
					Cmd($CA_USESKILLBARATTACKSKILL, $SKILL_FALLINGSPIDER, $aTarget[0])
					Do
						$aDead = CmdCB($CA_GETISDEAD, $aTarget[0])
						CmdCB($CA_SKILLRECHARGE, $SKILL_FALLINGSPIDER)
					Until $cbVar[0] <> 0 OR $aDead[0]
					Cmd($CA_USESKILLBARATTACKSKILL, $SKILL_BLADESOFSTEEL, $aTarget[0])
					Do
						$aDead = CmdCB($CA_GETISDEAD, $aTarget[0])
						CmdCB($CA_SKILLRECHARGE, $SKILL_BLADESOFSTEEL)
					Until $cbVar[0] <> 0 OR $aDead[0]
				EndIf
			EndIf

			;Swap back to spear
			Sleep(200)
			Cmd($CA_CANCELACTION)
			Sleep(100)
			Cmd($CA_CHANGEWEAPONSET, $WEAPON_SPEAR)

		Case 3
			$cbType = "int"
			;Swap to hammer
			Cmd($CA_CANCELACTION)
			Sleep(100)
			Cmd($CA_CHANGEWEAPONSET, $WEAPON_HAMMER)

			$aTarget = CmdCB($CA_GETNEARESTALIVEENEMYTOAGENT, -2)
			ConsoleWrite($aTarget[0]&@CRLF)
			If Not @error Then
				$aDead = CmdCB($CA_GETISDEAD, $aTarget[0])
				If $aDead[0] <> 1 Then
					Cmd($CA_USESKILLBARATTACKSKILL, $SKILL_BACKBREAKER, $aTarget[0])
					Do
						$aDead = CmdCB($CA_GETISDEAD, $aTarget[0])
						CmdCB($CA_SKILLADRENALINE, $SKILL_BACKBREAKER)
					Until $cbVar[0] <> $ADRENALINE_BACKBREAKER OR $aDead[0]
					Do
						$aDead = CmdCB($CA_GETISDEAD, $aTarget[0])
						CmdCB($CA_GETSKILL, -2)
					Until $cbVar[0] = 0 OR $aDead[0]
					Cmd($CA_USESKILLBARSKILL, $SKILL_FLURRY)
					Sleep(400)
					Cmd($CA_CANCELACTION)
					Sleep(100)
					Cmd($CA_CHANGEWEAPONSET, $WEAPON_VAMPIRICDAGGERS)
					Sleep(100)
					Cmd($CA_USESKILLBARATTACKSKILL, $SKILL_FALLINGSPIDER, $aTarget[0])
					Do
						$aDead = CmdCB($CA_GETISDEAD, $aTarget[0])
						CmdCB($CA_SKILLRECHARGE, $SKILL_FALLINGSPIDER)
					Until $cbVar[0] <> 0 OR $aDead[0]
					Cmd($CA_USESKILLBARATTACKSKILL, $SKILL_TWISTINGFANGS, $aTarget[0])
					Do
						$aDead = CmdCB($CA_GETISDEAD, $aTarget[0])
						CmdCB($CA_SKILLRECHARGE, $SKILL_TWISTINGFANGS)
					Until $cbVar[0] <> 0 OR $aDead[0]
					Cmd($CA_USESKILLBARATTACKSKILL, $SKILL_FALLINGLOTUS, $aTarget[0])
					Do
						$aDead = CmdCB($CA_GETISDEAD, $aTarget[0])
						CmdCB($CA_SKILLRECHARGE, $SKILL_FALLINGLOTUS)
					Until $cbVar[0] <> 0 OR $aDead[0]
					Cmd($CA_USESKILLBARATTACKSKILL, $SKILL_BLADESOFSTEEL, $aTarget[0])
					Do
						$aDead = CmdCB($CA_GETISDEAD, $aTarget[0])
						CmdCB($CA_SKILLRECHARGE, $SKILL_BLADESOFSTEEL)
					Until $cbVar[0] <> 0 OR $aDead[0]
				EndIf
			EndIf

			;Swap back to spear
			Sleep(200)
			Cmd($CA_CANCELACTION)
			Sleep(100)
			Cmd($CA_CHANGEWEAPONSET, $WEAPON_SPEAR)

	EndSwitch

	Sleep(500)
WEnd
