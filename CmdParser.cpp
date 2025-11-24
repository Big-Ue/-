#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cassert>
#include "Place.h"
#include "CmdParser.h"
#include "GlobalInfo.h"
#include "User.h"
#include "MapData.h"
#include "Fighter.h"
#include "def.h"
#include "ItemData.h"
#include "food.h"
#include "weapon.h"

int function_exit (vector<string> &tokens){	
	return -1;
}

int function_next_direction (vector<string> &tokens){	
	if (tokens.size () != 1){
		for (vector<string>::iterator it = tokens.begin (); it != tokens.end (); it++){
			cerr << (*it) << " ";
		}
		cerr << " command error" << endl;
		return 0;
	}
	int next_dir = 0;
	string dir = tokens [0];
	/*if (dir == string ("east")){
		next_dir = eEast;
	} else if  (dir == string ("west")){
		next_dir = eWest;
	} else if  (dir == string ("north")){
		next_dir = eNorth;	
	} else if  (dir == string ("south")){
		next_dir = eSouth;	
	} else if  (dir == string ("eastnorth")){
		next_dir = eEastNorth;	
	} else if  (dir == string ("eastsouth")){
		next_dir = eEastSouth;	
	} else if  (dir == string ("westnorth")){
		next_dir = eWestNorth;	
	} else if  (dir == string ("westsouth")){
		next_dir = eWestSouth;*/
	if (dir == string("w")) {
		next_dir = eNorth;
	}
	else if (dir == string("a")) {
		next_dir = eWest;
	}
	else if (dir == string("s")) {
		next_dir = eSouth;
	}
	else if (dir == string("d")) {
		next_dir = eEast;
	} else {
		cout << "no such command " << dir << endl;
		return 0;
	}

	int next_city = CGlobalInfo::user->goto_next_city (next_dir);
	if (next_city){
		system ("cls");
		CGlobalInfo::map_data->show_description (next_city);		
	} else {
		cout << "那邊沒路喔!!!請換個方向吧" << endl;		
	}
	return 0;
}

int function_list (vector<string> &tokens){	
	if (tokens.size () != 1){
		for (vector<string>::iterator it = tokens.begin (); it != tokens.end (); it++){
			cerr << (*it) << " ";
		}
		cerr << " command error" << endl;
		return 0;
	}	
	int city = CGlobalInfo::user->get_current_city ();	
	CPlace *cityptr = CGlobalInfo::map_data->get_place_by_id (city);
	if (cityptr){
		cityptr->show_mosters ();
	}	
	return 0;
}

int function_kill (vector<string> &tokens){	
	if (tokens.size () != 2){
		for (vector<string>::iterator it = tokens.begin (); it != tokens.end (); it++){
			cerr << (*it) << " ";
		}
		cerr << " command error" << endl;
		return 0;
	}	
	string monster_engname = tokens[1];
	int city = CGlobalInfo::user->get_current_city ();	
	CPlace *cityptr = CGlobalInfo::map_data->get_place_by_id (city);
	assert(cityptr);
	CMonster *monster = cityptr->get_monster_by_engname (monster_engname);
	if (!monster){
		cout << "no such monsters" << endl;
		return 0;
	}

	CLifeEntity *usr = CGlobalInfo::user->get_user ();
	assert (usr);
	if (usr->kill (monster)){		
		cityptr->remove_moster_by_engname (monster_engname);		
		cout << "怪物已死，從怪物身上掉下寶物" << endl;		
		CItemData *id = CGlobalInfo::itm_data;		
		if (usr->isA() == efighter){
			((CFighter *) usr)->captureItem (id->getRand ());				
		}
	} else {
		cout << "你現在屬於死亡狀態" << endl;
	}

	return 0;
}


