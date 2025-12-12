#include <iostream>
#include "Mage.h"
#include "food.h"
#include "Bag.h"
#include "def.h"
#include "GlobalInfo.h"
#include "MapData.h"
#include "ItemData.h"

using namespace std;

CMage::CMage(int initHP, int initSP, int initInt, int in_city, int initMoney, int initExp, string initName) {
	if (initHP == 0 && initSP == 0 && initInt == 0) {
		// use maximum values by default for Mage
		initHP = MAGE_MAXHP;
		initSP = MAGE_MAXSP;
		initInt = MAGE_MAXINT;
	}

	if (initMoney == 0) {
		initMoney = 200;
	}

	setMoney(initMoney);
	setExp(initExp);
	setname(initName);

	setInitSPHP(initHP, initSP);
	Intellect = initInt;
	cur_city = in_city;
	bag = new CBag();
	// Initialize magic (MP)
	maxMagic = initSP; // use initSP as max magic by default
	Magic = initSP;
	Level = 1;
	cout << "One Mage is created with (maxHP, maxSP, maxInt) = (" << initHP << ", " << initSP << ", " << initInt << ")" << endl;
}

CMage::~CMage() {
	if (bag) delete bag;
}

int CMage::getIntellect() {
	return Intellect;
}

int CMage::physicaldamage() {
	// mage physical damage lower, use SP/Magic as resource
	return (rand() % (Magic > 0 ? Magic : 1));
}

int CMage::attack(CLifeEntity *l) {
	// mage deals magic damage based on intellect
	int damage = getIntellect() - l->defense(l);
	if (damage > l->getHP()) damage = l->getHP();

	l->gethurt(damage);

	if (damage > 0) {
		cout << this->getname() << " ?????]?k???? " << l->getname() << " " << damage << " ?I??`" << endl;
	} else {
		cout << this->getname() << " ???]?k?? " << l->getname() << " ?L??" << endl;
	}
	return (damage > 0 ? damage : 0);
}

int CMage::defense(CLifeEntity *l) {
	// mage defense uses intellect / 2
	return getIntellect() / 2;
}

void CMage::captureItem(CItem *in_item) {
	CBagEntry *entry = bag->item_lookup(in_item->isA(), in_item->getID());
	if (!entry)
		bag->item_insert(in_item);
	else
		entry->addNum(1);
	cout << this->getname() << " ??o " << in_item->getName() << endl;
}

int CMage::showAllBagItems() {
	return bag->showAllItems();
}

void CMage::getAllBagItems(vector<int>& item_isA, vector<int>& item_ID) {
	bag->getAllItems(item_isA, item_ID);
}

bool CMage::useBagItems(int no) {
	CBagEntry* ne = bag->item_lookup(no);
	if (!ne) return false;

	CItem* itm = ne->getItem();
	if (itm->isA() == eweapon) {
		// mage can use some weapons; same logic as fighter
		CWeapon *cur_weapon = this->getWeapon();
		if (cur_weapon != NULL) {
			CItem *weapon_item = (CItem *)cur_weapon;
			CBagEntry *entry = bag->item_lookup(weapon_item->isA(), weapon_item->getID());
			if (!entry) bag->item_insert(weapon_item);
			else entry->addNum(1);
			cout << this->getname() << " ???^ " << weapon_item->getName() << " ??I?]" << endl;
			weapon_item->UnUsed(this);
		}
		this->setWeapon((CWeapon*)itm);
	}

	itm->beUsed(this);
	ne->deleteNum();
	if (ne->getNum() == 0) {
		bag->item_delete(ne);
	}
	this->showAllBagItems();
	return true;
}

int CMage::isA() {
	return emage;
}

int CMage::goto_next_city(int next_dir) {
	int next_city = CGlobalInfo::map_data->next_city(cur_city, next_dir);
	if (next_city) {
		cur_city = next_city;
		return cur_city;
	}
	return 0;
}

int CMage::get_current_city() {
	return cur_city;
}

int CMage::sellAllBagItems(CItemData* itemData) {
	if (!itemData) return 0;
	vector<int> item_isA;
	vector<int> item_ID;
	bag->getAllItems(item_isA, item_ID);
	int total = 0;
	for (size_t i = 0; i < item_isA.size(); ++i) {
		int type = item_isA[i];
		int id = item_ID[i];
		if (type == efood) total += itemData->getFoodPrice(id);
		else if (type == eweapon) total += itemData->getWeaponPrice(id);
	}
	if (bag) delete bag;
	bag = new CBag();
	return total;
}

int CMage::sellBagItem(int unitIndex, CItemData* itemData) {
	if (!itemData) return 0;
	CItem* itm = bag->popOneItemByGlobalIndex(unitIndex);
	if (!itm) return 0;
	int gain = 0;
	if (itm->isA() == efood) gain = itemData->getFoodPrice(itm->getID());
	else if (itm->isA() == eweapon) gain = itemData->getWeaponPrice(itm->getID());
	return gain;
}

// Magic accessors
int CMage::getMagic() const { return Magic; }
int CMage::getMAXMagic() const { return maxMagic; }
void CMage::addMagic(int val) { Magic += val; if (Magic > maxMagic) Magic = maxMagic; }
void CMage::delMagic(int val) { Magic -= val; if (Magic < 0) Magic = 0; }

int CMage::getLevel() const { return Level; }
void CMage::levelUp() {
	int need = 100 * Level; // required EXP to level up
	if (getExp() < need) {
		cout << "經驗不足，無法升級。需要 " << need << " 點經驗" << endl;
		return;
	}
	setExp(getExp() - need);
	Level++;
	// increase stats per request: MAXHP +40, MAXSP +20, maxMagic +50
	int newMaxHP = getMAXHP() + 40;
	int newMaxSP = getMAXSP() + 20;
	int newMaxMagic = getMAXMagic() + 50;
	// setInitSPHP sets both max and current HP/SP, so this refills HP/SP
	setInitSPHP(newMaxHP, newMaxSP);
	// update magic and refill MP
	maxMagic = newMaxMagic;
	Magic = maxMagic;
	cout << "升級成功！等級現在為 LV " << Level << "。最大 HP/SP/MP 已增加，並已回滿。" << endl;
}
void CMage::setLevel(int lv) { Level = lv; }