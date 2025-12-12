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
#include "Mage.h"
#include "Orc.h"
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
		cout << "怪物已死，從怪物身上掉下寶物" << endl;
		CItemData *id = CGlobalInfo::itm_data;
		int type = usr->isA();
		if (type == efighter || type == emage || type == eorc){
			CItem *randItem = id->getRand();
			if (type == efighter) {
				CFighter *fp = static_cast<CFighter*>(usr);
				if (fp) fp->captureItem(randItem);
			}
			else if (type == emage) {
				CMage *mp = static_cast<CMage*>(usr);
				if (mp) mp->captureItem(randItem);
			}
			else if (type == eorc) {
				COrc *op = static_cast<COrc*>(usr);
				if (op) op->captureItem(randItem);
			}

			cout << "賺取 " << monster->getMoney() << " 金幣" << endl;
			usr->setMoney(usr->getMoney()+ monster->getMoney());
			cout << "習得 " << monster->getExp() << " 點經驗值" << endl;
			usr->setExp(usr->getExp() + monster->getExp());
		}
		cityptr->remove_moster_by_engname(monster_engname);
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
	int type = usr->isA();
	if (type == efighter || type == emage || type == eorc){
		string uname = usr->getname();
		cout << uname << " 把背包打開" << endl;
		int num = 0;
		if (type == efighter) {
			CFighter *fp = static_cast<CFighter*>(usr);
			if (fp) num = fp->showAllBagItems();
		}
		else if (type == emage) {
			CMage *mp = static_cast<CMage*>(usr);
			if (mp) num = mp->showAllBagItems();
		}
		else if (type == eorc) {
			COrc *op = static_cast<COrc*>(usr);
			if (op) num = op->showAllBagItems();
		}
		if (num == 0){
			cout << "背包空空如也" << endl;
			cout << uname << "  關上背包" << endl;
			return 0;
		}

		int selection = 0;
		cout << "需要什麼物品 (0代表不需要)" << endl;
		cin >> selection;
		while (selection > 0){
			bool used = false;
			if (type == efighter) {
				CFighter *fp = static_cast<CFighter*>(usr);
				if (fp) used = fp->useBagItems(selection);
			}
			else if (type == emage) {
				CMage *mp = static_cast<CMage*>(usr);
				if (mp) used = mp->useBagItems(selection);
			}
			else if (type == eorc) {
				COrc *op = static_cast<COrc*>(usr);
				if (op) used = op->useBagItems(selection);
			}
			if (!used){
				cout << "無此選項存在" << endl;
			}
			selection = 0;
			cout << "需要什麼物品 (0代表不需要)" << endl;
			cin >> selection;
		}
		cout << uname << "  關上背包" << endl;
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

	// 顯示職業
	int ptype = usr->isA();
	string pjob = "未知職業";
	if (ptype == efighter) pjob = "冒險者";
	else if (ptype == emage) pjob = "法師";
	else if (ptype == eorc) pjob = "獸人";
	cout << "職業: " << pjob << endl;

	// 顯示等級
	int plevel = 1;
	if (ptype == efighter) {
		CFighter* fptr = static_cast<CFighter*>(usr);
		if (fptr) plevel = fptr->getLevel();
	} else if (ptype == emage) {
		CMage* mptr = static_cast<CMage*>(usr);
		if (mptr) plevel = mptr->getLevel();
	} else if (ptype == eorc) {
		COrc* optr = static_cast<COrc*>(usr);
		if (optr) plevel = optr->getLevel();
	}
	cout << "等級: LV " << plevel << endl;

	cout << "錢包餘額: " << usr->getMoney() << " 元" << endl;

	cout << "可用經驗: " << usr->getExp() << " 點" << endl;

	cout << "血量: " << usr->getHP() << "/" << usr->getMAXHP() << " HP" << endl;
	usr->bloodbarshow("", usr->getMAXHP(), usr->getHP());
	//cout << endl;

	cout << "體力: " << usr->getSP() << "/" << usr->getMAXSP() << " SP" << endl;
	usr->bloodbarshow("", usr->getMAXSP(), usr->getSP());
	//cout << endl;

	// Show magic (MP) for mages
	if (ptype == emage) {
		CMage* mptr = static_cast<CMage*>(usr);
		if (mptr) {
			cout << "魔力: " << mptr->getMagic() << "/" << mptr->getMAXMagic() << " MP" << endl;
			usr->bloodbarshow("", mptr->getMAXMagic(), mptr->getMagic());
		}
	}

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

	int cur_city = CGlobalInfo::user->get_current_city();
	CPlace* cityptr = CGlobalInfo::map_data->get_place_by_id(cur_city);
	if (cityptr->getname() != "市集") {
		cout << "需在市集才能使用該功能" << endl;
		return 0;
	}

	system("cls");

	struct ShopItem { string name; int data; int price; bool isWeapon; };
	vector<ShopItem> shopItems;
	int food_size = 0;

	CItemData* idata = CGlobalInfo::itm_data;
	if (!idata) {
		cout << "商品資料遺失" << endl;
		return 0;
	}

	for (size_t i = 0; i < idata->food_array.size(); ++i) {
		CFood* f = idata->food_array[i];
		if (!f) continue;
		int price = idata->getFoodPrice((int)i);
		shopItems.push_back({f->getName(), f->gethpbonus(), price, false});
	}
	food_size = (int)shopItems.size();
	for (size_t i = 0; i < idata->weapon_array.size(); ++i) {
		CWeapon* w = idata->weapon_array[i];
		if (!w) continue;
		int price = idata->getWeaponPrice((int)i);
		shopItems.push_back({w->getName(), w->getattackbonus(), price, true});
	}

	cout << "歡迎來到大陸市集，在這你可以找到各種你想要得物品" << endl;
	cout << "當找到喜歡的物品，你可以輸入:" << endl;
	cout << "add+商品ID -> 將商品加入購物車(只有加入購物的商品才能帶走歐~)" << endl;
	cout << "del+商品ID -> 將商品從購物車刪除" << endl;
	cout << "check     -> 查看購物車" << endl;
	cout << "settle    -> 結帳" << endl;
	cout << "sale      -> 出售背包內所有或單一物品 (輸入 all 或 索引)" << endl;
	cout << "out       -> 烙幹" << endl;
	cout << endl;
	cout << "食物店鋪: " << endl;
	cout << "ID | 食物名稱 | 回復HP | 價格" << endl;

	for (int i = 0; i < food_size; ++i) {
		cout << (i+1) << " | " << shopItems[i].name << " | " << shopItems[i].data << " | " << shopItems[i].price << endl;
	}

	cout << endl << "武器店鋪: " << endl;
	cout << "ID | 武器名稱 | 攻擊力 | 價格" << endl;
	for (int i = food_size; i < (int)shopItems.size(); ++i) {
		cout << (i+1) << " | " << shopItems[i].name << " | " << shopItems[i].data << " | " << shopItems[i].price << endl;
	}

	CLifeEntity* usr = CGlobalInfo::user->get_user();
	if (!usr) {
		cout << "使用者資料遺失" << endl;
		return 0;
	}
	int utype = usr->isA();
	if (utype != efighter && utype != emage && utype != eorc) {
		cout << "僅戰鬥職業可以購買物品" << endl;
		return 0;
	}

	CFighter* fptr = nullptr;
	CMage* mptr = nullptr;
	COrc* optr = nullptr;
	if (utype == efighter) fptr = static_cast<CFighter*>(usr);
	else if (utype == emage) mptr = static_cast<CMage*>(usr);
	else if (utype == eorc) optr = static_cast<COrc*>(usr);

	string cmd;
	int chid;
	vector<int> cart;
	while (true) {
		if (!(cin >> cmd)) {
			cin.clear();
			cin.ignore(1024, '\n');
			continue;
		}
		if (cmd == "add") {
			if (!(cin >> chid)) {
				cout << "請輸入正確的商品 ID" << endl;
				cin.clear();
				cin.ignore(1024, '\n');
				continue;
			}
			if (chid <= 0 || chid > (int)shopItems.size()) {
				cout << "沒有這個商品" << endl;
				continue;
			}
			cart.push_back(chid - 1);
			cout << shopItems[chid - 1].name << " 已加入購物車" << endl;
		}
		else if (cmd == "del") {
			if (!(cin >> chid)) {
				cout << "請輸入正確的商品 ID" << endl;
				cin.clear();
				cin.ignore(1024, '\n');
				continue;
			}
			if (chid <= 0 || chid > (int)shopItems.size()) {
				cout << "找不到商品" << endl;
				continue;
			}
			auto it = find(cart.begin(), cart.end(), (chid - 1));
			if (it != cart.end()) {
				cart.erase(it);
				cout << shopItems[chid - 1].name << " 從購物車刪除" << endl;
			}
			else {
				cout << "找不到商品" << endl;
			}
		}
		else if (cmd == "check") {
			sort(cart.begin(), cart.end());
			if (cart.empty()) {
				cout << "購物車空空如也" << endl;
				continue;
			}
			for (int idx : cart) {
				cout << shopItems[idx].name << endl;
			}
		}
		else if (cmd == "settle") {
			int total = 0;
			sort(cart.begin(), cart.end());
			cout << "購入商品: " << endl;
			for (int idx : cart) {
				cout << shopItems[idx].name << " " << shopItems[idx].price << " 元" << endl;
				total += shopItems[idx].price;
			}
			cout << "總價: " << total << " 元" << endl;
			cout << "你的錢包: " << usr->getMoney() << " 元" << endl;

			if (total <= usr->getMoney()) {
				usr->setMoney(usr->getMoney() - total);
				cout << "結帳完成: 剩餘 "<< usr->getMoney() << " 元" << endl;

				for (int idx : cart) {
					if (idx >= food_size) {
						if (fptr) fptr->captureItem(idata->getWeapon(idx - food_size));
						else if (mptr) mptr->captureItem(idata->getWeapon(idx - food_size));
						else if (optr) optr->captureItem(idata->getWeapon(idx - food_size));
					}
					else {
						if (fptr) fptr->captureItem(idata->getFood(idx));
						else if (mptr) mptr->captureItem(idata->getFood(idx));
						else if (optr) optr->captureItem(idata->getFood(idx));
					}
				}
				cart.clear();
			}
			else {
				cout << "結帳失敗: 餘額不足" << endl;
			}
		}
		else if (cmd == "sale") {
			vector<int> item_isA;
			vector<int> item_ID;
			if (fptr) fptr->getAllBagItems(item_isA, item_ID);
			else if (mptr) mptr->getAllBagItems(item_isA, item_ID);
			else if (optr) optr->getAllBagItems(item_isA, item_ID);
			if (item_isA.empty()) {
				cout << "背包空空如也" << endl;
				continue;
			}
			vector<pair<int,int>> units;
			for (size_t i = 0; i < item_isA.size(); ++i) {
				int type = item_isA[i];
				int id = item_ID[i]-1;
				units.push_back({type,id});
			}
			int total = 0;
			for (size_t i = 0; i < units.size(); ++i) {
				int type = units[i].first;
				int id = units[i].second;
				if (type == efood) {
					cout << (i+1) << ". " << idata->getFood(id)->getName() << " 賣出價: " << idata->getFoodPrice(id) << " 元" << endl;
					total += idata->getFoodPrice(id);
				} else if (type == eweapon) {
					cout << (i+1) << ". " << idata->getWeapon(id)->getName() << " 賣出價: " << idata->getWeaponPrice(id) << " 元" << endl;
					total += idata->getWeaponPrice(id);
				}
			}
			cout << "總共可獲得: " << total << " 元，輸入 'all' 賣出全部或輸入索引賣出單一物品" << endl;
			cin >> cmd;
			if (cmd == "all") {
				int gain = 0;
				if (fptr) gain = fptr->sellAllBagItems(idata);
				else if (mptr) gain = mptr->sellAllBagItems(idata);
				else if (optr) gain = optr->sellAllBagItems(idata);
				usr->setMoney(usr->getMoney() + gain);
				cout << "出售完成，獲得 " << gain << " 元，目前餘額 " << usr->getMoney() << " 元" << endl;
			} else {
				try {
					int idx = stoi(cmd);
					if (idx <= 0) throw 0;
					int gain = 0;
					if (fptr) gain = fptr->sellBagItem(idx, idata);
					else if (mptr) gain = mptr->sellBagItem(idx, idata);
					else if (optr) gain = optr->sellBagItem(idx, idata);
					if (gain > 0) {
						usr->setMoney(usr->getMoney() + gain);
						cout << "出售完成，獲得 " << gain << " 元，目前餘額 " << usr->getMoney() << " 元" << endl;
					} else {
						cout << "出售失敗，索引不存在" << endl;
					}
				} catch (...) {
					cout << "輸入不合法，已取消出售" << endl;
				}
			}
		}
		else if (cmd == "out") {
			if (!cart.empty()) {
				cout << "您的購物車還有未結帳的商品!!" << endl;
				cout << "如確認離開則清空購物車，輸入 ok 直接離開，或者其他字元放棄離開" << endl;
				cin >> cmd;
				if (cmd != "ok") {
					continue;
				}
			}

			system("cls");
			int cur_city = CGlobalInfo::user->get_current_city();
			CGlobalInfo::map_data->show_description(cur_city);
			break;
		}
		else {
			cout << "沒有這個指令" << endl;
		}
	}
	return 0;
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
	//cout << "showmoney" << " -> 查看錢包" << endl;
	cout << "checkbody" << " -> 查看狀態" << endl;
	cout << "upgrade" << "   -> 使用經驗升級等級 (消耗 EXP，需足夠經驗)" << endl;
	cout << "clr " << "      -> 清空視窗" << endl;
	cout << "map " << "      -> 顯示地圖" << endl;
	cout << endl << "目前所在: ";
	int cur_city = CGlobalInfo::user->get_current_city();
	CGlobalInfo::map_data->show_description(cur_city);
	return 0;
}


