#include <fstream>
#include <iostream>
#include "Item.h"
#include "food.h"
#include "weapon.h"
#include "ItemData.h"
#include "GlobalInfo.h"
using namespace std;

void CItemData::Initialize (){
	LoadFoodData ();
	LoadWeaponData ();
}

int CItemData::totalsize (){
	return food_array.size () + weapon_array.size ();	
}

CItem *CItemData::getRand (){
	unsigned int randnum = rand () % totalsize ();
	if (randnum < food_array.size ()){
		return food_array[randnum];
	}
	randnum -= food_array.size ();
	if (randnum < weapon_array.size ()){
		return weapon_array[randnum];
	}
	return NULL;
}


CItem* CItemData::getFood(int id) {
	return food_array[id];
}
CItem* CItemData::getWeapon(int id) {
	return weapon_array[id];
}


void CItemData::LoadFoodData (){
	ifstream fin("food.txt");	
	if (!fin){
		cout << "ÅªÀÉ¥¢±Ñ: food.txt" << endl;
		return;
	}
	string name;
	int hp_bonus;
	int inID;
	int price;
	CFood *food;
	// Read safely
	while (fin >> inID >> name >> hp_bonus >> price){
		food = new CFood (name, 0, 0, inID, hp_bonus);
		CGlobalInfo::itm_data->food_array.push_back (food);
		CGlobalInfo::itm_data->food_price.push_back(price);
	}
	fin.close ();
}

void CItemData::LoadWeaponData (){
	ifstream fin("weapon.txt");	
	if (!fin){
		cout << "ÅªÀÉ¥¢±Ñ: weapon.txt" << endl;
		return;
	}
	string name;
	int attack_bonus;
	int inID;
	int price;
	CWeapon *weapon;
	while (fin >> inID >> name >> attack_bonus >> price){
		weapon = new CWeapon (name, 0, 0, inID, attack_bonus);
		CGlobalInfo::itm_data->weapon_array.push_back (weapon);
		CGlobalInfo::itm_data->weapon_price.push_back(price);
	}
	fin.close ();
	
}

int CItemData::getFoodPrice(int idx) {
	if (idx < 0 || idx >= (int)food_price.size()) return 0;
	return food_price[idx];
}

int CItemData::getWeaponPrice(int idx) {
	if (idx < 0 || idx >= (int)weapon_price.size()) return 0;
	return weapon_price[idx];
}
