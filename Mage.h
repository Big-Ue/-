#ifndef MAGE_H
#define MAGE_H

#include "LifeEntity.h"
#include <vector>

#define MAGE_MAXHP 150
#define MAGE_MAXSP 200
#define MAGE_MAXINT 30

using namespace std;

class CFood;
class CBag;
class CItem;
class CItemData;

class CMage : public CLifeEntity {
public:
	CMage(int initHP = 0, int initSP = 0, int initInt = 0, int in_city = 1, int initMoney = 200, int initExp = 0, string initName = "Mage");
	~CMage();
	int getIntellect();
	int physicaldamage();
	virtual int attack(CLifeEntity *);
	virtual int defense(CLifeEntity *);
	virtual int isA();
	void captureItem(CItem *);
	int showAllBagItems();
	void getAllBagItems(vector<int>&, vector<int>&);
	bool useBagItems(int no);
	int goto_next_city(int next_dir);
	int get_current_city();
	// sell all items from bag and return total gained
	int sellAllBagItems(CItemData* itemData);
	// sell a single bag unit by global 1-based unit index. Returns gained money, 0 on failure.
	int sellBagItem(int unitIndex, CItemData* itemData);

	// Magic (MP) support for mage
	int getMagic() const;
	int getMAXMagic() const;
	void addMagic(int val);
	void delMagic(int val);

	// Level
	int getLevel() const;
	void levelUp();
	void setLevel(int lv);

private:
	int cur_city;
	int Intellect;
	CBag *bag;
	// Mage-specific magic points
	int Magic;
	int maxMagic;
	int Level;
};

#endif