int function_upgrade(vector<string>& tokens) {
    if (tokens.size() != 1) {
        for (auto &t : tokens) cerr << t << " ";
        cerr << " command error" << endl;
        return 0;
    }

    CLifeEntity* usr = CGlobalInfo::user->get_user();
    if (!usr) return 0;
    int type = usr->isA();

    int curLevel = 0;
    if (type == efighter) {
        CFighter* f = static_cast<CFighter*>(usr);
        if (!f) return 0;
        curLevel = f->getLevel();
    } else if (type == emage) {
        CMage* m = static_cast<CMage*>(usr);
        if (!m) return 0;
        curLevel = m->getLevel();
    } else if (type == eorc) {
        COrc* o = static_cast<COrc*>(usr);
        if (!o) return 0;
        curLevel = o->getLevel();
    } else {
        cout << "此職業無法升級" << endl;
        return 0;
    }

    int need = 100 * curLevel;
    cout << "升級將消耗 " << need << " 點經驗。是否要升級？(y/n) ";
    string ans;
    if (!(cin >> ans)) {
        cin.clear();
        cin.ignore(1024, '\n');
        cout << "讀取輸入失敗, 取消升級" << endl;
        return 0;
    }

    if (ans == "y" || ans == "Y") {
        if (type == efighter) {
            CFighter* f = static_cast<CFighter*>(usr);
            if (f) f->levelUp();
        } else if (type == emage) {
            CMage* m = static_cast<CMage*>(usr);
            if (m) m->levelUp();
        } else if (type == eorc) {
            COrc* o = static_cast<COrc*>(usr);
            if (o) o->levelUp();
        }
    } else {
        cout << "已取消升級" << endl;
    }

    // clear any leftover input
    cin.clear();
    cin.ignore(1024, '\n');
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
    //mappingfunc [string("showmoney")] = function_show_money;
    mappingfunc [string("clr")] = function_clr;
    mappingfunc [string("map")] = function_map;
    mappingfunc [string("help")] = function_help;
    mappingfunc [string("upgrade")] = function_upgrade;

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

