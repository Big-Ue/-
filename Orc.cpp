#include <iostream>
#include "Orc.h"
#include "food.h"
#include "Bag.h"
#include "def.h"
#include "GlobalInfo.h"
#include "MapData.h"
#include "ItemData.h"

using namespace std;

COrc::COrc(int initHP, int initSP, int initStr, int in_city, int initMoney, int initExp, string initName) {
	if (initHP == 0 && initSP == 0 && initStr == 0) {
		// use maximum values by default for Orc
		initHP = ORC_MAXHP;
		initSP = ORC_MAXSP;
		initStr = ORC_MAXSTR;
	}

	if (initMoney == 0) initMoney = 100;

	setMoney(initMoney);
	setExp(initExp);
	setname(initName);

	setInitSPHP(initHP, initSP);
	Strength = initStr;
	cur_city = in_city;
	bag = new CBag();
	Level = 1;
	cout << "One Orc is created with (maxHP, maxSP, maxStr) = (" << initHP << ", " << initSP << ", " << initStr << ")" << endl;
}

COrc::~COrc() {
	if (bag) delete bag;
}

int COrc::getStrength() {
	return Strength;
}

int COrc::physicaldamage() {
	return (rand() % (getSP() > 0 ? getSP() : 1)) + Strength / 2;
}

int COrc::attack(CLifeEntity *l) {
	int damage = physicaldamage() - l->defense(l);
	if (damage > l->getHP()) damage = l->getHP();

	l->gethurt(damage);

	if (damage > 0) {
		cout << this->getname() << " 以蠻力攻擊 " << l->getname() << " " << damage << " 點傷害" << endl;
	} else {
		cout << this->getname() << " 的攻擊對 " << l->getname() << " 無效" << endl;
	}
	return (damage > 0 ? damage : 0);
}

int COrc::defense(CLifeEntity *l) {
	// orc defense based on half strength
	return getStrength() / 3;
}

void COrc::captureItem(CItem *in_item) {
	CBagEntry *entry = bag->item_lookup(in_item->isA(), in_item->getID());
	if (!entry) bag->item_insert(in_item);
	else entry->addNum(1);
	cout << this->getname() << " 拾取 " << in_item->getName() << endl;
}

int COrc::showAllBagItems() {
	return bag->showAllItems();
}

void COrc::getAllBagItems(vector<int>& item_isA, vector<int>& item_ID) {
	bag->getAllItems(item_isA, item_ID);
}

bool COrc::useBagItems(int no) {
	CBagEntry* ne = bag->item_lookup(no);
	if (!ne) return false;

	CItem* itm = ne->getItem();
	if (itm->isA() == eweapon) {
		CWeapon *cur_weapon = this->getWeapon();
		if (cur_weapon != NULL) {
			CItem *weapon_item = (CItem *)cur_weapon;
			CBagEntry *entry = bag->item_lookup(weapon_item->isA(), weapon_item->getID());
			if (!entry) bag->item_insert(weapon_item);
			else entry->addNum(1);
			cout << this->getname() << " 收回 " << weapon_item->getName() << " 到背包" << endl;
			weapon_item->UnUsed(this);
		}
		this->setWeapon((CWeapon*)itm);
	}

	itm->beUsed(this);
	ne->deleteNum();
	if (ne->getNum() == 0) bag->item_delete(ne);
	this->showAllBagItems();
	return true;
}

int COrc::isA() {
	return eorc;
}

int COrc::goto_next_city(int next_dir) {
	int next_city = CGlobalInfo::map_data->next_city(cur_city, next_dir);
	if (next_city) {
		cur_city = next_city;
		return cur_city;
	}
	return 0;
}

int COrc::get_current_city() {
	return cur_city;
}

int COrc::sellAllBagItems(CItemData* itemData) {
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

int COrc::sellBagItem(int unitIndex, CItemData* itemData) {
	if (!itemData) return 0;
	CItem* itm = bag->popOneItemByGlobalIndex(unitIndex);
	if (!itm) return 0;
	int gain = 0;
	if (itm->isA() == efood) gain = itemData->getFoodPrice(itm->getID());
	else if (itm->isA() == eweapon) gain = itemData->getWeaponPrice(itm->getID());
	return gain;
}

void COrc::levelUp() {
	int need = 100 * Level;
	if (getExp() < need) {
		cout << "經驗不足，無法升級。需要 " << need << " 點經驗" << endl;
		return;
	}
	setExp(getExp() - need);
	Level++;
	int newMaxHP = getMAXHP() + 100; // increase by 100 per level
	int newMaxSP = getMAXSP() + 150; // increase by 150 per level
	// setInitSPHP sets both max and current values, so HP/SP will be refilled
	setInitSPHP(newMaxHP, newMaxSP);
	cout << "升級成功！等級現在為 LV " << Level << "。最大 HP/ SP 已增加，並已回滿。" << endl;
}

void COrc::setLevel(int lv) { Level = lv; }

int COrc::getLevel() const { return Level; }