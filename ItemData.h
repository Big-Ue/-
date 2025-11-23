
#ifndef ITEMDATA_H
#define ITEMDATA_H

#include <vector>

// 物品種類
	// 繼承 CItem 的子類別
	class CFood;	// 食物(名稱、大小、重量、ID、回升hp) -> food.h<末端>
	class CWeapon;	// 武器(名稱、大小、重量、ID、傷害) -> weapon.h<末端>
class CItem;	// 物品的基底類別（父類別）

class CItemData {
public:
	CItemData (){};
	~CItemData ();
	void Initialize ();
	int totalsize ();
	CItem *getRand ();
	vector <CFood *> food_array;	
	vector <CWeapon *> weapon_array;	
private:
	void LoadFoodData ();
	void LoadWeaponData ();
};

#endif
