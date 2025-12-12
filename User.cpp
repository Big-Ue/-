#include <iostream>
#include "LifeEntity.h"
#include "User.h"
#include "def.h"
#include "Fighter.h"
#include "Mage.h"
#include "Orc.h"

CUser::CUser (){
	user = NULL;
};	

void CUser::set_user (CLifeEntity *in_usr){
	user = in_usr;
}

int CUser::goto_next_city (int next){
	if (!user) return -1;
	int type = user->isA();
	if (type == efighter){
		return ((CFighter *) user)->goto_next_city (next);
	} else if (type == emage) {
		return ((CMage *) user)->goto_next_city (next);
	} else if (type == eorc) {
		return ((COrc *) user)->goto_next_city (next);
	}
	cout << "不是一般戰士" << endl;
	return -1;
}

int CUser::get_current_city (){
	if (!user) return -1;
	int type = user->isA();
	if (type == efighter){
		return ((CFighter *) user)->get_current_city ();
	} else if (type == emage) {
		return ((CMage *) user)->get_current_city ();
	} else if (type == eorc) {
		return ((COrc *) user)->get_current_city ();
	}
	return -1;
}