#pragma once
#include "gameNode.h"
class bossDungeonScene : public gameNode
{
private:
	animation* _aniBossUp;
	animation* _aniBossDead1;
	animation* _aniBossDead2;
	animation* _aniBossFistShoot;
	animation* _aniBossHandShootStart;
	animation* _aniBossHandShootEnd;

	vector<animation*> _vAni;
	int _idx;

	float _bossX;
	float _bossY;
public:

	HRESULT init();
	void release();
	void update();
	void render();
};

