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
	while (!fin.eof ()){
		fin >> inID >> name >> hp_bonus >> price;
		//cout << inID << name << hp_bonus << price << endl;
		food = new CFood (name, 0, 0, inID, hp_bonus);
		CGlobalInfo::itm_data->food_array.push_back (food);		
		//cout << name << " " << hp_bonus << endl;
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
	while (!fin.eof ()){
		fin >> inID >> name >> attack_bonus >> price;
		//cout << inID << name << attack_bonus << price << endl;
		weapon = new CWeapon (name, 0, 0, inID, attack_bonus);
		CGlobalInfo::itm_data->weapon_array.push_back (weapon);		
		//cout << name << " " << hp_bonus << endl;
	}
	fin.close ();
	
}
