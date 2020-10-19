#pragma once
#include "gameNode.h"
#include "boss.h"
#include "bossGolemKing.h"

enum BOSSSCENESTATE
{
	BS_INIT,BS_UPDATE
};


class bossDungeonScene : public gameNode
{
private:
	BOSSSCENESTATE _bsState;
private:
	bossGolemKing* _golemKing;

public:

	HRESULT init();
	void release();
	void update();
	void render();
};

