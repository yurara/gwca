#include "Memory.h"

Memory GWMemory;

Memory::Memory(void)
{
	byte* start = (byte*)0x00401000;
	byte* end = (byte*)0x00DF0000;

	byte PacketSendCode[] = { 0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x2C, 0x53, 0x56, 0x57, 0x8B,
		0xF9, 0x85 };

	byte BaseOffsetCode[] = { 0x56, 0x33, 0xF6, 0x3B, 0xCE, 0x74, 0x0E, 0x56, 0x33, 0xD2 };

	byte AgentBaseCode[] = { 0x56, 0x8B, 0xF1, 0x3B, 0xF0, 0x72, 0x04 };

	byte MessageHandlerCode[] = { 0x8B, 0x86, 0xA4, 0x0C, 0x00, 0x00, 0x85, 0xC0, 0x0F };

	byte SkillLogCode[] = { 0x8B, 0x46, 0x10, 0x5F, 0x40 };

	byte MapIdLocationCode[] = { 0xB0, 0x7F, 0x8D, 0x55 };

	byte WriteWhisperCode[] = { 0x55, 0x8B, 0xEC, 0x51, 0x53, 0x89, 0x4D, 0xFC, 0x8B, 0x4D,
		0x08, 0x56, 0x57, 0x8B };

	byte TargetFunctionsCode[] = { 0xBA, 0x01, 0x00, 0x00, 0x00, 0xB9, 0x00, 0x80, 0x00, 0x00,
		0xE8 };

	byte HeroSkillFunctionCode[] = { 0x5E, 0xC3, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
		0x90, 0x90, 0x55, 0x8B, 0xEC, 0x8B, 0x45, 0x08, 0x50 };

	byte ClickToMoveCode[] = { 0x3D, 0xD3, 0x01, 0x00, 0x00, 0x74 };

	byte BuildNumberCode[] = { 0x8D, 0x85, 0x00, 0xFC, 0xFF, 0xFF, 0x8D };

	byte ChangeTargetCode[] = { 0x33, 0xC0, 0x3B, 0xDA, 0x0F, 0x95, 0xC0, 0x33 };

	byte MaxZoomStillCode[] = { 0x3B, 0x44, 0x8B, 0xCB };

	byte MaxZoomMobileCode[] = { 0x50, 0xEB, 0x11, 0x68, 0x00, 0x80, 0x3B, 0x44, 0x8B, 0xCE };

	byte SkillCancelCode[] = { 0x85, 0xC0, 0x74, 0x1D, 0x6A, 0x00, 0x6A, 0x42 };
	
	byte SkillTypeBaseCode[] = { 0x8D, 0x04, 0xB6, 0x5E, 0xC1, 0xE0, 0x05,0x05 };

	byte AgentNameCode[] = { 0x57, 0x8B, 0x14, 0x81, 0x8B, 0x82, 0x04, 0x00, 0x00, 0x00,
		0x8B, 0x78, 0x2C, 0xE8 };

	byte SellSessionCode[] = { 0x33, 0xD2, 0x8B, 0xCF, 0xC7, 0x46, 0x0C };

	byte SellItemCode[] = { 0x8B, 0x46, 0x0C, 0x8D, 0x7E, 0x0C, 0x85 };

	byte BuyItemCode[] = { 0x64, 0x8B, 0x0D, 0x2C, 0x00, 0x00, 0x00, 0x89, 0x55, 0xFC,
		0x8B };

	byte PingLocationCode[] = { 0x90, 0x8D, 0x41, 0x24, 0x8B, 0x49, 0x18, 0x6A, 0x30 };

	byte LoggedInLocationCode[] = { 0x85, 0xC0, 0x74, 0x11, 0xB8, 0x07 };

	byte NameLocationCode[] = { 0x6A, 0x14, 0x8D, 0x96, 0xBC };

	byte DeadLocationCode[] = { 0x85, 0xC0, 0x74, 0x11, 0xB8, 0x02 };

	byte BasePointerLocationCode[] = { 0x85, 0xC9, 0x74, 0x3D, 0x8B, 0x46 };

	byte DialogCode[] = { 0x55, 0x8B, 0xEC, 0x8B, 0x41, 0x08, 0xA8, 0x01, 0x75, 0x24 };

	byte EngineCode[] = { 0x53, 0x56, 0xDF, 0xE0, 0xF6, 0xC4, 0x41 };

	while(start!=end){
		if(!memcmp(start, AgentBaseCode, sizeof(AgentBaseCode))){
			AgentArrayPtr = (byte*)(*(dword*)(start+0xC));
			AgentArrayMaxPtr = AgentArrayPtr+0x8;
			CurrentTarget = AgentArrayPtr-0x500;
		}
		if(!memcmp(start, BaseOffsetCode, sizeof(BaseOffsetCode))){
			BaseOffset = start;
		}
		if(!memcmp(start, PacketSendCode, sizeof(PacketSendCode))){
			PacketSendFunction = start;
		}
		if(!memcmp(start, MessageHandlerCode, sizeof(MessageHandlerCode))){
			MessageHandlerStart = start-0x95;
			MessageHandlerReturn = MessageHandlerStart+9;
		}
		if(!memcmp(start, SkillLogCode, sizeof(SkillLogCode))){
			SkillLogStart = start;
			SkillLogReturn = SkillLogStart+8;
		}
		if(!memcmp(start,SkillTypeBaseCode,sizeof(SkillTypeBaseCode))){
			SkillTypeBase = (byte*)(*(dword*)(start+8));
		}
		if(!memcmp(start, MapIdLocationCode, sizeof(MapIdLocationCode))){
			MapIdLocation = (byte*)(*(dword*)(start+0x46));
		}
		if(!memcmp(start, WriteWhisperCode, sizeof(WriteWhisperCode))){
			WriteWhisperStart = start;
		}
		if(!memcmp(start, TargetFunctionsCode, sizeof(TargetFunctionsCode))){
			TargetFunctions = start;
		}
		if(!memcmp(start, HeroSkillFunctionCode, sizeof(HeroSkillFunctionCode))){
			HeroSkillFunction = start+0xC;
		}
		if(!memcmp(start, ClickToMoveCode, sizeof(ClickToMoveCode))){
			ClickToMoveFix = start;
		}
		if(!memcmp(start, BuildNumberCode, sizeof(BuildNumberCode))){
			BuildNumber = start+0x53;
		}
		if(!memcmp(start, ChangeTargetCode, sizeof(ChangeTargetCode))){
			ChangeTargetFunction = start-0x78;
		}
		if(!memcmp(start, MaxZoomStillCode, sizeof(MaxZoomStillCode))){
			MaxZoomStill = start-2;
		}
		if(!memcmp(start, MaxZoomMobileCode, sizeof(MaxZoomMobileCode))){
			MaxZoomMobile = start+4;
		}
		if(!memcmp(start, SkillCancelCode, sizeof(SkillCancelCode))){
			SkillCancelStart = start-0xE;
			SkillCancelReturn = SkillCancelStart+7;
		}
		if(!memcmp(start, AgentNameCode, sizeof(AgentNameCode))){
			AgentNameFunction = start-0x16;
		}
		if(!memcmp(start, SellSessionCode, sizeof(SellSessionCode))){
			SellSessionStart = start-0x48;
			SellSessionReturn = SellSessionStart+9;
		}
		if(!memcmp(start, SellItemCode, sizeof(SellItemCode))){
			SellItemFunction = start-8;
		}
		if(!memcmp(start, BuyItemCode, sizeof(BuyItemCode))){
			BuyItemFunction = start-0xE;
		}
		if(!memcmp(start, PingLocationCode, sizeof(PingLocationCode))){
			PingLocation = (byte*)(*(dword*)(start-9));
		}
		if(!memcmp(start, LoggedInLocationCode, sizeof(LoggedInLocationCode))){
			LoggedInLocation = (byte*)(*(dword*)(start-4) + 4);
		}
		if(!memcmp(start, NameLocationCode, sizeof(NameLocationCode))){
			NameLocation = (byte*)(*(dword*)(start+9));
			EmailLocation = (byte*)(*(dword*)(start-9));
		}
		if(!memcmp(start, DeadLocationCode, sizeof(DeadLocationCode))){
			DeadLocation = (byte*)(*(dword*)(start-4));
		}
		if(!memcmp(start, BasePointerLocationCode, sizeof(BasePointerLocationCode))){
			BasePointerLocation = (byte*)(*(dword*)(start-4));
		}
		if(!memcmp(start, DialogCode, sizeof(DialogCode))){
			DialogStart = start;
			DialogReturn = DialogStart+8;
		}
		if(!memcmp(start, EngineCode, sizeof(EngineCode))){
			EngineStart = start+0x65;
			memcpy(EngineHookSave, EngineStart, 0x20);
		}
		if(	CurrentTarget &&
			BaseOffset &&
			PacketSendFunction &&
			MessageHandlerStart &&
			SkillLogStart &&
			MapIdLocation &&
			WriteWhisperStart &&
			TargetFunctions &&
			HeroSkillFunction &&
			ClickToMoveFix &&
			BuildNumber &&
			ChangeTargetFunction &&
			MaxZoomStill &&
			MaxZoomMobile &&
			SkillCancelStart &&
			SkillTypeBase &&
			SellSessionStart &&
			SellItemFunction &&
			BuyItemFunction &&
			PingLocation &&
			LoggedInLocation &&
			NameLocation &&
			DeadLocation &&
			BasePointerLocation &&
			DialogStart &&
			EngineStart){
			return;
		}
		start++;
	}
}
Memory::~Memory(void)
{
}
template <typename T> T Memory::ReadPtrChain(dword pBase, long pOffset1, long pOffset2, long pOffset3, long pOffset4){
	dword pRead = pBase;
	if(pRead == NULL){return 0;}

	if(pOffset1){pRead = *(dword*)(pRead + pOffset1);}
	if(pRead == NULL){return 0;}

	if(pOffset2){pRead = *(dword*)(pRead + pOffset2);}
	if(pRead == NULL){return 0;}

	if(pOffset3){pRead = *(dword*)(pRead + pOffset3);}
	if(pRead == NULL){return 0;}

	if(pOffset4){pRead = *(dword*)(pRead + pOffset4);}
	if(pRead == NULL){return 0;}

	return (T)pRead;
}
void Memory::WriteJMP(byte* location, byte* newFunction){
	DWORD dwOldProtection;
	VirtualProtect(location, 7, PAGE_EXECUTE_READWRITE, &dwOldProtection);
		location[0] = 0xB8;
		*((dword*)(location + 1)) = (dword)newFunction;
		location[5] = 0xFF;
		location[6] = 0xE0;
	VirtualProtect(location, 7, dwOldProtection, &dwOldProtection);
}
bool Memory::bDataCompare(const unsigned char* pData, const unsigned char* bMask, const char* szMask){
    for( ; *szMask; ++szMask, ++pData, ++bMask)
        if(*szMask == 'x' && *pData != *bMask )
            return false;

    return (*szMask) == 0;
}
unsigned long Memory::FindPattern(unsigned char* bMask, char* szMask, unsigned long dw_Address, unsigned long dw_Len){
    for(unsigned long i=0; i < dw_Len; i++)
        if( bDataCompare( (unsigned char*)( dw_Address+i ),bMask,szMask) )
            return (unsigned long)(dw_Address+i);

    return 0;
} 
long Memory::MapId(){
	return *(long*)(MapIdLocation);
}
long Memory::Ping(){
	return *(long*)(PingLocation);
}
long Memory::LoggedIn(){
	return *(long*)(LoggedInLocation);
}
wchar_t* Memory::Name(){
	return (wchar_t*)(NameLocation);
}
wchar_t* Memory::Email(){
	return (wchar_t*)(EmailLocation);
}
long Memory::Dead(){
	return *(long*)(DeadLocation);
}
dword Memory::BasePointer(){
	return *(dword*)(BasePointerLocation);
}
dword Memory::SkillbarPtr(){
	return ReadPtrChain<dword>(BasePointer(), 0x18, 0x2C, 0x68C);
}
long Memory::CurrentBalthFaction(){
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x724);
}
long Memory::MaxBalthFaction(){
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x74C);
}
long Memory::CurrentKurzickFaction(){
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x6E4);
}
long Memory::MaxKurzickFaction(){
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x744);
}
long Memory::CurrentLuxonFaction(){
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x6F4);
}
long Memory::MaxLuxonFaction(){
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x748);
}
long Memory::MoneySelf(){
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x40, 0xF8, 0x7C);
}
long Memory::MoneyStorage(){
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x40, 0xF8, 0x80);
}
Bag** Memory::BagArrayPointer(){
	return ReadPtrChain<Bag**>(BasePointer(), 0x18, 0x40, 0xF8);
}
long Memory::TitleTreasure(){ //credits to ddarek
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x70C);
}
long Memory::TitleLucky(){ //credits to ddarek
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x25C);
}
long Memory::TitleUnlucky(){ //credits to ddarek
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x284);
}
long Memory::TitleWisdom(){ //credits to ddarek
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x734);
}
long Memory::TitleDrunkard(){ //credits to ddarek
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x11C);
}
long Memory::TitleSunspear(){ //credits to ddarek
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x2AC);
}
long Memory::TitleVanguard(){ //credits to ddarek
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x644);
}
long Memory::TitleNorn(){ //credits to ddarek
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x66C);
 }
