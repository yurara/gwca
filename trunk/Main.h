#pragma once
#ifndef _MAIN_H
#define _MAIN_H
/*	Guild Wars Client API - GWCA
	This is a large set of functions that makes interfacing with Guild Wars a lot easier.
	Uses Window Messages for communicating by hooking the WndProc of GW, allowing you
	to SendMessage directly to the Guild Wars window.
	Protocol rules are, if your Message expects a reply, then you have to provide the
	window handle of your application in lParam. The reply is sent with code 0x500. */


#include "stdafx.h"
#include "Item.h"
#include "Functions.h"
#include "Agent.h"
#include "Skillbar.h"
#include "Packet.h"
#include "Hooks.h"
#include "LUA Main.h"

#include <Commdlg.h>

#define WND_MAIN_CLASS_NAME (LPCSTR)"Guildwars_Client_API"
#define WND_CHILD_CLASS_NAME (LPCSTR)"Compass"
// identifiers.
#define COMPASS  2000
#define TARGET   2001
#define CD       2002
#define SELF     2003
#define TARGETID 2004
#define YTARGET  2005
#define XTARGET  2006
#define ETARGET  2007
#define HPTARGET 2008
#define SKILL8   2009
#define SKILL7   2010
#define SKILL6   2011
#define SKILL5   2012
#define SKILL4   2013
#define SKILL3   2014
#define SKILL2   2015
#define SKILL1   2016
#define AREASELF 2017
#define YSELF    2018
#define XSELF    2019
#define ESELF    2020
#define HPSELF   2021
#define DISABLE  2022
#define Load     2023
#define START    2024
#define BROWSE   2025
#define SCRIPT   2026
#define CHECK    2027

void DrawCompass();
LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
HWND create_wnd();
void update_wnd();
void create_wnd_content(HWND parent);
void register_classes();
int message_loop();
int WindowMain();
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved);

#endif