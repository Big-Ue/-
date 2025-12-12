#ifndef ORC_H
#define ORC_H

#include "LifeEntity.h"
#include <vector>

#define ORC_MAXHP 300
#define ORC_MAXSP 160
#define ORC_MAXSTR 30

using namespace std;

class CFood;
class CBag;
class CItem;
class CItemData;

class COrc : public CLifeEntity {
public:
	COrc(int initHP = 0, int initSP = 0, int initStr = 0, int in_city = 1, int initMoney = 100, int initExp = 0, string initName = "Orc");
	~COrc();
	int getStrength();
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

	int getLevel() const;
	void levelUp();
	void setLevel(int lv);
private:
	int cur_city;
	int Strength;
	CBag *bag;
	int Level;
};

#endif