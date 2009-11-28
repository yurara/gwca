#include "Main.h"
#include <gdiplus.h>

//#define DrawOnDisabled   //uncomment to update window only when graphic is disabled

using namespace Gdiplus;

bool disabled=false;
HINSTANCE instance;
HFONT h_font;
//Window
HWND MainWnd;
//Controls
HWND RunStop,LoadDll,Script,Engine,Browse;
HWND HPSelf,ESelf,XSelf,YSelf,AreaID;
HWND HPTarget,XTarget,YTarget,TargetID;
//HWND Spell1,Spell2,Spell3,Spell4,Spell5,Spell6,Spell7,Spell8;

//HWND* Spells[] = {&Spell1,&Spell2,&Spell3,&Spell4,&Spell5,&Spell6,&Spell7,&Spell8};
//GDI+
ULONG_PTR m_gdiplusToken;
HANDLE updateThread;

bool wopen = true;

void DrawCompass(){
	float distance;
	float angle;
	REAL x;
	REAL y;
	//graphics
	Pen      blue(Color(255, 0, 0, 255));
	Pen      green(Color(255, 0, 255, 0));
	Pen      red(Color(255, 255, 0, 0));
	Pen		 grey(Color(255,125,125,125));
	SolidBrush blackBrush(Color(255, 0, 0, 0));
	Graphics graphics(MainWnd);
	graphics.FillEllipse(&blackBrush,195, 330, 210, 210);
    //compass
    graphics.DrawEllipse(&blue, 195, 330, 210, 210);
	//self
	graphics.DrawRectangle(&green,300,435,2,2);
	//agents
	for(unsigned int i=1;i<=maxAgent;i++){
		distance=GetDistanceFromAgentToAgent(i,myId) / 50;
		if(distance>0){
			angle=-1 * GetAngleFromAgentToAgent(myId,i);
			x = 300 + distance * cos(angle);
			y = 435 + distance * sin(angle);
			if(/*Agents[i]->ModelState != 0x400 &&*/ Agents[i]->HP != 0){
			if(i==*GWMemory.CurrentTarget){
				graphics.DrawRectangle(&blue,x,y,(REAL)2,(REAL)2);
			}else if(Agents[i]->Allegiance == 0x300){
				graphics.DrawRectangle(&red,x,y,(REAL)2,(REAL)2);
			}else{
				graphics.DrawRectangle(&green,x,y,(REAL)2,(REAL)2);
			}
			}else{
				graphics.DrawRectangle(&grey,x,y,(REAL)2,(REAL)2);
			}
		}
	}
}
LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	OPENFILENAME ofn;
	char szFile[MAX_PATH] = ""; 
	switch (msg)
	{
	case WM_COMMAND:
		switch (LOWORD(wparam))
		{
		case Load:	// button "Load DLL"
			SendMessage(Script,WM_GETTEXT,260,LPARAM(&szFile));
			if(!LoadLibrary(szFile)){
				MessageBox(hwnd,"Not able to load Library","Error", MB_OK|MB_ICONERROR);
			}
			break;

		case DISABLE:	// button "Disable Graphics"
			if(disabled){
				char text[] = {"Disable Graphics"};
				disabled=false;
				SendMessage(Engine,WM_SETTEXT,0,(LPARAM)&text);
				SetEngineHook(0);
			}else{
				char text[] = {"Enable Graphics"};
				disabled=true;
				SendMessage(Engine,WM_SETTEXT,0,(LPARAM)&text);
				SetEngineHook(1);
			}
			break;

		case START:	// button "Start script"
			SendMessage(Script,WM_GETTEXT,260,LPARAM(&szFile));
			run_script(szFile);
			ZeroMemory(&szFile, sizeof(szFile));
			SendMessage(Script,WM_SETTEXT,0,(LPARAM)&szFile);
			break;

		case BROWSE:	// button "Browse ..." 
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFilter = "Library (*.dll)\0*.dll\0GWCA Script (*.gwca)\0*.gwca\0";
			ofn.nFilterIndex = 2;
			ofn.Flags = OFN_EXPLORER |OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if(GetOpenFileName(&ofn)){
				SendMessage(Script,WM_SETTEXT,0,(LPARAM)szFile);
			}
			break;
		}
		break;

	case WM_CREATE:
		create_wnd_content(hwnd);
		break;

	case WM_CLOSE:
		wopen=false;
		DestroyWindow(hwnd);
		CloseHandle(updateThread);
		GdiplusShutdown(m_gdiplusToken);
		close_lua();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);	// PostQuitMessage(return_code) quits the message loop.
		break;

	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return FALSE;
}

