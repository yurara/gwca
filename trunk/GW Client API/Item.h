#ifndef _ITEM_H
#define _ITEM_H

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

class ItemManager {
public:
	ItemManager(){}
	~ItemManager();

	long GetBagSize(int iBag){
		Bag* pBag = GetBagPtr(iBag);
		if(!pBag){ return 0; }
		return pBag->slots;
	}

	long GetItemId(int iBag, int iSlot){
		Item* pItem = GetItemPtr(iBag, iSlot);
		if(!pItem){ return 0; }

		return pItem->id;
	}

	long GetItemModelId(int iBag, int iSlot){
		Item* pItem = GetItemPtr(iBag, iSlot);
		if(!pItem){ return 0; }

		return pItem->modelId;
	}

	long GetItemModelId(long itemId){
		Item* pItem = GetItemPtr(itemId);
		if(!pItem){ return 0; }

		return pItem->modelId;
	}

	long FindIdKit(){	
		for(int i = 1;i < 16;i++){
			Bag* pBag = GetBagPtr(i);
			if(!pBag){ continue; }

			Item** pItems = pBag->itemArray;
			Item* pCurrentItem;
			for(int j = 0;j < pBag->slots;j++){
				pCurrentItem = pItems[j];
				if(!pCurrentItem){ continue; }
				if(pCurrentItem->modelId == 2989||pCurrentItem->modelId == 5899){ return pCurrentItem->id; }
			}
		}
		return 0;
	}

	long GetItemByModelId(long model){
		for(int i = 1;i < 16;i++){
			Bag* pBag = GetBagPtr(i);
			if(!pBag){ continue; }

			Item** pItems = pBag->itemArray;
			Item* pCurrentItem;
			for(int j = 0;j < pBag->slots;j++){
				pCurrentItem = pItems[j];
				if(!pCurrentItem){ continue; }
				if(pCurrentItem->modelId == model){ return pCurrentItem->id; }
			}
		}
		return 0;
	}

	long GetItemPositionByItemId(long itemId, long mode){
		for(int i = 1;i < 16;i++){
			Bag* pBag = GetBagPtr(i);
			if(!pBag){ continue; }

			Item** pItems = pBag->itemArray;
			Item* pCurrentItem;
			for(int j = 0;j < pBag->slots;j++){
				pCurrentItem = pItems[j];
				if(!pCurrentItem){ continue; }
				if(pCurrentItem->id == itemId){
					if(mode == 1){ return i; }
					if(mode == 2){ return j+1; }
				}
			}
		}
		return 0;
	}

	long GetItemPositionByModelId(long modelId, long mode){
			for(int i = 1;i < 16;i++){
			Bag* pBag = GetBagPtr(i);
			if(!pBag){ continue; }

			Item** pItems = pBag->itemArray;
			Item* pCurrentItem;
			for(int j = 0;j < pBag->slots;j++){
				pCurrentItem = pItems[j];
				if(!pCurrentItem){ continue; }
				if(pCurrentItem->modelId == modelId){
					if(mode == 1){ return i; }
					if(mode == 2){ return j+1; }
				}
			}
		}
		return 0;
	}

	long FindEmptySlot(long startingBag, long mode){
		for(int i = startingBag;i < 16;i++){
			Bag* pBag = GetBagPtr(i);
			if(!pBag){ continue; }

			Item** pItems = pBag->itemArray;
			Item* pCurrentItem;
			for(int j = 0;j < pBag->slots;j++){
				pCurrentItem = pItems[j];
				if(!pCurrentItem){ 
					if(mode == 1){ return i; }
					if(mode == 2){ return j+1; }
				}
			}
		}
		return 0;
	}

	long FindNextGoldItem(long lastBag){
		for(int i = 1;i < lastBag+1;i++){
			Bag* pBag = GetBagPtr(i);
			if(!pBag){ continue; }

			Item** pItems = pBag->itemArray;
			Item* pCurrentItem;
			for(int j = 0;j < pBag->slots;j++){
				pCurrentItem = pItems[j];
				if(!pCurrentItem){ continue; }
				if(pCurrentItem->extraItemInfo->rarity == 0x40){ return pCurrentItem->id; }
			}
		}
		return 0;
	}

	long GetItemPositionByRarity(byte rarity, long mode){
		for(int i = 1;i < 16;i++){
			Bag* pBag = GetBagPtr(i);
			if(!pBag){ continue; }

			Item** pItems = pBag->itemArray;
			Item* pCurrentItem;
			for(int j = 0;j < pBag->slots;j++){
				pCurrentItem = pItems[j];
				if(!pCurrentItem){ continue; }
				if(pCurrentItem->extraItemInfo->rarity == rarity){
					if(mode == 1){ return i; }
					if(mode == 2){ return j+1; }
				}
			}
		}
		return 0;
	}

	Bag* GetBagPtr(int iBag){
		if(iBag < 1){ return 0; }
		Bag* pBag = MySectionA->BagArrayPointer()[iBag];
		return pBag;
	}

	Item* GetItemPtr(int iBag, int iSlot){
		Bag* pBag = GetBagPtr(iBag);
		if(!pBag){ return 0; }
		if(iSlot > pBag->slots || iSlot < 1){ return 0; }

		Item** pItems = pBag->itemArray;
		Item* pItem = pItems[(iSlot - 1)];
		return pItem;
	}

	Item* GetItemPtr(long itemId){
		for(int i = 1;i < 16;i++){
			Bag* pBag = GetBagPtr(i);
			if(!pBag){ continue; }

			Item** pItems = pBag->itemArray;
			Item* pCurrentItem;
			for(int j = 0;j < pBag->slots;j++){
				pCurrentItem = pItems[j];
				if(!pCurrentItem){ continue; }
				if(pCurrentItem->id == itemId){ return pCurrentItem; }
			}
		}
		return NULL;
	}
};

#endif