#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "UserSystem.h"
#include "GlobalInfo.h"
#include "LifeEntity.h"
#include "ItemData.h"
#include "bsd-list.h"
#include "Fighter.h"
#include "User.h"
#include "Item.h"
#include "def.h"
#include "Bag.h"
#include "Mage.h"
#include "Orc.h"

using namespace std;

void UserSystem::account_log_in(void) {
	while (true) {
		cout << "請輸入帳號: ";
		cin >> Username;

		if (Username == "create") {
			cout << "請輸入要註冊的帳號名稱: ";
			cin >> Username;

			cout << "請設定密碼: ";
			cin >> Password;

			// choose profession
			cout << "選擇職業: <1 冒險者> <2 法師> <3 獸人>，請輸入數字: ";
			int choice = 1;
			cin >> choice;
			int prof = efighter;
			if (choice == 2) prof = emage;
			else if (choice == 3) prof = eorc;

			string Index = "user/" + Username + ".txt";
			ofstream fout(Index);
			fout << Password << endl;
			// write header token and profession type so loader knows which class to create
			fout << "User " << prof << endl;

			cout << "註冊完成!" << endl;
			fout.close();
			continue;
		}

		if (search_account(Username) == 0) {
			break;
		}
		cout << "找不到此帳號! 若要註冊請輸入 <create>" << endl;
	}

	
	log_password();

	system("cls");


	ifstream fin(Index);
	string cmd;
	fin >> Password;

	if (!(fin >> cmd)) {
		// no further data -> default to fighter; set character name to account username
		CFighter* fighter = new CFighter(0,0,0,1,300,0, Username);
		CGlobalInfo::user->set_user ((CLifeEntity *) fighter);
		return;
	}

	// expected token is "User"
	if (cmd != "User") {
		// unknown format -> default
		CFighter* fighter = new CFighter(0,0,0,1,300,0, Username);
		CGlobalInfo::user->set_user ((CLifeEntity *) fighter);
		return;
	}

	int tpye = 0;
	if (!(fin >> tpye)) {
		// only header present, create default fighter
		CFighter* fighter = new CFighter(0,0,0,1,300,0, Username);
		CGlobalInfo::user->set_user ((CLifeEntity *) fighter);
		fin.close();
		return;
	}

	// skip whitespace and check if there's more character data
	fin >> ws;
	if (fin.eof()) {
		// no saved detailed data -> create default instance based on tpye
		if (tpye == efighter) {
			CFighter* fighter = new CFighter(0,0,0,1,300,0, Username);
			CGlobalInfo::user->set_user ((CLifeEntity *) fighter);
		} else if (tpye == emage) {
			CMage* mage = new CMage(0,0,0,1,200,0, Username);
			CGlobalInfo::user->set_user ((CLifeEntity *) mage);
		} else if (tpye == eorc) {
			COrc* orc = new COrc(0,0,0,1,100,0, Username);
			CGlobalInfo::user->set_user ((CLifeEntity *) orc);
		} else {
			CFighter* fighter = new CFighter(0,0,0,1,300,0, Username);
			CGlobalInfo::user->set_user ((CLifeEntity *) fighter);
		}
		fin.close();
		return;
	}

	// there is detailed saved data; treat as existing save (currently supported for fighter)
	if (tpye == efighter) {
		string in_name;
		int in_HP, in_MAXHP, in_SP, in_MAXSP, in_Exp, in_Money, in_Lucky, in_Level, in_City;
		
		if (!(fin >> in_name >> in_HP >> in_MAXHP >> in_SP >> in_MAXSP >> in_Exp >> in_Money >> in_Lucky >> in_Level >> in_City)) {
			// failed to read full data, fallback to default
			CFighter* fighter = new CFighter(0,0,0,1,300,0, Username);
			CGlobalInfo::user->set_user ((CLifeEntity *) fighter);
			fin.close();
			return;
		}

		CFighter *fighter = new CFighter(in_HP, in_SP, in_Lucky, in_City, in_Money, in_Exp, in_name);
		fighter->setLevel(in_Level);
		CGlobalInfo::user->set_user ((CLifeEntity *) fighter);
		// load bag items if any
		int item_type, item_ID;
		CItemData* id = CGlobalInfo::itm_data;
		while (fin >> item_type >> item_ID) {
			if (item_type == efood) {
				fighter->captureItem(id->getFood(item_ID));
			}
			else if (item_type == eweapon) {
				fighter->captureItem(id->getWeapon(item_ID));
			}
		}
	}
	else if (tpye == emage) {
		string in_name;
		int in_HP, in_MAXHP, in_SP, in_MAXSP, in_Exp, in_Money, in_Int, in_Level, in_City;
		if (!(fin >> in_name >> in_HP >> in_MAXHP >> in_SP >> in_MAXSP >> in_Exp >> in_Money >> in_Int >> in_Level >> in_City)) {
			CMage* mage = new CMage(0,0,0,1,200,0, Username);
			CGlobalInfo::user->set_user ((CLifeEntity *) mage);
			fin.close();
			return;
		}

		CMage *mage = new CMage(in_HP, in_SP, in_Int, in_City, in_Money, in_Exp, in_name);
		mage->setLevel(in_Level);
		CGlobalInfo::user->set_user ((CLifeEntity *) mage);
		// load bag items
		int item_type, item_ID;
		CItemData* id = CGlobalInfo::itm_data;
		while (fin >> item_type >> item_ID) {
			if (item_type == efood) {
				mage->captureItem(id->getFood(item_ID));
			}
			else if (item_type == eweapon) {
				mage->captureItem(id->getWeapon(item_ID));
			}
		}
	}
	else if (tpye == eorc) {
		string in_name;
		int in_HP, in_MAXHP, in_SP, in_MAXSP, in_Exp, in_Money, in_Str, in_Level, in_City;
		if (!(fin >> in_name >> in_HP >> in_MAXHP >> in_SP >> in_MAXSP >> in_Exp >> in_Money >> in_Str >> in_Level >> in_City)) {
			COrc* orc = new COrc(0,0,0,1,100,0, Username);
			CGlobalInfo::user->set_user ((CLifeEntity *) orc);
			fin.close();
			return;
		}

		COrc *orc = new COrc(in_HP, in_SP, in_Str, in_City, in_Money, in_Exp, in_name);
		orc->setLevel(in_Level);
		CGlobalInfo::user->set_user ((CLifeEntity *) orc);
		// load bag items
		int item_type, item_ID;
		CItemData* id = CGlobalInfo::itm_data;
		while (fin >> item_type >> item_ID) {
			if (item_type == efood) {
				orc->captureItem(id->getFood(item_ID));
			}
			else if (item_type == eweapon) {
				orc->captureItem(id->getWeapon(item_ID));
			}
		}
	}
	else {
		// unknown type -> fallback default fighter
		CFighter* fighter = new CFighter(0,0,0,1,300,0, Username);
		CGlobalInfo::user->set_user ((CLifeEntity *) fighter);
	}
	fin.close();
}


