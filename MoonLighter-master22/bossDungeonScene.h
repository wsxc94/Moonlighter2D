#pragma once
#include "gameNode.h"
class bossDungeonScene : public gameNode
{
private:

public:

	HRESULT init();
	void release();
	void update();
	void render();
};

