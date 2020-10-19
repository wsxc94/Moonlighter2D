#pragma once
#include "gameNode.h"

enum POTALSTATE
{
	POTAL_INIT,POTAL_UPDATE,POTAL_BREAK,
	POTAL_PLAYERIN,POTAL_PLAYEROUT
};

class potal : public gameNode
{
private:
	POTALSTATE _potalState;		//포탈의 상태
	float _x, _y;				//포탈 센터좌표
	RECT _rc;					//포탈 바닥렉트
	animation* _init;			//이닛에니메이션
	animation* _update;			//업데이트
	animation* _break;			//부서지는거
	animation* _playerIn;		//플레이어 빨아들이기
	animation* _playerOut;		//플레이어 뱉기

	bool _isActivate;			//활성화됐냐?
	bool _isInRange;			//범위안에 있냐?


public:
	potal() {}
	~potal() {}

	HRESULT init(float x, float y, POTALSTATE state);
	void update();
	void render();


	POTALSTATE getPotalState() { return _potalState;}
	bool getIsActivate() { return _isActivate; }
	bool getIsInRange() { return _isInRange; }
	float getX() { return _x; }
	float getY() { return _y; }
	animation* getAnimation();

	void setPotalState(POTALSTATE state);
	void setIsActivate(bool activate) { _isActivate = activate; }
	void setIsInRange(bool range) { _isInRange = range; }
	float setX(float x) { _x = x; }
	float setY(float y) { _y = y; }

};

