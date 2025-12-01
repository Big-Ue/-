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

using namespace std;

void UserSystem::account_log_in(void) {
	while (true) {
		cout << "請輸入帳號: ";
		cin >> Username;

		if (Username == "create") {
			cout << "請創建帳戶名稱: ";
			cin >> Username;

			cout << "請創建密碼: ";
			cin >> Password;

			string Index = "user/" + Username + ".txt";
			ofstream fout(Index);
			fout << Password << endl;

			cout << "創建成功!" << endl;
			fout.close();
			continue;
		}

		if (search_account(Username) == 0) {
			break;
		}
		cout << "查無此帳號! 輸入<create>可創建帳戶" << endl;
	}


	
	log_password();

	system("cls");


	ifstream fin(Index);
	string cmd;
	fin >> Password;

	if (!(fin >> cmd)) {
		CFighter* fighter = new CFighter();
		CGlobalInfo::user->set_user ((CLifeEntity *) fighter);
		return;
	}




	int tpye;

	fin >> tpye;
	if (tpye == efighter) {
		string in_name;
		int in_HP, in_MAXHP, in_SP, in_MAXSP, in_Exp, in_Money, in_Lucky, in_City;
				
		fin >> in_name >> in_HP >> in_MAXHP >> in_SP >> in_MAXSP >> in_Exp >> in_Money >> in_Lucky >> in_City;

		CFighter *fighter = new CFighter(in_HP, in_SP, in_Lucky, in_City, in_Money, in_Exp, in_name);
		CGlobalInfo::user->set_user ((CLifeEntity *) fighter);	
		//CLifeEntity* usr = CGlobalInfo::user->get_user();

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
	/*for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); it++)
	{
		cout << *it << " " << endl;
	}*/

	ofstream fout(Index);
	CLifeEntity* usr = CGlobalInfo::user->get_user();
	CFighter* f = (CFighter*)usr;

	fout << Password << endl;
	fout << "User";
	fout << " " << usr->isA();
	fout << " " << usr->getname();
	fout << " " << usr->getHP();
	fout << " " << usr->getMAXHP();
	fout << " " << usr->getSP();
	fout << " " << usr->getMAXSP();
	fout << " " << usr->getExp();
	fout << " " << usr->getMoney();

		
	if (usr->isA() == efighter) {
		fout << " " << f->getLucky();
		fout << " " << CGlobalInfo::user->get_current_city();
		
		vector<int> item_isA;
		vector<int> item_ID;
		int num;
		
		f->getAllBagItems(item_isA, item_ID);
		num = f->showAllBagItems();

		while (num--) {
			fout << " " << item_isA[num];
			fout << " " << (item_ID[num]-1);
		}

	}
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

