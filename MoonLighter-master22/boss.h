#pragma once
#include "gameNode.h"

enum BOSSSTATE
{
	BOSS_INIT,BOSS_UPDATE,BOSS_DIE
};

class boss : public gameNode
{
protected:
	BOSSSTATE _bState;		//보스 스테이트
	float _x, _y;			//보스 중점좌표
	RECT _bossRC;			//보스 렉트

	vector<animation*> _vAni;	//보스 애니메이션 모음
	int _aniIdx;				//보스 벡터 이동할 인덱스

public:
	boss() {}
	virtual ~boss() {}

	virtual HRESULT init(int x, int y);			//xy 좌표로 초기화
	virtual void release();
	virtual void update();
	virtual void render();

};

