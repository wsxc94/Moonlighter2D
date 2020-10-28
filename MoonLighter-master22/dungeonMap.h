#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "object.h"
#include "enemy.h"
#include "itemManager.h"
#include "potal.h"

enum DUNGEONDOOR
{
	DOOR_OPEN, DOOR_CLOSE
};

enum DUNGEONKIND
{
	DG_NOMAL,DG_SEMIBOSS,DG_SPA
};


class DungeonMap : public gameNode
{
private:
	itemManager* _itemManager;
	vector<gameItem*> _vItem;
	potal* _potal;		//엠블렘으로 집갈때 생성될 포탈
private:
	string _fileName;
	string _backImg;
	int _x, _y;
	bool _isCheck;			//dfs 변수
	bool _isClear;
	vector<tagTile> _vTile;
	vector<tagTile> _vObject;
	vector<object*> _vDGObject;

	vector<int> _vEnemyTest;		//에너미 테스트용벡터
	vector<enemy*> _vEnemy;

	dgDoor* _leftDoor;
	dgDoor* _rightDoor;
	dgDoor* _topDoor;
	dgDoor* _bottomDoor;

	DungeonMap* _leftDg;
	DungeonMap* _rightDg;
	DungeonMap* _topDg;
	DungeonMap* _bottomDg;

	int _test;
	int _floor;

private:
	vector<DungeonMap*> _vMaps;
	queue<DungeonMap*> _qTemp;
	queue<DungeonMap*> _qDfs;
	POINT _pTemp;
	int _mapSize;
	bool _checkLoadTile;
	
	DUNGEONDOOR _doorState;
	DUNGEONKIND _dgKind;
public:
	DungeonMap(int x, int y, int floor);
	~DungeonMap() {}

	HRESULT init();
	void release();
	void update();
	void render();

	void setStartDungeon();
	void setRandomMap(DungeonMap** map);
	void connectMap(DungeonMap** map);
	void bossMapDfs();
	void setBossMap(DungeonMap** map, int percent);
	void loadTiles(DungeonMap** map);
	void dgDoorRender();
	void dgDoorUpdate();
	void dgDoorOpen();
	void dgDoorClose();
	void enemyUpdate();
	void popEnemy(int arr);

	void itemUpdate();

	void checkColiMoveBen();
	void checkColiHole();
	void checkCollisionSpa();

	void checkColiArrow();

	void initPotal();
	void potalUpdate();
	void potalRender();
	void releasePotal();

	//현재 몇층인지 알려줌
	void setCurrentFloor(int floor) { _floor = floor; }

	DungeonMap* findMap(POINT pos);
	bool isFindMap(POINT pos);

	DungeonMap* getLeftDG() { return _leftDg; }
	DungeonMap* getRightDG() { return _rightDg; }
	DungeonMap* getTopDG() { return _topDg; }
	DungeonMap* getBottomDG() { return _bottomDg; }
	POINT getDungeonXY() { return PointMake(_x, _y); }

	vector<tagTile>& getTile() { return _vTile; }
	DungeonMap* moveDungeon(RECT rc);
	// 1 왼쪽  2 오른쪽  3 위   4 아래  로 이동
	int moveDungeonDirection(RECT rc);
	bool isMoveBen(RECT rc);
	bool isTSHole(RECT rc);
	RECT getMoveBenTile(RECT rc);
	RECT getHoleTile(RECT rc);
	vector<tagTile> getVTile() { return _vTile; }
	DUNGEONDOOR getDungeonDoorState() { return _doorState; }
	DUNGEONKIND getDungeonKind() { return _dgKind; }

	potal* getPotal() { return _potal; }

};

