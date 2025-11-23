
// 角色與怪物的基底類別，提供血量、魔力、名字、武器管理及共用操作，
// 並定義戰鬥相關的純虛擬函式讓子類別實作攻擊、防禦與類型。


#ifndef LIFEENTITY_H
#define LIFEENTITY_H

#include <string>
#include "weapon.h"

#define MAXBLOODBARLEN 40
using namespace std;

class CLifeEntity {
public:
	CLifeEntity (int initHP = 0, int initSP = 0, int initMoney = 300, string initname = "");
	void setInitSPHP (int initHP = 0, int initSP = 0);
	bool isdead ();

	void setMoney(int);
	int getMoney();

	void setHP (int);
	void addHP (int);
	int getHP ();
	int getMAXHP ();
	int getSP ();
	int getMAXSP ();
	void addSP (int);
	void delSP (int);
	int gethurt (int hurt);
	string getname ();
	void setname (string);
	CWeapon *getWeapon (){return weapon;};
	void setWeapon (CWeapon *in_weapon){weapon = in_weapon;}
	bool kill (CLifeEntity *);
	virtual int attack (CLifeEntity *) = 0;
	virtual int defense (CLifeEntity *) = 0;
	virtual int isA () = 0;
	void bloodbarshow(string title, int maxvalue, int value);
private:	
	void fightstatus (CLifeEntity *f, CLifeEntity *s);
	
	int maxSP, maxHP;
	int SP, HP;
	int money;
	string Name;
	CWeapon *weapon;

};

#endif