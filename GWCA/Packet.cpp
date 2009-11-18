#include "Packet.h"

dword PacketLocation = 0;
dword FlagLocation = 0;
HANDLE PacketMutex;
std::vector<CPacket*> PacketQueue;

	CPacket::CPacket(byte* nBuffer, dword nSize){
		Buffer = nBuffer;
		Size = nSize;

		Header = *((word*)&Buffer[0]);
	}
	CPacket::~CPacket(){
	
	}
	void CPacket::SetPacket(word nHeader){
		Header = nHeader;
		Set<word>(0, Header);
	}

	void CPacket::SetPacket(word nHeader, word nSize){
		Header = nHeader;
		Size = nSize;
		Set<word>(0, Header);
	}

	void CPacket::NewPacket(word nHeader, word nSize){
		Header = nHeader;
		Size = nSize;
		Set<dword>(0, Header);
	}

	
	void CPacket::SetUString( word pos, char* string )
	{
		for(dword i = 0; i < strlen(string); i++){
			*((word*)&Buffer[pos + (i*2)]) = string[i];
		}
	}
	void CPacket::SetUString( word pos, wchar_t* string )
	{
		for(dword i = 0; i < wcslen(string); i++){
			*((word*)&Buffer[pos + (i*2)]) = string[i];
		}
	}
	

	void CPacket::SendPacket(CPacket* pak){
		if(WaitForSingleObject(PacketMutex, 1000) == WAIT_TIMEOUT) return;
		PacketQueue.push_back(pak);
		ReleaseMutex(PacketMutex);
	}

void SendPacketQueueThread(){
	while(true){
		Sleep(10);

		if(WaitForSingleObject(PacketMutex, 100) == WAIT_TIMEOUT) continue;
		if(PacketQueue.size() < 1 || mapLoading == 2) goto nextLoop;
		if(GWMemory.LoggedIn() != 1 && mapLoading != 1) goto nextLoop;

		{
			std::vector<CPacket*>::iterator itrPak = PacketQueue.begin();
			CPacket* CurPacket = *itrPak;

			dword testValue = 0x99;
			_asm {
				MOV ECX, FlagLocation
				MOV ECX, DWORD PTR DS:[ECX]
				MOVZX ECX, BYTE PTR DS:[ECX+8]
				MOV testValue, ECX
			}
			if((testValue & 1)){
				{
					byte* buffer = CurPacket->Buffer;
					dword psize = CurPacket->Size;
				
					_asm {
						MOV EAX, PacketLocation
						MOV EAX, DWORD PTR DS:[EAX]
						MOV ECX, EAX
						MOV EDX, psize
						PUSH buffer
						CALL GWMemory.PacketSendFunction
					}
				}

				delete [] CurPacket->Buffer;
				delete CurPacket;
			}

			PacketQueue.erase(itrPak);
		}

nextLoop:
		ReleaseMutex(PacketMutex);
	}
}