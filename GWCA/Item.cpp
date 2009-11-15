#include "Item.h"
ItemManager MyItemManager;

long ItemManager::GetBagSize(int iBag){
	Bag* pBag = GetBagPtr(iBag);
	if(!pBag){ return 0; }
	return pBag->slots;
}
long ItemManager::GetItemId(int iBag, int iSlot){
	Item* pItem = GetItemPtr(iBag, iSlot);
	if(!pItem){ return 0; }
	return pItem->id;
}
long ItemManager::GetItemModelId(int iBag, int iSlot){
	Item* pItem = GetItemPtr(iBag, iSlot);
	if(!pItem){ return 0; }
	return pItem->modelId;
}
long ItemManager::FindIdKit(){
	for(int i = 1;i < 5;i++){
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
Bag* ItemManager::GetBagPtr(int iBag){
	if(iBag < 1){ return 0; }
	Bag* pBag = GWMemory.BagArrayPointer()[iBag];
	return pBag;
}
Item* ItemManager::GetItemPtr(int iBag, int iSlot){
	Bag* pBag = GetBagPtr(iBag);
	if(!pBag){ return 0; }
	if(iSlot > pBag->slots){ return 0; }
	Item** pItems = pBag->itemArray;
	Item* pItem = pItems[(iSlot - 1)];
	return pItem;
}
Item* ItemManager::GetItemPtr(long itemId){
	for(int i = 1;i < 5;i++){
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