int function_check_bag (vector<string> &tokens){	
	if (tokens.size () != 1){
		for (vector<string>::iterator it = tokens.begin (); it != tokens.end (); it++){
			cerr << (*it) << " ";
		}
		cerr << " command error" << endl;
		return 0;
	}	
	CLifeEntity *usr = CGlobalInfo::user->get_user ();
	assert (usr);
	if (usr->isA() == efighter){
		CFighter *f = (CFighter *) usr;	
		cout << f->getname () << " 把背包打開" << endl;
		int num = f->showAllBagItems ();		
		if (num == 0){			
			cout << "背包空空如也" << endl;
			cout << f->getname () << "  關上背包" << endl;
			return 0;
		}
	
		int selection = 0;		
		cout << "需要什麼物品 (0代表不需要)" << endl;
		cin >> selection;
		while (selection > 0){
			if (!f->useBagItems (selection)){
				cout << "無此選項存在" << endl;
			}
			selection = 0;
			cout << "需要什麼物品 (0代表不需要)" << endl;
			cin >> selection;
		}		
		cout << f->getname () << "  關上背包" << endl;
	}	
	cin.clear();
	cin.ignore(1024, '\n');		
	return 0;
}


int function_clr(vector<string>& tokens) {
	if (tokens.size() != 1) {
		for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
			cerr << (*it) << " ";
		}
		cerr << " command error" << endl;
		return 0;
	}
	
	system("cls");
	int cur_city = CGlobalInfo::user->get_current_city();
	CGlobalInfo::map_data->show_description(cur_city);

	return 0;
}


int function_map(vector<string>& tokens) {
	if (tokens.size() != 1) {
		for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
			cerr << (*it) << " ";
		}
		cerr << " command error" << endl;
		return 0;
	}

	string RPG_map = "\
            郊外草原----沙漠 \n\
               |             \n\
     森林----城鎮中心--市集  \n\
               |             \n\
               |             \n\
              礦洞          ";

	cout << RPG_map << endl;
	

	return 0;
}


int function_check_body(vector<string>& tokens) {
	if (tokens.size() != 1) {
		for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
			cerr << (*it) << " ";
		}
		cerr << " command error" << endl;
		return 0;
	}

	system("cls");
	

	CLifeEntity* usr = CGlobalInfo::user->get_user();


	cout << usr->getname() << " 的狀態" << endl;

	cout << "血量: " << usr->getHP() << "/" << usr->getMAXHP() << " HP" << endl;
	usr->bloodbarshow("", usr->getMAXHP(), usr->getHP());
	//cout << endl;

	cout << "體力: " << usr->getSP() << "/" << usr->getMAXSP() << " SP" << endl;
	usr->bloodbarshow("", usr->getMAXSP(), usr->getSP());
	//cout << endl;

	cout << "防禦力: " << usr->defense(usr) << endl;


	cout << "武器: " << usr->getWeapon() << endl;

	//CFighter::defense(usr);

	cout << endl << "目前所在: ";
	int cur_city = CGlobalInfo::user->get_current_city();
	CGlobalInfo::map_data->show_description(cur_city);

	
}	


