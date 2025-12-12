#ifndef BAG_H
#define BAG_H

#include <iostream>
#include "bsd-list.h"
#include <vector>

using namespace std;

class CItem;

class CBagEntry {
	friend class CBag;
	friend class CFighter;
public:
	CBagEntry (CItem *in_item = NULL, int in_num = 1);
	void addNum (int adds = 1){number += adds;};
	void deleteNum (int dels = 1){number -= dels;};
	int getNum (){return number;};
	CItem* getItem() { return itm; }
	LIST_ENTRY(CBagEntry) next_link; // public for macro access
	CBagEntry* getNext() const { return next_link.le_next; }
private:
	CItem *itm;
	int number;
};

LIST_HEAD(bag_itemlist, CBagEntry);

class CBag {
public:
	CBag ();
	~CBag ();
	void item_insert (CItem *);
	CBagEntry* item_lookup (int type, int id);
	CBagEntry* item_lookup (int no);
	void item_delete (CBagEntry*);
	int showAllItems ();
	void getAllItems(vector<int> &, vector<int>&);
	// pop a single unit by global 1-based index (units enumerated across entries)
	// returns the CItem* for the popped unit, or NULL if not found
	CItem* popOneItemByGlobalIndex(int no);
private:
	bag_itemlist listhead;
};

#endif