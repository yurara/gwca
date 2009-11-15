#include "stdafx.h"
void InjectErr(const char* lpzText){
	char* buf = new char[100];
	sprintf(buf, "The %s could not be found!\nPlease contact SOMEONE about this issue.", lpzText);
	MessageBox(NULL,(LPCSTR)*buf,"Hooking error!", MB_OK);
}