int function_shop(vector<string>& tokens) {
	if (tokens.size() != 1) {
		for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
			cerr << (*it) << " ";
		}
		cerr << " command error" << endl;
		return 0;
	}

	system("cls");

	vector<string> shname;
	vector<int> shdata;
	//vector<int> shinID;
	vector<int> shprice;

	string name;
	int data;
	int inID;
	int price;

	ifstream fin_food("food.txt");
	if (!fin_food) {
		cout << "讀檔失敗: food.txt" << endl;
		return 0;
	}

	cout << "歡迎來到大陸市集，在這你可以找到各種你想要得物品" << endl;
	cout << "當找到喜歡的物品，你可以輸入:" << endl;
	cout << "add+商品ID -> 將商品加入購物車(只有加入購物的商品才能帶走歐~)" << endl;
	cout << "del+商品ID -> 將商品從購物車刪除" << endl;
	cout << "check     -> 查看購物車" << endl;
	cout << "settle    -> 結帳" << endl;
	cout << "out       -> 烙幹" << endl;
	cout << endl;
	cout << "食物店鋪: " << endl;
	cout << "ID | 食物名稱 | 回復HP | 價格" << endl;
	while (!fin_food.eof()) {
		fin_food >> inID >> name >> data >> price;
		cout << inID << " | " << name << " | " << data << " | " << price << endl;
		shname.push_back(name);
		shdata.push_back(data);
		shprice.push_back(price);
	}
	fin_food.close();
	const int food_size = shname.size();

	int a = shname.size();
	ifstream fin_weapon("weapon.txt");
	if (!fin_weapon) {
		cout << "讀檔失敗: weapon.txt" << endl;
		return 0;
	}
	cout << "武器店鋪: " << endl;
	cout << "ID | 武器名稱 | 攻擊力 | 價格" << endl;
	while (!fin_weapon.eof()) {
		fin_weapon >> inID >> name >> data >> price;
		cout << inID+a << " | " << name << " | " << data << " | " << price << endl;
		shname.push_back(name);
		shdata.push_back(data);
		shprice.push_back(price);
	}
	fin_weapon.close();
	cout << endl;
	CLifeEntity* usr = CGlobalInfo::user->get_user();
	string cmd;
	int chid;
	vector<int> ch;
	while (true) {
		cin >> cmd;
		if (cmd == "add") {
			cin >> chid;
			if (chid <= 0 || chid > shname.size()) {
				cout << "沒有這個商品" << endl;
				continue;
			}
			ch.push_back(chid-1);
			cout << shname[chid - 1] << " 已加入購物車" << endl;
		}
		else if (cmd == "del") {
			cin >> chid;
			if (chid <= 0 || chid > shname.size()) {
				cout << "找不到商品" << endl;
				continue;
			}
			auto it = find(ch.begin(), ch.end(), (chid-1));
			if (it != ch.end()) {
				ch.erase(it);
				cout << shname[chid - 1] << " 從購物車刪除" << endl;
			}
			else {
				cout << "找不到商品" << endl;
			}
		}
		else if (cmd == "check") {
			sort(ch.begin(), ch.end());
			if (ch.size() == 0) {
				cout << "購物車空空如也" << endl;
				continue;
			}
			for (vector<int>::iterator it = ch.begin(); it != ch.end(); it++)
			{
				cout << shname[*it] << endl;
			}
			
		}
		else if (cmd == "settle") {
			int tatol = 0;
			
			cout << "購入商品: " << endl;
			sort(ch.begin(), ch.end());
			for (vector<int>::iterator it = ch.begin(); it != ch.end(); it++)
			{
				cout << shname[*it] << " " << shprice[*it] << " 元" << endl;
				tatol += shprice[*it];
			}
			cout << "總價: " << tatol << " 元" << endl;
			cout << "你的錢包: " << usr->getMoney() << " 元" << endl;

			if (tatol <= usr->getMoney()) {
				usr->setMoney(usr->getMoney() - tatol);
				cout << "結帳完成: 剩餘 "<< usr->getMoney() << " 元" << endl;

				CItemData* id = CGlobalInfo::itm_data;
				for (vector<int>::iterator it = ch.begin(); it != ch.end(); it++)
				{
					if (*it >= food_size) {
						((CFighter*)usr)->captureItem(id->getWeapon(*it-food_size));
					}
					else {
						((CFighter*)usr)->captureItem(id->getFood(*it));
					}		
				}
				ch.clear();
				//cout << "離開商店" << endl;
				//break;
			}
			else {
				cout << "結帳失敗: 餘額不足" << endl;
			}	
		}
		else if (cmd == "out") {
			if (ch.size() != 0) {
				cout << "您的購物車還有未結帳的商品!!" << endl;
				cout << "如確認離開則清空購物車，輸入 ok 直接離開，或者其他字元放棄離開" << endl;
				cin >> cmd;
				if (cmd != "ok") {
					continue;
				}
			}
			//cout << "離開商店" << endl;

			//CLifeEntity* usr = CGlobalInfo::user->get_user();
			
			

			system("cls");
			int cur_city = CGlobalInfo::user->get_current_city();
			CGlobalInfo::map_data->show_description(cur_city);
			break;
		}
		else {
			cout << "沒有這個指令" << endl;
		}
	}
}


