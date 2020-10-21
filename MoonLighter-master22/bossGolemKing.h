#pragma once
#include "boss.h"
#include "progressBar.h"

#define ROCKDAMAGE 7
#define FISTDAMAGE 15
#define HANDDAMAGE 10

enum class GOLEMKINGSTATE
{
	BS_INIT,BS_IDLE,BS_FIST,BS_ROCK_SHOOT,BS_ROCK_ROUND,BS_HAND,BS_DEAD
};

enum class GOLEMANISTATE
{
	ANI_BOSSUP,ANI_DEAD1,ANI_DEAD2,ANI_FISTSHOOT1, ANI_FISTSHOOT2, ANI_FISTSHOOT3, ANI_HANDSHOOTSTART,ANI_HANDSHOOTEND,ANI_IDLE
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
	int atkCount;
	bool isHit;
	HANDSTATE state;
};

struct tagRock
{
	image* img;
	RECT rc;
	RECT colRC;
	float x, y;
	float hight;
	float shadowScale;
	int time;				//유지시간
	bool isHit;
};

struct tagFist
{
	animation* ani;
	POINT pt[4];
	int wid;
	int hei;
	int count;
	float x;
	float y;
	float angle;
	bool isMoveLeft;
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
	animation* _aniBossFistShoot1;				//슬라임이 주먹발사하는것
	animation* _aniBossFistShoot2;
	animation* _aniBossFistShoot3;
	animation* _aniBossHandShootStart;			//골렘 손 위로 던지기
	animation* _aniBossHandShootEnd;			//골렘 손 받기
	animation* _aniIdle;						//평소 골렘업뎃
	animation* _scroll;							//스크롤

	
	progressBar* _hpBar;						//렌더할 hp
	progressBar* _hpRed;						//발깡
	progressBar* _hpWhite;						//흰색

	int _hp;									//체력	
	int _attackCool;							//공격 쿨타임
	int _hitCount;

	tagGolemHand _golemHand;					//손으로 찍는공격

	int _rockFireTime;							//일정주기로 돌을 떨어뜨려라
	int _rockFireCount;							//몇번 떨궜냐?
	vector<tagRock> _vRock;						//돌들

	float _rockShootAngle[3];						//슛할 각도들

	tagFist _bossFist;							//보스 주먹질

	bool _isAttackSoundPlay;					//공격전 사운드
	bool _isDeadSoundPlay;						//죽는 사운드
	bool _isDead;								//죽었냐?
	bool _isHit;
	bool _coliSkillAroow;

public:
	bossGolemKing() {}
	~bossGolemKing() {}

	HRESULT init(int x, int y);
	void release();
	void update();
	void render();
	void changeAniState(GOLEMANISTATE state);				//애니메이션 변경
	void initAttack();										//공격하기전 상태별 초기화

	void collisionPlayer();
	void hitUpdate();

	void initVGolemAttack();								//벡터에 공격패턴 넣기
	void suffleVGolemAttack();								//랜덤하게 셔플하기
	void initGolemHand();									//손공격 이닛

	void bsFistUpdate();
	void bsRockShootUpdate();
	void bsRockRoundUpdate();
	void bsHandUpdate();
	void vRockUpdate();
	void collisionRock();


	float getX() { return _x; }
	float getY() { return _y; }
	bool getIsDead() { return _isDead; }

	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
};

