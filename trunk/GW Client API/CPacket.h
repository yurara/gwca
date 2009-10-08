#ifndef _CPACKET_H
#define _CPACKET_H

class CPacket {
public:
	dword Size;
	word Header;
	byte* Buffer;

	CPacket(byte* nBuffer, dword nSize){
		Buffer = nBuffer;
		Size = nSize;

		Header = *((word*)&Buffer[0]);
	}
	~CPacket(){}

	void SetPacket(word nHeader){
		Header = nHeader;
		Set<word>(0, Header);
	}

	void SetPacket(word nHeader, word nSize){
		Header = nHeader;
		Size = nSize;
		Set<word>(0, Header);
	}

	void NewPacket(word nHeader, word nSize){
		Header = nHeader;
		Size = nSize;
		Set<dword>(0, Header);
	}

	template <typename T> T Get( word pos )
	{
		return *((T*)&Buffer[pos]);
	}
	template <typename T> void Set( word pos, T val )
	{
		*((T*)&Buffer[pos]) = val;
	}
	void SetUString( word pos, char* string )
	{
		for(dword i = 0; i < strlen(string); i++){
			*((word*)&Buffer[pos + (i*2)]) = string[i];
		}
	}
	void SetUString( word pos, wchar_t* string )
	{
		for(dword i = 0; i < wcslen(string); i++){
			*((word*)&Buffer[pos + (i*2)]) = string[i];
		}
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
private:
};

#endif