HWND create_wnd()
{
	MainWnd = CreateWindowEx(WS_EX_CONTROLPARENT,WND_MAIN_CLASS_NAME,"Guildwars Client API",WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 481, 597, NULL, NULL, instance,NULL);
	ShowWindow(MainWnd, SW_SHOWNORMAL);
	UpdateWindow(MainWnd);
	return MainWnd;
}
void update_wnd(){
	char text[32];
	float f;
	bool load;
	while(wopen){
		long* T = (long*)(GWMemory.CurrentTarget+0x410);
		while((!GWMemory.LoggedIn() || *T==2) && *T!=1){
			Sleep(100);
			load = true;
		}
		if(load){
			load = false;
			Sleep(5000);
		}
		#ifdef DrawOnDisabled
		if(disabled){
		#endif
		//Self Info
		if(*T!=2){
		memcpy(&f,&Agents[myId]->X,sizeof(float));
		sprintf(text,"X: %.2f",f);
		SendMessage(XSelf,WM_SETTEXT,0,(LPARAM)&text);
		}else{load = true;}

		if(*T!=2 && load==false){
		memcpy(&f,&Agents[myId]->Y,sizeof(float));
		sprintf(text,"Y: %.2f",f);
		SendMessage(YSelf,WM_SETTEXT,0,(LPARAM)&text);
		}else{load = true;}

		if(*T!=2 && load==false){
		memcpy(&f,&Agents[myId]->HP,sizeof(float));
		sprintf(text,"HP: %.0f %%",f * 100);
		SendMessage(HPSelf,WM_SETTEXT,0,(LPARAM)&text);
		}else{load = true;}

		if(*T!=2 && load==false){
		memcpy(&f,&Agents[myId]->Energy,sizeof(float));
		sprintf(text,"E: %.0f %%",f * 100);
		SendMessage(ESelf,WM_SETTEXT,0,(LPARAM)&text);
		}else{load = true;}

		if(*T!=2 && load==false){
		sprintf(text,"AreaID: %i",GWMemory.MapId());
		SendMessage(AreaID,WM_SETTEXT,0,(LPARAM)&text);
		}else{load = true;}
		//Target Info
		if(*(long*)GWMemory.CurrentTarget != 0){

		if(*T!=2 && load==false){
		memcpy(&f,&Agents[*(long*)GWMemory.CurrentTarget ]->X,sizeof(float));
		sprintf(text,"X: %.2f",f);
		SendMessage(XTarget,WM_SETTEXT,0,(LPARAM)&text);
		}else{load = true;}

		if(*T!=2 && load==false){
		memcpy(&f,&Agents[*(long*)GWMemory.CurrentTarget ]->Y,sizeof(float));
		sprintf(text,"Y: %.2f",f);
		SendMessage(YTarget,WM_SETTEXT,0,(LPARAM)&text);
		}else{load = true;}

		if(*T!=2 && load==false){
		memcpy(&f,&Agents[*(long*)GWMemory.CurrentTarget ]->HP,sizeof(float));
		sprintf(text,"HP: %.0f %%",f * 100);
		SendMessage(HPTarget,WM_SETTEXT,0,(LPARAM)&text);
		}else{load = true;}

		if(*T!=2 && load==false){
		sprintf(text,"ID: %i",*(long*)GWMemory.CurrentTarget );
		SendMessage(TargetID,WM_SETTEXT,0,(LPARAM)&text);
		}else{load = true;}
		}
		else{
		sprintf(text,"X:");
		SendMessage(XTarget,WM_SETTEXT,0,(LPARAM)&text);
		//sprintf(text,"Y:");
		//SendMessage(YTarget,WM_SETTEXT,0,(LPARAM)&text);
		sprintf(text,"HP:");
		SendMessage(HPTarget,WM_SETTEXT,0,(LPARAM)&text);
		sprintf(text,"ID:");
		SendMessage(TargetID,WM_SETTEXT,0,(LPARAM)&text);
		}
		//Recharging
		/*
		if(load==false && *T==1){
			ReloadSkillbar();
			if(!MySkillbar==NULL){
				for(int i=0;i<8;i++){
					if(GWMemory.LoggedIn() && *T!=2 && load==false){
					if(MySkillbar->Skill[i].Recharge != 0){
						sprintf(text,"Spell %i: charging",i + 1);
					}else{
						sprintf(text,"Spell %i: charged",i + 1);
					}
					SendMessage(*Spells[i],WM_SETTEXT,0,(LPARAM)&text);
					}else{
						break;
						load = true;
					}
				}
			}
		}else{load = true;}*/
		//Compass
		DrawCompass();
		Sleep(100);
		#ifdef DrawOnDisabled
		}
		#endif
	}
}
void create_wnd_content(HWND parent)
{
	HWND wnd;
	//Groups:
	wnd = CreateWindowEx(0x00000000,"Button","Info Self:", 0x50020007, 24, 136, 144, 152, parent, (HMENU) SELF, instance, NULL);
	SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
	wnd = CreateWindowEx(0x00000000, "Button", "Compass:", 0x50020007, 160, 304, 288, 248, parent, (HMENU) COMPASS, instance, NULL);
	SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
	wnd = CreateWindowEx(0x00000000, "Button", "Target Info:", 0x50020007, 304, 136, 144, 152, parent, (HMENU) TARGET, instance, NULL);
	SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);
	/*wnd = CreateWindowEx(0x00000000, "Button", "Cooldown:", 0x50020007, 24, 328, 112, 224, parent, (HMENU) CD, instance, NULL);
	SendMessage(wnd, WM_SETFONT, (WPARAM) h_font, TRUE);*/
	//Target Info:
	HPTarget = CreateWindowEx(0x00000000, "Static", "HP:", 0x50000300, 320, 160, 120, 24, parent, (HMENU) HPTARGET, instance, NULL);
	SendMessage(HPTarget, WM_SETFONT, (WPARAM) h_font, TRUE);
	TargetID = CreateWindowEx(0x00000000, "Static", "ID:", 0x50000300, 320, 256, 120, 24, parent, (HMENU) TARGETID, instance, NULL);
	SendMessage(TargetID, WM_SETFONT, (WPARAM) h_font, TRUE);
	YTarget = CreateWindowEx(0x00000000, "Static", "Y:", 0x50000300, 320, 232, 120, 24, parent, (HMENU) YTARGET, instance, NULL);
	SendMessage(YTarget, WM_SETFONT, (WPARAM) h_font, TRUE);
	XTarget = CreateWindowEx(0x00000000, "Static", "X:", 0x50000300, 320, 208, 120, 24, parent, (HMENU) XTARGET, instance, NULL);
	SendMessage(XTarget, WM_SETFONT, (WPARAM) h_font, TRUE);
	/*ETarget = CreateWindowEx(0x00000000, "Static", "E:", 0x50000300, 320, 184, 120, 24, parent, (HMENU) ETARGET, instance, NULL);
	SendMessage(ETarget, WM_SETFONT, (WPARAM) h_font, TRUE);*/
	//Cooldowns
	/*Spell8 = CreateWindowEx(0x00000000, "Static", "Spell 8:", 0x50000300, 40, 520, 88, 24, parent, (HMENU) SKILL8, instance, NULL);
	SendMessage(Spell8, WM_SETFONT, (WPARAM) h_font, TRUE);
	Spell7 = CreateWindowEx(0x00000000, "Static", "Spell 7:", 0x50000300, 40, 496, 88, 24, parent, (HMENU) SKILL7, instance, NULL);
	SendMessage(Spell7, WM_SETFONT, (WPARAM) h_font, TRUE);
	Spell6 = CreateWindowEx(0x00000000, "Static", "Spell 6:", 0x50000300, 40, 472, 88, 24, parent, (HMENU) SKILL6, instance, NULL);
	SendMessage(Spell6, WM_SETFONT, (WPARAM) h_font, TRUE);
	Spell5 = CreateWindowEx(0x00000000, "Static", "Spell 5:", 0x50000300, 40, 448, 88, 24, parent, (HMENU) SKILL5, instance, NULL);
	SendMessage(Spell5, WM_SETFONT, (WPARAM) h_font, TRUE);
	Spell4 = CreateWindowEx(0x00000000, "Static", "Spell 4:", 0x50000300, 40, 424, 88, 24, parent, (HMENU) SKILL4, instance, NULL);
	SendMessage(Spell4, WM_SETFONT, (WPARAM) h_font, TRUE);
	Spell3 = CreateWindowEx(0x00000000, "Static", "Spell 3:", 0x50000300, 40, 400, 88, 24, parent, (HMENU) SKILL3, instance, NULL);
	SendMessage(Spell3, WM_SETFONT, (WPARAM) h_font, TRUE);
	Spell2 = CreateWindowEx(0x00000000, "Static", "Spell 2:", 0x50000300, 40, 376, 88, 24, parent, (HMENU) SKILL2, instance, NULL);
	SendMessage(Spell2, WM_SETFONT, (WPARAM) h_font, TRUE);
	Spell1 = CreateWindowEx(0x00000000, "Static", "Spell 1:", 0x50000300, 40, 352, 88, 24, parent, (HMENU) SKILL1, instance, NULL);
	SendMessage(Spell1, WM_SETFONT, (WPARAM) h_font, TRUE);*/
	//Self Info
	AreaID = CreateWindowEx(0x00000000, "Static", "Area ID:", 0x50000300, 40, 256, 120, 24, parent, (HMENU) AREASELF, instance, NULL);
	SendMessage(AreaID, WM_SETFONT, (WPARAM) h_font, TRUE);
	YSelf = CreateWindowEx(0x00000000, "Static", "Y:", 0x50000300, 40, 232, 120, 24, parent, (HMENU) YSELF, instance, NULL);
	SendMessage(YSelf, WM_SETFONT, (WPARAM) h_font, TRUE);
	XSelf = CreateWindowEx(0x00000000, "Static", "X:", 0x50000300, 40, 208, 120, 24, parent, (HMENU) XSELF, instance, NULL);
	SendMessage(XSelf, WM_SETFONT, (WPARAM) h_font, TRUE);
	ESelf = CreateWindowEx(0x00000000, "Static", "E:", 0x50000300, 40, 184, 120, 24, parent, (HMENU) ESELF, instance, NULL);
	SendMessage(ESelf, WM_SETFONT, (WPARAM) h_font, TRUE);
	HPSelf = CreateWindowEx(0x00000000, "Static", "HP:", 0x50000300, 40, 160, 120, 24, parent, (HMENU) HPSELF, instance, NULL);
	SendMessage(HPSelf, WM_SETFONT, (WPARAM) h_font, TRUE);
	//Buttons
	LoadDll = CreateWindowEx(0x00000000, "Button", "Load DLL", 0x50012F00, 168, 80, 128, 32, parent, (HMENU) Load, instance, NULL);
	SendMessage(LoadDll, WM_SETFONT, (WPARAM) h_font, TRUE);
	Engine = CreateWindowEx(0x00000000, "Button", "Disable Graphics", 0x50012F00, 304, 80, 128, 32, parent, (HMENU) DISABLE, instance, NULL);
	SendMessage(Engine, WM_SETFONT, (WPARAM) h_font, TRUE);
	RunStop = CreateWindowEx(0x00000000, "Button", "Start script", 0x50012F00, 32, 80, 128, 32, parent, (HMENU) START, instance, NULL);
	SendMessage(RunStop, WM_SETFONT, (WPARAM) h_font, TRUE);
	Browse = CreateWindowEx(0x00000000, "Button", "Browse ...", 0x50012F00, 312, 32, 96, 24, parent, (HMENU) BROWSE, instance, NULL);
	SendMessage(Browse, WM_SETFONT, (WPARAM) h_font, TRUE);
	Script = CreateWindowEx(0x00000200, "Edit", "Script/DLL", 0x50010080, 40, 32, 240, 24, parent, (HMENU) SCRIPT, instance, NULL);
	SendMessage(Script, WM_SETFONT, (WPARAM) h_font, TRUE);
	updateThread=CreateThread(0,0,(LPTHREAD_START_ROUTINE)&update_wnd,0,0,0);
	DrawCompass();
}
    
