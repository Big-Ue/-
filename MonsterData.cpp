#include <iostream>
#include <fstream>
#include "MonsterData.h"

CMonsterType::CMonsterType (int in_id, string in_name, string in_eng, int in_hp, int in_sp, int in_rough, int in_money, int in_exp){
	id = in_id;
	name = in_name;
	prefix_eng_name = in_eng;
	max_hp = in_hp;
	max_sp = in_sp;
	max_rough = in_rough;
	money = in_money;
	exp = in_exp;
}

CMonsterData::CMonsterData (){
	ifstream fin("monsterdata.txt");	
	if (!fin){
		cout << "ÅªÀÉ¥¢±Ñ: monsterdata.txt" << endl;
		exit(0);
	}
	int tmp_id;
	string tmp_name;
	string tmp_eng;
	int tmp_hp, tmp_sp, tmp_rough, tmp_money, tmp_exp;
	CMonsterType *tmp_type;
	while (fin >> tmp_id >> tmp_name >> tmp_eng >> tmp_hp >> tmp_sp >> tmp_rough >> tmp_money >> tmp_exp){
		tmp_type = new CMonsterType (tmp_id, tmp_name, tmp_eng, tmp_hp, tmp_sp, tmp_rough, tmp_money, tmp_exp);
		monsterdata.push_back (tmp_type);
	}
#if 0
	for (vector<CMonsterType *>::iterator it = monsterdata.begin (); it != monsterdata.end (); it++){
		cout << (*it)->id << " " << (*it)->name << " " << (*it)->prefix_eng_name << " " << (*it)->max_hp << " " << (*it)->max_sp << " " << (*it)->max_rough << endl;
	}
#endif
	fin.close ();
}

CMonsterData::~CMonsterData (){
	for (vector<CMonsterType *>::iterator it = monsterdata.begin (); it != monsterdata.end (); it++){
		delete (*it);
	}
}

int CMonsterData::rand_get_monster_id (){
	int idx = rand () % monsterdata.size ();
	return monsterdata [idx]->id;
}

CMonsterType *CMonsterData::get_monstertype_by_id (int id){
	for (vector<CMonsterType *>::iterator it = monsterdata.begin (); it != monsterdata.end (); it++){
		if ((*it)->id == id){
			return (*it);
		}
	}
	return NULL;
}
