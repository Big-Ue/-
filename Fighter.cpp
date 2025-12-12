#include <iostream>
#include "Fighter.h"
#include "food.h"
#include "Bag.h"
#include "def.h"
#include "GlobalInfo.h"
#include "MapData.h"
#include "ItemData.h"

using namespace std;

CFighter::CFighter (int initHP, int initSP, int initLucky, int in_city, int initMoney, int initExp, string initName) {
	if (initHP == 0 && initSP == 0 && initLucky == 0){
		// use maximum values by default
		initHP = FIGHTER_MAXHP;
		initSP = FIGHTER_MAXSP;
		initLucky = FIGHTER_MAXLUCKY;
	}

	if (initMoney == 0) {
		initMoney = 300;
	}

	setMoney(initMoney);
	setExp(initExp);
	setname(initName);

	setInitSPHP (initHP,initSP);
	Lucky = initLucky;
	cur_city = in_city;
	bag = new CBag ();
	Level = 1;
	cout << "One Fighter is created with (maxHP, maxSP, maxLucky) = (" << initHP << ", " << initSP << ", " << initLucky << ")" <<endl; 
}

CFighter::~CFighter (){
	if (bag)
		delete bag;
}

int CFighter::getLucky (){
	return Lucky;
}

int CFighter::physicaldamage (){
	return (rand () % getSP ());
}

int CFighter::attack (CLifeEntity *l){
	int damage = physicaldamage () - l->defense (l); 
	if (damage > l->getHP ())
		damage = l->getHP ();
	
	l->gethurt (damage);
	
	if (damage > 0){
		cout << this->getname () << " 猛力一揮，造成 " << l->getname () << " " << damage << " 血損失" <<endl;			
	} else {
		cout << this->getname () << " 猛力一揮，但是 " << l->getname () << " 強力一擋，因此沒有造成任何損失" <<endl;
	}
	return (damage > 0 ? damage : 0);
}

int CFighter::defense (CLifeEntity *l){
	return 	getLucky();
}

void CFighter::captureItem (CItem *in_item){
	
	CBagEntry *entry = bag->item_lookup (in_item->isA(), in_item->getID ());
	if (!entry)
		bag->item_insert (in_item);
	else
		entry->addNum (1);	
	cout << this->getname () << " 獲得 " << in_item->getName () << endl;
}

int CFighter::showAllBagItems (){	
	return bag->showAllItems ();	
}
void CFighter::getAllBagItems(vector<int>& item_isA, vector<int>& item_ID) {
	bag->getAllItems(item_isA, item_ID);
}


bool CFighter::useBagItems (int no){
	CBagEntry* ne = bag->item_lookup (no);
	if (!ne){
		return false;
	} 
	if (ne->itm->isA () == eweapon){
		CWeapon *cur_weapon = this->getWeapon ();
		if (cur_weapon != NULL){			
			CItem *weapon_item = (CItem *) cur_weapon;
			CBagEntry *entry = bag->item_lookup (weapon_item->isA(), weapon_item->getID ());
			if (!entry)
				bag->item_insert (weapon_item);
			else
				entry->addNum (1);	
			cout << this->getname () << " 將手上武器 " << weapon_item->getName () << " 放回背包中" << endl;
			weapon_item->UnUsed (this);			
		}
		this->setWeapon ((CWeapon *) ne->itm);				
	}

	ne->itm->beUsed (this);
	ne->deleteNum ();
	if (ne->getNum () == 0){
		bag->item_delete (ne);
	}
	this->showAllBagItems ();
	return true;
}

int CFighter::isA (){
	return efighter;
}

int CFighter::goto_next_city (int next_dir){
	int next_city = CGlobalInfo::map_data->next_city (cur_city, next_dir);	
	if (next_city){
		cur_city = next_city;
		return cur_city;
	}
	return 0;
}

int CFighter::get_current_city (){
	return cur_city;	
}

int CFighter::sellAllBagItems(CItemData* itemData) {
	if (!itemData) return 0;
	vector<int> item_isA;
	vector<int> item_ID;
	bag->getAllItems(item_isA, item_ID);
	int total = 0;
	for (size_t i = 0; i < item_isA.size(); ++i) {
		int type = item_isA[i];
		int id = item_ID[i];
		if (type == efood) {
			total += itemData->getFoodPrice(id);
		} else if (type == eweapon) {
			total += itemData->getWeaponPrice(id);
		}
	}
	// remove all items from bag
	// simple approach: delete bag and create a new one
	if (bag) delete bag;
	bag = new CBag();
	return total;
}

int CFighter::sellBagItem(int unitIndex, CItemData* itemData) {
	if (!itemData) return 0;
	CItem* itm = bag->popOneItemByGlobalIndex(unitIndex);
	cout << "出售了" << (itm ? itm->getName() : "無效物品") << endl;
	if (!itm) return 0;
	int gain = 0;
	if (itm->isA() == efood) {
		int id = itm->getID();
		gain = itemData->getFoodPrice(id-1);
	} else if (itm->isA() == eweapon) {
		int id = itm->getID();
		gain = itemData->getWeaponPrice(id-1);
	}
	// do not delete item; ItemData owns item objects
	return gain;
}

void CFighter::levelUp() {
	int need = 100 * Level; // EXP needed to level up
	if (getExp() < need) {
		cout << "經驗不足，無法升級。需要 " << need << " 點經驗" << endl;
		return;
	}
	setExp(getExp() - need);
	Level++;
	int newMaxHP = getMAXHP() + 50; // increase by 50 per level
	int newMaxSP = getMAXSP() + 50; // increase by 50 per level
	// setInitSPHP sets both max and current values, so HP/SP will be refilled
	setInitSPHP(newMaxHP, newMaxSP);
	cout << "升級成功！等級現在為 LV " << Level << "。最大 HP/ SP 已增加，並已回滿。" << endl;
}

int CFighter::getLevel() const { return Level; }

void CFighter::setLevel(int lv) { Level = lv; }