int function_help(vector<string>& tokens) {
	if (tokens.size() != 1) {
		for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
			cerr << (*it) << " ";
		}
		cerr << " command error" << endl;
		return 0;
	}
	
	system("cls");
	cout << "可用指令:" << endl;
	cout << "exit" << "      -> 結束程式"  << endl;
	cout << "w   " << "      -> 向北移動" << endl;
	cout << "a   " << "      -> 向西移動" << endl;
	cout << "s   " << "      -> 向南移動" << endl;
	cout << "d   " << "      -> 向東移動" << endl;
	cout << "ls  " << "      -> 顯示當前位置怪物資訊" << endl;
	cout << "kill" << "      -> kill+(怪物名稱) 攻擊怪物" << endl;
	cout << "shop" << "      -> 商店(只在市集有效)" << endl;
	cout << "checkbag" << "  -> 查看背包" << endl;
	cout << "checkbody" << " -> 查看狀態" << endl;
	cout << "clr " << "      -> 清空視窗" << endl;
	cout << "map " << "      -> 顯示地圖" << endl;
	cout << endl << "目前所在: ";
	int cur_city = CGlobalInfo::user->get_current_city();
	CGlobalInfo::map_data->show_description(cur_city);
	return 0;
}


CCmdParser::CCmdParser (){
	mappingfunc [string("exit")] = function_exit;
	mappingfunc [string("w")] = function_next_direction;
	mappingfunc [string("a")] = function_next_direction;
	mappingfunc [string("s")] = function_next_direction;
	mappingfunc [string("d")] = function_next_direction;
	/*mappingfunc [string("east")] = function_next_direction;
	mappingfunc [string("eastnorth")] = function_next_direction;
	mappingfunc [string("eastsouth")] = function_next_direction;
	mappingfunc [string("west")] = function_next_direction;
	mappingfunc [string("westsouth")] = function_next_direction;
	mappingfunc [string("westnorth")] = function_next_direction;
	mappingfunc [string("north")] = function_next_direction;
	mappingfunc [string("south")] = function_next_direction;*/
	mappingfunc [string("ls")] = function_list;
	mappingfunc [string("kill")] = function_kill;
	mappingfunc [string("shop")] = function_shop;
	mappingfunc [string("checkbag")] = function_check_bag;
	mappingfunc [string("checkbody")] = function_check_body;
	mappingfunc [string("clr")] = function_clr;
	mappingfunc [string("map")] = function_map;
	mappingfunc [string("help")] = function_help;

#if 0
	for (vector<string>::iterator it = tokens.begin (); it != tokens.end (); it++){
		cout << (*it) << endl;			
	}		
#endif

}


int CCmdParser::query (){
	string sentense;	
	getline (cin, sentense);	// 讀取整行指令
	vector<string> tokens;		
	splitstring (sentense, tokens, string(" "));	
	if (tokens.size () == 0){
		return 0;
	}

	map<string,  void (*)>::iterator it;
	it = mappingfunc.find (tokens [0]);
	if (it == mappingfunc.end ()){
		cout << "no such commands" << endl;
		return 0;
	}	
	return (reinterpret_cast<int(*)(const vector<string> &)>(it->second)(tokens));		
}

//
void CCmdParser::splitstring(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find_first_of(c);
	pos1 = 0;
	while (string::npos != pos2){
		if(pos2-pos1)
			v.push_back(s.substr(pos1, pos2 - pos1));
		pos1 = pos2 + 1;
		pos2 = s.find_first_of(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

