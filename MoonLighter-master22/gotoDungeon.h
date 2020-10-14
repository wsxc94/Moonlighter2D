#pragma once
#include "gameNode.h"
#include "tileNode.h"

class gotoDungeon : public gameNode
{
private:
	vector<tagTile> _vTile;
	bool _portalPosCheck;

	bool _dungeonDoorCheck;

	animation* _playerGotoAnim;
	animation* _DungeonDoorAnim;
public:
	gotoDungeon() {}
	~gotoDungeon() {}

	HRESULT init();
	void release();
	void update();
	void render();

	void loadTile();
	void collTile();


};