long Memory::TitleAsura(){ //credits to ddarek
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x5F4);
}
long Memory::TitleDeldrimor(){ //credits to ddarek
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x61C);
}
long Memory::TitleNorthMastery(){ //credits to ddarek
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x694);
}
long Memory::TitleLightbringer(){ //credits to ddarek
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x324);
}
long Memory::TitleSweet(){ //credits to ddarek
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x554);
}
long Memory::TitleParty(){ //credits to ddarek
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x6BC);
}
long Memory::TitleCommander(){ //credits to ddarek
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0x374);
}
long Memory::TitleTrackLuxon(){ //total points transfered to Luxon's - credits to ddarek
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0xF4);
}
long Memory::TitleTrackKurzick(){ //total points transfered to Kurzick's - credits to ddarek
	return ReadPtrChain<long>(BasePointer(), 0x18, 0x2C, 0x7A4, 0xCC);
}
dword Memory::MerchantItems(){
	return ReadPtrChain<dword>(BasePointer(), 0x18, 0x2C, 0x24);
}
dword Memory::HeroesStruct(){
	return ReadPtrChain<dword>(BasePointer(), 0x18, 0x2C, 0x520);
}
dword Memory::GHStruct(){
	return ReadPtrChain<dword>(BasePointer(), 0x18, 0x3C);
}