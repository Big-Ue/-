#include <iostream>
//#include <fstream>
#include <cstdlib>
#include <string>
#include <iterator>
#include <sstream>
#include <ctime>
#include <windows.h>
#include "Fighter.h"
#include "Monster.h"
#include "food.h"
#include "ItemData.h"
#include "MapData.h"
#include "GlobalInfo.h"
#include "CmdParser.h"
#include "User.h"
#include "MonsterData.h"
#include <limits>
using namespace std;

#define MAXBARLEN 40

void askforbaglist (CFighter *f){
	int num = f->showAllBagItems ();
	if (num == 0)
		return;
	cout << f->getname () << " 需要使用背包物品嗎?\n0 --> No, Others --> Yes, 使用之物品編號: ";
	int selection;
	cin >> selection;
	if (selection){		
		if (!f->useBagItems (selection))
			cout << "無此選項存在" << endl;
	}
}

CMonster *MonsterSelection (CFighter *f){
	int selection;
	askforbaglist (f);
	cout << "請選擇怪物等級: 1. 訊咖 2. 強咖 3. 猛咖 4. 找死咖 " << endl;
	cin >> selection;
	int m_MaxHP, m_MaxSP, m_MaxRough;
	switch (selection){		
		case 1:
			m_MaxHP = f->getMAXHP () / 2;
			m_MaxSP = f->getMAXSP () / 2;
			m_MaxRough = (int)(f->getMAXHP () * 0.1);
			break;
		case 2:
			m_MaxHP = f->getMAXHP ();
			m_MaxSP = f->getMAXSP ();
			m_MaxRough = (int)(f->getMAXHP () * 0.2);
			break;
		case 3:
			m_MaxHP = (int)(f->getMAXHP () * 1.5);
			m_MaxSP = (int)(f->getMAXSP () * 1.2);
			m_MaxRough = (int)(f->getMAXHP () * 0.2);
			break;
		default:
			m_MaxHP = f->getMAXHP () * 2;
			m_MaxSP = f->getMAXSP () * 2;
			m_MaxRough = (int)(f->getMAXHP () * 0.5);			
	}
	CMonster *m  = new CMonster(m_MaxHP, m_MaxSP, m_MaxRough);
	system ("CLS");
	return m;
}

void bloodbarshow (int maxhp, int hp){		
		cout <<  "HP     |";
		float hpslotlen = (float)maxhp / MAXBARLEN;
		int numhp = (int)ceil(hp / hpslotlen);
		for (int i = 0; i < numhp; i++){
			cout <<"#";
		}
		numhp = (int)floor((maxhp- hp) / hpslotlen);
		for (int i = 0; i < numhp; i++){
			cout <<" ";
		}
		cout << "|" << endl;
}

void fightstatus (CMonster *m, CFighter *f){
		cout << endl << f->getname () << endl;
		bloodbarshow (f->getMAXHP (), f->getHP ());
		cout << m->getname () << endl;
		bloodbarshow (m->getMAXHP (), m->getHP ());
		cout << endl;
}

void fightshow (CMonster *m, CFighter *f){
		fightstatus (m, f);
		cout << "您剩下 " << f->getHP () << "滴血" << endl;
		Sleep (1000);		
}

void startfight (CMonster *m, CFighter *f){
	int f_damage = 0, s_damage = 0;
	CLifeEntity *first, *second;
	int whofirst;
	while (!m->isdead () && !f->isdead()){

		whofirst = rand () % 2;
		if (whofirst == 0){
			cout << "怪物搶得先機，先出手傷人" << endl;
			first = (CLifeEntity *) m;
			second = (CLifeEntity *) f;
		} else {
			cout << "你搶得先機，先出手傷人" << endl;
			first = (CLifeEntity *) f;
			second = (CLifeEntity *) m;	
		}

		s_damage = first->attack (second);		
		if (second->isdead()){
			whofirst == 0 ? fightshow ((CMonster *)first, (CFighter *)second) : fightshow ((CMonster *)second, (CFighter *)first);
			break;
		}

		f_damage = second->attack (first);		
		whofirst == 0 ? fightshow ((CMonster *)first, (CFighter *)second) : fightshow ((CMonster *)second, (CFighter *)first);
	}
	if (m->isdead () && !f->isdead ()){
		cout << "怪物已死，從怪物身上掉下寶物" << endl;		
		CItemData *id = CGlobalInfo::itm_data;			
		f->captureItem (id->getRand ());		
		//f->showAllBagItems ();
	}
}

// CGlobalInfo.h 
CItemData *CGlobalInfo::itm_data = new CItemData;
CMapData *CGlobalInfo::map_data = new CMapData;
CCmdParser *CGlobalInfo::parser = new CCmdParser;
CUser *CGlobalInfo::user = new CUser;
CMonsterData *CGlobalInfo::monster_data = new CMonsterData;

int CMonster::counter_for_monster_id = 0;

void Initialize (){ 
	srand((unsigned int)time(NULL));                // 設定亂數種子
	CGlobalInfo::itm_data->Initialize ();			// 初始化物品資料
	CGlobalInfo::map_data->generate_monsters ();	// 生成地圖怪物
}

int main(){		
	Initialize ();	
	CFighter *fighter = new CFighter();
	CGlobalInfo::user->set_user ((CLifeEntity *) fighter);		
	int cur_city = CGlobalInfo::user->get_current_city ();
	cout << endl;
	CGlobalInfo::map_data->show_description (cur_city);		

	while (CGlobalInfo::parser->query () >= 0){
		//CGlobalInfo::map_data->show_description (cur_city);
		//cin >> cur_city;
	}

#if 0
	CFighter fighter;	
	while (!fighter.isdead ()){
		CMonster *m = MonsterSelection (&fighter);
		if (m){
			fightstatus (m, &fighter);
			cout << endl << endl << "戰鬥開始" << endl  << endl;
			startfight (m, &fighter);
			delete m;
		} else
			cout << "No monster generated" << endl;
	}
	cout << "Game Over!!!" << endl;
#endif
	system("pause");
	return 0;
}

