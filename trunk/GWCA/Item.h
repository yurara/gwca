#pragma once
#ifndef _ITEM_H
#define _ITEM_H
#include "stdafx.h"
/* Structure definition of Items and bags.Also Contains
   The Item Manager									*/

struct Bag {
	byte unknown1[4];
	long index;
	long id;
	Item* containerItem;
	long numOfItems;
	Bag** bagArray;
	Item** itemArray;
	long fakeSlots;
	long slots;
};

struct ItemExtra {
	byte rarity;
	byte unknown1[3];
	byte modifier;
	byte unknown2[13];
	byte lastModifier;
};

struct Item {
	long id;
	byte unknown1[8];
	Bag* bag;
	byte unknown2[8];
	wchar_t* customized;
	byte unknown3[16];
	long modelId;
	byte unknown4[8];
	ItemExtra* extraItemInfo;
	byte unknown5[15];
	byte quantity;
	byte unknown6[2];
	byte slot;
};
struct SellItemStruct {
	long header;
	long sessionId;
	long random;
	long itemId;
};
class ItemManager {
public:
	ItemManager(){}
	~ItemManager(){}

	long GetBagSize(int iBag);
	long GetItemId(int iBag, int iSlot);
	long GetItemModelId(int iBag, int iSlot);
	long FindIdKit();
	Bag* GetBagPtr(int iBag);
	Item* GetItemPtr(int iBag, int iSlot);
	Item* GetItemPtr(long itemId);
};
extern ItemManager MyItemManager;
#endif