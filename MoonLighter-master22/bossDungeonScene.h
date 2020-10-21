#pragma once
#include "gameNode.h"
#include "boss.h"
#include "bossGolemKing.h"
#include "tileNode.h"
#include "potal.h"
#include "itemManager.h"


enum BOSSSCENESTATE
{
	BS_INIT,BS_UPDATE,BS_RESULT
};

struct tagDungeonSlot
{
	int slotIdx;
	bool isEmpty;
};

enum class BOSSRESULTKIND
{
	RT_POTAL,RT_DIE
};

class bossDungeonScene : public gameNode
{
private:
	itemManager* _itemManager;
	vector<gameItem*> _vRootItem;
private:
	vector<gameItem*> _vItem;
	tagDungeonSlot _dungeonSlot[28];
	vector<RESULTENEMY> _vEnemy;			//결과창 에너미s
	RESULTENEMY* _killEnemy;				//플레이어죽인에너미
	animation* _aniCenter;					//결과창 가운데에있는 애니메이션 ( 포탈 )
	BOSSRESULTKIND _rtKind;
private:
	BOSSSCENESTATE _bsState;
private:
	bossGolemKing* _golemKing;

	vector<tagTile> _vTile;

	potal* _potal;
	bool _isPlayerRender;				//플레이어 렌더할거냐
public:

	HRESULT init();
	void release();
	void update();
	void render();

	void loadTile();
	void collisionTile();

	void initItemSlot();
	void getInvenItem();
	void deleteInvenItems();
	void itemResultRender();
	void countRender(int count, int destX, int destY);
	void resultRender();

};

