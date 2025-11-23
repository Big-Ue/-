
#ifndef GLOBALINFO_H
#define GLOBALINFO_H


class CItemData;
class CMapData;
class CCmdParser;
class CUser;
class CMonsterData;

class CGlobalInfo {
public:
	static CItemData *itm_data;			// 遊戲物品資料  包含武器食物一般物品，儲存其參數    -> ItemData.h
	static CMapData *map_data;			// 地圖資料     管理城市、怪物生成、地圖描述等      -> MapData.h
	static CCmdParser *parser;			// 指令解析器   處理玩家文字輸入
	static CUser *user;					// 玩家資料     包括角色狀態、背包、所在城市
	static CMonsterData *monster_data;	// 怪物資料     管理怪物屬性與生成邏輯
};

#endif