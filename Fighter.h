#ifndef FIGHTER_H
#define FIGHTER_H

#include "LifeEntity.h"
#include <vector>

#define FIGHTER_MAXHP 200
#define FIGHTER_MAXSP 120
#define FIGHTER_MAXLUCKY 20

using namespace std;

class CFood;
class CBag;
class CItem;
class CItemData;

class CFighter : public CLifeEntity {
public:
	CFighter (int initHP = 0, int initSP = 0, int initLucky = 0, int in_city = 1, int initMoney = 300, int initExp = 0, string initName = "«_ÀIªÌ");
	~CFighter();
	int getLucky ();
	int physicaldamage (); 
	virtual int attack (CLifeEntity *);
	virtual int defense (CLifeEntity *); 
	virtual int isA ();
	void captureItem (CItem *);
	int showAllBagItems ();
	void getAllBagItems(vector<int>&, vector<int>&);
	bool useBagItems (int no);
	int goto_next_city (int next_dir);
	int get_current_city ();
	// sell all items from bag and return total gained
	int sellAllBagItems(CItemData* itemData);
	// sell a single bag unit by global 1-based unit index. Returns gained money, 0 on failure.
	int sellBagItem(int unitIndex, CItemData* itemData);

	// Level
	int getLevel() const;
	void levelUp();
	void setLevel(int lv);
private:
	int cur_city;
	int Lucky;
	CBag *bag;
	int Level;
};

#endif