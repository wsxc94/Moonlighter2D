#pragma once
#include "gameNode.h"
enum MENU_STATE
{
	계속하기, 맵툴, 옵션, 나가기
};
class startScene : public gameNode
{
private:
	int _count;
	int _index;
	int _alpha;

	int a_alpha;

	int t_count;
	int t_index;

	bool titleLoad;

	int r_index;

	int state;

	MENU_STATE m_st;


public:
	HRESULT init();
	void release();
	void update();
	void render();
	void bubbleAnim();
	void titleAnim();
	void currentPos();
};

