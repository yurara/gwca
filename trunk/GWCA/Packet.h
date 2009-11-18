#pragma once
#ifndef _PACKET_H
#define _PACKET_H
#include "stdafx.h"
/* Contains the CPacketClass with all Packet commands and the
   PacketQueThread											*/

extern dword PacketLocation;
extern dword FlagLocation;

extern HANDLE PacketMutex;

class CPacket
{
public:
	dword Size;
	word Header;
	byte* Buffer;

	CPacket(byte* nBuffer, dword nSize);
	~CPacket(void);
	void SetPacket(word nHeader);
	void SetPacket(word nHeader, word nSize);
	void NewPacket(word nHeader, word nSize);
	void SetUString( word pos, char* string );
	void SetUString( word pos, wchar_t* string );
	void SendPacket(CPacket* pak);
	//no templates in .cpp file :X
	template <typename T> T  Get( word pos )
	{
		return *((T*)&Buffer[pos]);
	}
	template <typename T> void Set( word pos, T val )
	{
		*((T*)&Buffer[pos]) = val;
	}
	template <typename T> void Add( T val )
	{
		*((T*)&Buffer[Size]) = val;
		Size += sizeof(T);
	}
	template <> void Add<char*>( char* val )
	{
		for(word i = 0; i < strlen(val); i++){
			Add<word>(val[i]);
		}
	}
};

extern std::vector<CPacket*> PacketQueue;
void SendPacketQueueThread();

#endif