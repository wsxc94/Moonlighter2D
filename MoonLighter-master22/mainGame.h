#pragma once
#include "gameNode.h"
/*앞으로 메인게임은 각각의 씬들만 관리를 한다*/
/*씬들의 헤더파일만 이곳에 추가한다*/
#include "maptoolLoading.h"
#include "maptoolScene.h"
#include "startScene.h"
#include "townLoading.h"
#include "townScene.h"
#include "nomalDungeonScene.h"
#include "bossDungeonScene.h"
#include "dungeonLoading.h"
#include "itemMenu.h"
#include "shopLoading.h"
#include "shopScene.h"
#include "gotoDungeon.h"
#include "bossLoading.h"

class mainGame : public gameNode
{
private:
	itemMenu* _itemMenu;
	HFONT hFont;
	HFONT oldFont;

public:
	HRESULT init();
	void release();
	void update();
	void render();
};