void UserSystem::save(void) {
	ifstream fin(Index);
	vector<string> tokens;
	string line;

	while (getline(fin, line)) {
		splitstring(line, tokens, string(" "));
	}
	fin.close();

	ofstream fout(Index);
	CLifeEntity* usr = CGlobalInfo::user->get_user();
	if (!usr) return;

	fout << Password << endl;
	// write header and base stats
	fout << "User";
	fout << " " << usr->isA();
	fout << " " << usr->getname();
	fout << " " << usr->getHP();
	fout << " " << usr->getMAXHP();
	fout << " " << usr->getSP();
	fout << " " << usr->getMAXSP();
	fout << " " << usr->getExp();
	fout << " " << usr->getMoney();

	int type = usr->isA();
	// save class-specific extra value + current city, then bag items (one pair per unit)
	if (type == efighter) {
		CFighter* f = static_cast<CFighter*>(usr);
		if (f) {
			fout << " " << f->getLucky();
			// write level then current city
			fout << " " << f->getLevel();
			fout << " " << CGlobalInfo::user->get_current_city();

			vector<int> item_isA;
			vector<int> item_ID;
			f->getAllBagItems(item_isA, item_ID);
			for (size_t i = 0; i < item_isA.size(); ++i) {
				fout << " " << item_isA[i] << " " << (item_ID[i] - 1);
			}
		}
	}
	else if (type == emage) {
		CMage* m = static_cast<CMage*>(usr);
		if (m) {
			fout << " " << m->getIntellect();
			// write level then current city
			fout << " " << m->getLevel();
			fout << " " << CGlobalInfo::user->get_current_city();

			vector<int> item_isA;
			vector<int> item_ID;
			m->getAllBagItems(item_isA, item_ID);
			for (size_t i = 0; i < item_isA.size(); ++i) {
				fout << " " << item_isA[i] << " " << (item_ID[i] - 1);
			}
		}
	}
	else if (type == eorc) {
		COrc* o = static_cast<COrc*>(usr);
		if (o) {
			// write strength, level and current city
			fout << " " << o->getStrength();
			fout << " " << o->getLevel();
			fout << " " << CGlobalInfo::user->get_current_city();

			vector<int> item_isA;
			vector<int> item_ID;
			o->getAllBagItems(item_isA, item_ID);
			for (size_t i = 0; i < item_isA.size(); ++i) {
				fout << " " << item_isA[i] << " " << (item_ID[i] - 1);
			}
		}
	}
	fout.close();
}


void UserSystem::log_password(void) {
	string password;
	int num = 3;
	ifstream fin(Index);
	fin >> Password;
	fin.close();

	while (true) {
		if (num == 0) {
			cout << "可輸入<reset>重設密碼" << endl;
			num = 3;
		}

		cout << "請輸入密碼: ";
		cin >> password;

		if (password == Password) break;
		else if (password == "reset") {
			ifstream fin(Index);
			vector<string> lines;
			string line;

			cout << "請輸入新密碼: ";
			cin >> Password;

			while (getline(fin, line)) {
				lines.push_back(line);
			}
			fin.close();
				
			lines[0] = Password;

			ofstream fout(Index);
			for (size_t i = 0; i < lines.size(); ++i) {
				fout << lines[i];
				if (i + 1 < lines.size()) fout << endl;
			}

			cout << "密碼已更改" << endl;
			num = 3;
		}
		else {
			cout << "密碼錯誤!" << endl;
			num--;
		}
	}
}


int UserSystem::search_account(string account) {
	string index = "user/" + account + ".txt";
	ifstream fin(index);
	if (!fin) {
		cout << "用戶不存在" << endl;
		return 1;
	}
	Index = index;
	fin.close();
	return 0;
}

	
void UserSystem::splitstring(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find_first_of(c);
	pos1 = 0;
	while (string::npos != pos2) {
		if (pos2 - pos1)
			v.push_back(s.substr(pos1, pos2 - pos1));
		pos1 = pos2 + 1;
		pos2 = s.find_first_of(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

