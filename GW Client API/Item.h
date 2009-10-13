#ifndef _ITEM_H
#define _ITEM_H

struct Bag {
	byte unknown1[4];
	long index;
	byte unknown2[4]; //long id;
	Item* containerItem;
	long numOfItems;
	Bag** bagArray;
	Item** itemArray;
	long fakeSlots;
	long slots;
};

struct Item {
	long id;
	byte unknown1[8];
	Bag* bag;
	byte unknown2[8];
	wchar_t* customized;
	byte unknown3[16];
	long modelId;
	byte unknown4[23];
	byte quantity;
	byte unknown5[2];
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
		Bag* pBag = GetBagPtr(iBag);
		if(iSlot > pBag->slots){ return 0; }

		Item** pItems = pBag->itemArray;
		Item* pItem = pItems[(iSlot - 1)];
		if(!pItem){ return 0; }

		return pItem->id;
	}

	long FindIdKit(){
		for(int i = 1;i < 5;i++){
			Bag* pBag = GetBagPtr(i);
			if(!pBag){ continue; }

			Item** pItems = pBag->itemArray;
			Item* pCurrentItem;
			for(int j = 0;j < pBag->slots;j++){
				pCurrentItem = pItems[j];
				if(!pCurrentItem){ continue; }
				if(pCurrentItem->modelId == 2989){ return pCurrentItem->id; }
			}
		}
		return 0;
	}

private:
	Bag* GetBagPtr(int iBag){
		if(iBag < 1){ return 0; }
		Bag* pBag = MySectionA->BagArrayPointer()[iBag];
		return pBag;
	}
};

#endif