// register all the window classes.
void register_classes()
{
	WNDCLASS wc;

	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance     = instance;
	wc.lpszMenuName  = NULL;
	wc.style		 = CS_PARENTDC | CS_DBLCLKS/*CS_HREDRAW | CS_VREDRAW*/;

	wc.lpfnWndProc   = wnd_proc;
	wc.lpszClassName = WND_MAIN_CLASS_NAME;

	RegisterClass(&wc);
}

// message loop.
int message_loop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	GdiplusShutdown(m_gdiplusToken);
	DeleteObject(h_font);
	return msg.wParam;
}

int WindowMain(){
	instance = GetModuleHandle(NULL);
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	register_classes();
	h_font = CreateFont(-13, 0, 0, 0, FW_NORMAL, 0,
				0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Times New Roman");
	create_wnd();
	return message_loop();
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch(dwReason){
		case DLL_PROCESS_ATTACH:
	if(!GWMemory.BaseOffset){
		InjectErr("BaseOffset");
		return false;
	}
	if(!GWMemory.PacketSendFunction){
		InjectErr("PacketSendFunction");
		return false;
	}else{
		PacketLocation = *(reinterpret_cast<dword*>(GWMemory.BaseOffset - 4));
		FlagLocation = PacketLocation - 0x130;
		PacketMutex = CreateMutex(NULL, false, NULL);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&SendPacketQueueThread, 0, 0, 0);
	}
	if(!GWMemory.AgentArrayPtr){
		InjectErr("AgentArrayPtr");
		return false;
	}
	if(!GWMemory.MessageHandlerStart){
		InjectErr("MessageHandler");
		return false;
	}else{
		DWORD dwOldProtection;
		VirtualProtect(GWMemory.MessageHandlerStart, 9, PAGE_EXECUTE_READWRITE, &dwOldProtection);
		memset(GWMemory.MessageHandlerStart, 0x90, 9);
		VirtualProtect(GWMemory.MessageHandlerStart, 9, dwOldProtection, NULL);
		GWMemory.WriteJMP(GWMemory.MessageHandlerStart, (byte*)CustomMsgHandler);
	}
	if(!GWMemory.SkillLogStart){
		InjectErr("SkillLog");
		return false;
	}else{
		DWORD dwOldProtection;
		VirtualProtect(GWMemory.SkillLogStart, 8, PAGE_EXECUTE_READWRITE, &dwOldProtection);
		memset(GWMemory.SkillLogStart, 0x90, 8);
		VirtualProtect(GWMemory.SkillLogStart, 8, dwOldProtection, NULL);
		GWMemory.WriteJMP(GWMemory.SkillLogStart, (byte*)SkillLogHook);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&SkillLogQueueThread, 0, 0, 0);
	}
	if(!GWMemory.SkillTypeBase){
		InjectErr("SkillTypeBase");
		return false;
	}
	if(!GWMemory.MapIdLocation){
		InjectErr("MapIdLocation");
		return false;
	}
	if(!GWMemory.WriteWhisperStart){
		InjectErr("WriteWhisperStart");
		return false;
	}
	if(!GWMemory.TargetFunctions){
		InjectErr("TargetFunctions");
		return false;
	}
	if(!GWMemory.HeroSkillFunction){
		InjectErr("HeroSkillFunction");
		return false;
	}
	if(!GWMemory.ClickToMoveFix){
		InjectErr("ClickToMoveFix");
		return false;
	}else{
		DWORD dwOldProtection;
		VirtualProtect(GWMemory.ClickToMoveFix, 5, PAGE_EXECUTE_READWRITE, &dwOldProtection);
		byte ClickToMoveFixCode[] = { 0x83, 0xF8, 0x00, 0x90, 0x90 };
		memcpy(GWMemory.ClickToMoveFix, ClickToMoveFixCode, 5);
		VirtualProtect(GWMemory.ClickToMoveFix, 5, dwOldProtection, NULL);
	}
	if(!GWMemory.BuildNumber){
		InjectErr("BuildNumber");
		return false;
	}
	if(!GWMemory.ChangeTargetFunction){
		InjectErr("ChangeTargetFunction");
		return false;
	}
	if(!GWMemory.MaxZoomStill){
		InjectErr("MaxZoomStill");
		return false;
	}
	if(!GWMemory.MaxZoomMobile){
		InjectErr("MaxZoomMobile");
		return false;
	}
	if(!GWMemory.SkillCancelStart){
		InjectErr("SkillCancelStart");
		return false;
	}else{
		DWORD dwOldProtection;
		VirtualProtect(GWMemory.SkillCancelStart, 7, PAGE_EXECUTE_READWRITE, &dwOldProtection);
		memset(GWMemory.SkillCancelStart, 0x90, 7);
		VirtualProtect(GWMemory.SkillCancelStart, 7, dwOldProtection, NULL);
		GWMemory.WriteJMP(GWMemory.SkillCancelStart, (byte*)SkillCancelHook);
	}
	if(!GWMemory.AgentNameFunction){
		InjectErr("AgentNameFunction");
		return false;
	}
	if(!GWMemory.SellSessionStart){
		InjectErr("SellSessionStart");
		return false;
	}else{
		DWORD dwOldProtection;
		VirtualProtect(GWMemory.SellSessionStart, 9, PAGE_EXECUTE_READWRITE, &dwOldProtection);
		memset(GWMemory.SellSessionStart, 0x90, 9);
		VirtualProtect(GWMemory.SellSessionStart, 9, dwOldProtection, NULL);
		GWMemory.WriteJMP(GWMemory.SellSessionStart, (byte*)SellSessionHook);
	}
	if(!GWMemory.SellItemFunction){
		InjectErr("SellItemFunction");
		return false;
	}
	if(!GWMemory.BuyItemFunction){
		InjectErr("BuyItemFunction");
		return false;
	}
	if(!GWMemory.PingLocation){
		InjectErr("PingLocation");
		return false;
	}
	if(!GWMemory.LoggedInLocation){
		InjectErr("LoggedInLocation");
		return false;
	}
	if(!GWMemory.NameLocation){
		InjectErr("NameLocation");
		return false;
	}
	if(!GWMemory.DeadLocation){
		InjectErr("DeadLocation");
		return false;
	}
	if(!GWMemory.BasePointerLocation){
		InjectErr("BasePointerLocation");
		return false;
	}
	if(!GWMemory.DialogStart){
		InjectErr("DialogStart");
		return false;
	}else{
		DWORD dwOldProtection;
		VirtualProtect(GWMemory.DialogStart, 8, PAGE_EXECUTE_READWRITE, &dwOldProtection);
		memset(GWMemory.DialogStart, 0x90, 8);
		VirtualProtect(GWMemory.DialogStart, 8, dwOldProtection, NULL);
		GWMemory.WriteJMP(GWMemory.DialogStart, (byte*)DialogHook);
	}
	if(!GWMemory.EngineStart){
		InjectErr("EngineStart");
		return false;
	}
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&WindowMain, 0, 0, 0);
	init_lua(); //init lua
	/*
	AllocConsole();
	FILE *fh;
	freopen_s(&fh, "CONOUT$", "wb", stdout);
	printf("BaseOffset=0x%06X\n", GWMemory.BaseOffset);
	printf("PacketSendFunction=0x%06X\n", GWMemory.PacketSendFunction);
	printf("CurrentTarget=0x%06X\n", GWMemory.CurrentTarget);
	printf("AgentArrayPtr=0x%06X\n", GWMemory.AgentArrayPtr);
	printf("AgentArrayMaxPtr=0x%06X\n", GWMemory.AgentArrayMaxPtr);
	printf("MessageHandlerStart=0x%06X\n", GWMemory.MessageHandlerStart);
	printf("MessageHandlerReturn=0x%06X\n", GWMemory.MessageHandlerReturn);
	printf("SkillLogStart=0x%06X\n", GWMemory.SkillLogStart);
	printf("SkillLogReturn=0x%06X\n", GWMemory.SkillLogReturn);
	printf("WriteWhisperStart=0x%06X\n", GWMemory.WriteWhisperStart);
	printf("TargetFunctions=0x%06X\n", GWMemory.TargetFunctions);
	printf("HeroSkillFunction=0x%06X\n", GWMemory.HeroSkillFunction);
	printf("ClickToMoveFix=0x%06X\n", GWMemory.ClickToMoveFix);
	printf("BuildNumber=0x%06X\n", GWMemory.BuildNumber);
	printf("ChangeTargetFunction=0x%06X\n", GWMemory.ChangeTargetFunction);
	printf("MaxZoomStill=0x%06X\n", GWMemory.MaxZoomStill);
	printf("MaxZoomMobile=0x%06X\n", GWMemory.MaxZoomMobile);
	printf("SkillCancelStart=0x%06X\n", GWMemory.SkillCancelStart);
	printf("SkillCancelReturn=0x%06X\n", GWMemory.SkillCancelReturn);
	printf("AgentNameFunction=0x%06X\n", GWMemory.AgentNameFunction);
	printf("SellSessionStart=0x%06X\n", GWMemory.SellSessionStart);
	printf("SellItemFunction=0x%06X\n", GWMemory.SellItemFunction);
	printf("BuyItemFunction=0x%06X\n", GWMemory.BuyItemFunction);
	printf("PingLocation=0x%06X\n", GWMemory.PingLocation);
	printf("LoggedInLocation=0x%06X\n", GWMemory.LoggedInLocation);
	printf("NameLocation=0x%06X\n", GWMemory.NameLocation);
	printf("DeadLocation=0x%06X\n", GWMemory.DeadLocation);
	printf("BasePointerLocation=0x%06X\n", GWMemory.BasePointerLocation);
	printf("DialogStart=0x%06X\n", GWMemory.DialogStart);
	printf("DialogReturn=0x%06X\n", GWMemory.DialogReturn);
	printf("EngineStart=0x%06X\n", GWMemory.EngineStart);
	*/
	case DLL_PROCESS_DETACH:
			break;
	}
	return true;
}