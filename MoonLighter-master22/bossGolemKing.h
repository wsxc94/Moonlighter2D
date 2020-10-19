#pragma once
#include "boss.h"
#include "progressBar.h"

enum class GOLEMKINGSTATE
{
	BS_INIT,BS_IDLE,BS_FIST,BS_ROCK_SHOOT,BS_ROCK_ROUND,BS_HAND
};

enum class GOLEMANISTATE
{
	ANI_BOSSUP,ANI_DEAD1,ANI_DEAD2,ANI_FISTSHOOT,ANI_HANDSHOOTSTART,ANI_HANDSHOOTEND,ANI_IDLE
};

enum class HANDSTATE
{
	HAND_INIT,HAND_FALL,HAND_UP
};

struct tagGolemHand
{
	animation* ani;
	RECT rc;
	float x, y;
	float hight;
	float speed;
	float shadowScale;
	int count;
	bool isHit;
	HANDSTATE state;
};

struct tagRock
{
	image* img;
	RECT rc;
	float x, y;
	float hight;
	float shadowScale;
	int time;				//유지시간
	bool isHit;
};

class bossGolemKing : public boss
{
private:
	GOLEMKINGSTATE _golemState;					//골렘 스테이트
	GOLEMANISTATE _golemAni;					//골렘 애니메이션

	vector<GOLEMKINGSTATE> _vGolemAttack;			//골렘 공격패턴들

	animation* _aniBossUp;						//init모션
	animation* _aniBossDead1;					//죽는거 첫번째
	animation* _aniBossDead2;					//죽는거 두번째
	animation* _aniBossFistShoot;				//슬라임이 주먹발사하는것
	animation* _aniBossHandShootStart;			//골렘 손 위로 던지기
	animation* _aniBossHandShootEnd;			//골렘 손 받기
	animation* _aniIdle;						//평소 골렘업뎃
	animation* _scroll;							//스크롤

	
	progressBar* _hpBar;						//렌더할 hp
	progressBar* _hpRed;						//발깡
	progressBar* _hpWhite;						//흰색

	int _hp;									//체력	
	int _attackCool;							//공격 쿨타임

	tagGolemHand _golemHand;					//손으로 찍는공격

	int _rockFireTime;							//일정주기로 돌을 떨어뜨려라
	int _rockFireCount;							//몇번 떨궜냐?
	vector<tagRock> _vRock;						//돌들

	float _rockShootAngle[3];						//슛할 각도들

public:
	bossGolemKing() {}
	~bossGolemKing() {}

	HRESULT init(int x, int y);
	void release();
	void update();
	void render();

	void bossAtkUpdate();

	void initGolemAttack();
	void suffleGolemAttack();
	void initGolemHand();

	float getX() { return _x; }
	float getY() { return _y; }

	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
};

