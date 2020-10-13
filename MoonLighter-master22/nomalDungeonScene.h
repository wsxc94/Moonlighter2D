#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "dungeonMap.h"
#include "object.h"
#include "enemy.h"

class nomalDungeonScene : public gameNode
{
private:

	DungeonMap* _startDungeon;				//전체 던전의 시작점

	DungeonMap* _currentDungeon;				//현재 플레이어가 있는 던전

	vector<pair<POINT, DungeonMap*>> _vMinimap;

	//스타트던전 두루마리
	animation* _golemScroll;

	//플레이어 테스트
	RECT _player;
	bool left;
	bool right;
	bool up;
	bool down;

	//에너미 테스트
	enemy*	_em;

	//던전 층수
	int _dgFloor;

public:
	nomalDungeonScene() { };
	~nomalDungeonScene() { };

	HRESULT init();
	void release();
	void update();
	void render();
	bool minimapPush(POINT pt);
	void minimapRender();
	void setNewFloor();
};

