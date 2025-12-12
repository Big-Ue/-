#include <iostream>
#include "Bag.h"
#include "Item.h"
#include <vector>
using namespace std;

CBagEntry::CBagEntry(CItem *in_item, int in_num){
	itm = in_item; 
	number = in_num;
}

CBag::CBag (){
	LIST_INIT(&listhead);	
}

CBag::~CBag (){
	CBagEntry* ne = listhead.lh_first;
	CBagEntry* tmp = NULL;
	while (ne){
		tmp = ne->getNext();
		LIST_REMOVE (ne, next_link);
		ne = tmp;
	}
}

void CBag::item_insert (CItem *in_item){
	CBagEntry* new_entry = new CBagEntry (in_item);
	LIST_INSERT_HEAD(&listhead, new_entry, next_link);
}

CBagEntry* CBag::item_lookup (int type, int id){
	CBagEntry* ne = listhead.lh_first;
	for (; ne; ne = ne->getNext()){
		if (ne->itm->isA() == type && ne->itm->getID () == id)
			break;
	}
	return ne;
}

CBagEntry* CBag::item_lookup (int no){
	CBagEntry* ne = listhead.lh_first;
	int countnum = 1;
	for (; ne; ne = ne->getNext()){
		if (no == countnum){						
			break;
		}
		countnum++;
	}
	return ne;
}

void CBag::item_delete (CBagEntry* be){
	LIST_REMOVE (be, next_link);
}

int CBag::showAllItems (){
	CBagEntry* ne = listhead.lh_first;
	CItem *tmp;
	int countnum = 0;
	if (ne)
		cout << "背包內容如下所示：" << endl;
	for (; ne; ne = ne->getNext()){
		tmp = ne->itm;
		cout << ++countnum << ". " << tmp->getName () << ", 數量-> " << ne->number << endl;
	}	
	return countnum;
}


void CBag::getAllItems(vector<int>&item_isA, vector<int>& item_ID) {
	CBagEntry* ne = listhead.lh_first;
	CItem* tmp;
	for (; ne; ne = ne->getNext()) {
		tmp = ne->itm;
		for (int k = 0; k < ne->number; ++k) {
			item_isA.push_back(tmp->isA());
			item_ID.push_back(tmp->getID());
		}
	}
}

CItem* CBag::popOneItemByGlobalIndex(int no) {
	CBagEntry* ne = listhead.lh_first;
	int count = 1;
	for (; ne; ne = ne->getNext()) {
		if (count + ne->number - 1 >= no) {
			// this entry contains the requested unit
			CItem* itm = ne->itm;
			ne->deleteNum(1);
			if (ne->getNum() <= 0) {
				LIST_REMOVE(ne, next_link);
				delete ne;
			}
			return itm;
		}
		count += ne->number;
	}
	return NULL;
}
