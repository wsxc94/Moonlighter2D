#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "progressBar.h"
#include "itemManager.h"

enum ENEMYKIND			// 종류
{
	EM_GOLEM, EM_REDGOLEM, EM_POTGOLEM, EM_SKULLHAMMER, EM_BABYSLIME , EM_BOSS_SKELETON
};

enum ENEMYSTATE			// 상태
{
	EM_MOVE, EM_ATTACK, EM_DIE, EM_HIT
};

enum ENEMYDIRECTION		//보고있는 방향
{
	EM_TOP, EM_BOTTOM, EM_LEFT, EM_RIGHT
};

enum NODESTATE
{
	NODE_START,
	NODE_END,
	NODE_WALL,
	NODE_EMPTY
};

enum WALLSTATE
{
	WALL_OBJ,WALL_HOLE,WALL_EMPTY
};

struct node
{
	RECT rc;
	int idx, idy;
	int centerX, centerY;
	int F, G, H;
	bool isRange;
	node* parentNode;
	NODESTATE nodeState;
	WALLSTATE wallState;

	void init(int _idx, int _idy)
	{
		idx = _idx;
		idy = _idy;
		rc = RectMake(140 + (_idx * 35), 105 + (_idy * 35), 35, 35);
		centerX = (rc.left + rc.right) / 2;
		centerY = (rc.top + rc.bottom) / 2;
		F = G = H = 0;
		isRange = false;
		nodeState = NODE_EMPTY;
		wallState = WALL_EMPTY;
		parentNode = NULL;
	}
};

class enemy : public gameNode
{
protected:
	ENEMYKIND _emKind;
	ENEMYSTATE _emState;
	ENEMYDIRECTION _emDirection;
	image* _enemyImg;

	progressBar* _hpBar;

	bool _isHpBarRender;
	int _hpBarRenterTime;
	int _hpBarAlpha;

	RECT _emRC;
	RECT _aStarRC;
	RECT _attackRange;
	bool _isActivate;
	bool _isAttackRange;
	bool _isHit;
	bool _isItemDrop;

	bool _coliSkillAroow;

	float _x;
	float _y;

	int _emHp;
	int _emAtk;
	// 아이템
	int* _itemIndex;			//아이템 인덱스
	int* _itemNum;				//아이템 개수
	int	 _itemIndexSize;		//아이템인덱스 사이즈
	int  _itemDropSize;			//아이템드롭될개수


	bool _emPlayerColi;	//충돌용 함수
	int count;			//충돌용 함수

	//에너미 에이스타 용
	int _idx;						//타일 사이즈
	int _idy;						//타일 사이즈
	node** _totalNode;				//동적할당으로 이차원배열을 초기화하기위해 이중포인터이다
	node* _startNode;
	node* _endNode;
	node* _curNode;
	vector<node*> _openList;
	vector<node*> _closeList;
	vector<node*> _finalList;
	vector<node*> _rangeList;

	int _count;
	bool _isFind;
public:
	enemy() {}
	virtual ~enemy() {}

	virtual HRESULT init(int x, int y);
	virtual void release();
	virtual void update();
	virtual void render();
	virtual void initTileSize(int x, int y);
	virtual void setEndTile(int x, int y);
	virtual void setWallTile(vector<tagTile> vTile);
	virtual void aStar();
	virtual void addOpenList(int idx, int idy);
	virtual void delOpenList(int arr);
	virtual void isAttackRange(RECT rc);
	virtual bool isAstarFail();
	virtual void setRange(int x, int y);
	virtual void collision();
	virtual void soundUpdate();
	void setProgressBar();
	virtual void hitSoundPlay();
	virtual bool checkDirection();	//적이공격하는 방향검사
	virtual void allEnemyColi(int emDemage, bool checkDirection);	//모든적과 플레이어 충돌 검사
	void aStarRender();
	
public:
	bool getIsActivate() { return _isActivate; }
	void setIsActivate(bool at) { _isActivate = at; }
	int getEmHp() { return _emHp; }
	void setEmHp(int hp) { _emHp = hp; }
	int getEmAtk() { return _emAtk; }
	RECT getEmRC() { return _emRC; }
	int* getDropItemIndex() { return _itemNum; }
	int getDropItemSize() { return _itemDropSize; }
	float getX() { return _x; }
	float getY() { return _y; }
	bool getIsItemDrop() { return _isItemDrop; }
	void setIsItemDrop(bool drop) { _isItemDrop = drop; }
	virtual animation* getAttackAnimation() { return nullptr; };
	virtual int getDownDirectionY() { return 0; };
	virtual float getEnemyScale() { return 1.f; }
};

class redGolem : public enemy
{
private:

	animation* _move;
	animation* _attack;
	animation* _moveHit;
	animation* _attackHit;
	animation* _die;

	RECT _attackBox;

	bool _emPlayerColi;	//충돌용 함수
	int count;			//충돌용 함수

	bool _isAttackSoundPlay;
	bool _isDieSoundPlay;
	float _knockBackSpeed;
public:
	redGolem() {}
	virtual ~redGolem() {}

	virtual HRESULT init(int x, int y);
	virtual void release();
	virtual void update();
	virtual void render();
	
	void directionUpdate();
	void moveRender();
	void attackRender();
	void attackAniPlay();
	bool attackAniStop();
	void attackBox();
	void golemMove();
	void setStartNode();
	void soundUpdate();
	void hitSoundPlay() override;
	void dropItem();
	void redGolemCollision();
	virtual animation* getAttackAnimation();
	virtual int getDownDirectionY();

	RECT getAttackBox() { return _attackBox; }
};

class golem : public enemy
{
	// isAttackRange 에다 플레이어 렉트 넣어줄것
	// 플레이어가 때렸을시 _isHit 트루 만들어주고 hp깍아줄것
private:
	animation* _move;
	animation* _attack;
	animation* _moveHit;
	animation* _attackHit;
	animation* _die;

	RECT _attackBox;

	bool _emPlayerColi;	//충돌용 함수
	int count;			//충돌용 함수

	bool _isAttackSoundPlay;
	bool _isDieSoundPlay;
	float _knockBackSpeed;
public:
	golem() {}
	virtual ~golem() {}

	virtual HRESULT init(int x, int y);
	virtual void release();
	virtual void update();
	virtual void render();

	void directionUpdate();
	void moveRender();
	void attackRender();
	void attackAniPlay();
	bool attackAniStop();
	void attackBox();
	void golemMove();
	void setStartNode();
	void hitSoundPlay() override;
	void golemCollision();
	virtual animation* getAttackAnimation();
	virtual int getDownDirectionY();
};

class potGolem : public enemy
{
private:
	enum BULLETDIRECTION
	{
		BM_UP, BM_DOWN, BM_LEFT, BM_RIGHT
	};
	struct tagPotBullet
	{
		BULLETDIRECTION direction;
		image* img;
		RECT rc;
		float x, y;
		float angle;
		bool isFire;
		int frameY;
	};
private:
	animation* _attack;
	animation* _attackHit;
	animation* _die;

	RECT _attackRangeL;
	RECT _attackRangeR;
	RECT _attackRangeT;
	RECT _attackRangeB;

	int _hitCount;
	int _fireCount;
	bool _isFire;
	bool _isDieSoundPlay;
	bool _isFireSoundPlay;

	bool _emPlayerColi;	//충돌용 함수
	int count;			//충돌용 함수

	tagPotBullet* _bullet;
public:
	potGolem() {}
	~potGolem() {}

	HRESULT init(int x, int y);
	void release();
	void update();
	void render();
	void aniUpdate();
	void aniRender();
	void emDirectionUpdate();
	void emBulletFire();
	void emBulletMove();
	bool bulletEmpty();
	virtual void isAttackRange(RECT rc);
	virtual void setWallTile(vector<tagTile> vTile);
	void hitSoundPlay() override;
	void potGolemCollision();
	virtual animation* getAttackAnimation();
	virtual int getDownDirectionY();
};

class skullHammer : public enemy
{
private:
	animation* _move;
	animation* _attack;
	animation* _moveHit;
	animation* _attackHit;
	animation* _die;

	RECT _aStarRC;				// 에이스타용 바닥렉트
	RECT _attackBox;				// 공격범위 렉트
	float _knockBackSpeed;			//넉백스피드
	int _hitCount;					// 히트 카운트 (히트 해제용)
	bool _isDieSoundPlay;
	bool _isAttackSoundPlay;

	bool _emPlayerColi;	//충돌용 함수
	int count;			//충돌용 함수

public:
	skullHammer() {}
	~skullHammer() {}

	virtual HRESULT init(int x, int y);
	virtual void release();
	virtual void update();
	virtual void render();

	void directionUpdate();
	void moveRender();
	void attackRender();
	void attackAniPlay();
	bool attackAniStop();
	void attackBox();
	void skullMove();
	void setStartNode();
	void hitSoundPlay() override;
	void skullHammerCollision();
	
	virtual animation* getAttackAnimation();
	virtual int getDownDirectionY();
};

class gasMan : public enemy
{
private:
	int _attRange;
	enum GASARROWDIRECTION
	{
		AR_UP, AR_DOWN, AR_LEFT, AR_RIGHT
	};
	struct tagGasArrow
	{
		float x, y;
		float angle;
		float speed;
		bool isFire;
		RECT rc;
		GASARROWDIRECTION direction;
	};
private:
	animation* _move;
	animation* _moveHit;
	animation* _attack;
	animation* _attackHit;
	animation* _die;

	tagGasArrow _arrow;
	vector<tagGasArrow> _vArrow;

	float _knockBackSpeed;
	int _hitCount;
	bool _isFire;
	bool _isDieSoundPlay;
	bool _isFireSoundPlay;

	bool _emPlayerColi;	//충돌용 함수
	int count;			//충돌용 함수

public:
	gasMan() {}
	~gasMan() {}

	HRESULT init(int x, int y);
	void release();
	void update();
	void render();

	void aniRender();
	void attackRangeUpdate();
	void setEmDirection();
	void enemyMove();
	void setStartNode();
	void arrowFire();
	void arrowMove();
	void arrowRender();
	void hitSoundPlay() override;
	void gasManCollision();
	virtual animation* getAttackAnimation();
	virtual int getDownDirectionY();
};

class bossSkeleton : public enemy
{
public:
	enum SKELETONSTATE
	{
		ST_ATTACK_SWORD, ST_ATTACK_HAMMER, ST_SKILL_SWORD, ST_SKILL_HAMMER, ST_WAVE, ST_MOVE, ST_INIT, ST_DIE
	};
	enum SKELETONPHASE
	{
		ST_PHASE_1, ST_PHASE_2
	};
	struct tagAtkBox
	{
		bool isHit;
		RECT box;
	};
	struct tagBlade
	{
		animation* ani;
		float x, y;				// 센터점
		float angle;			//날아가는 각도
		int atk;
		bool isFire;			// 발사했냐?
		bool isBreak;			//부서지는 애니메이션 재생용 && 충돌기능도
	};
	SKELETONSTATE _stState;
	SKELETONPHASE _bossPhase;
private:
	animation* _move;				//움직임
	animation* _attackSword;		//칼 근접공격
	animation* _attackHammer;		//해머 근접공격
	animation* _hammerWave1;		//해머충격파
	animation* _scroll;
	tagBlade* _blade;				//블레이드

	vector<animation*> _vSkillEffect; // 스킬 충격파 이펙트 에니매이션

	RECT _swordRange;
	RECT _hammerRange;
	tagAtkBox _swordAtkBox;
	tagAtkBox _hammerAtkBox;
	tagAtkBox _hammerWaveAtkEllipse;

	int _waveCount;			//충격파 횟수
	int _waveTime;			//다음충격파
	int _emAtkHammer;		//해머 공격력
	int _emAtkSword;		//칼 공격력
	int _emAtkWave;			//충격파 공격력
	int _hitCount;			//히트카운트
	int _autoAttackCount;	//오토공격 카운트
	int _autoAttackCool;	//오토어택 쿨
	int _dieCount;			//죽었을때 흰색으로 띄울시간

	bool _isWaveHit;		//충격파 충돌했냐?

	progressBar* _hpBarRed;
	progressBar* _hpBarWhite;


	// 사운드 관련 불함수
	bool _isAttackPlay;


public:
	bossSkeleton() {}
	~bossSkeleton() {}

	HRESULT init(int x, int y);
	void release();
	void update();
	void render();
	void setStartNode();
	void enemyMove();
	void animationRender();
	void atkRangeUpdate();
	void atkBoxUpdate();
	void moveUpdate();
	void attackUpdate();
	void soundUpdate();
	void bladeUpdate();
	void hitUpdate();
	void hitToPlayer();
	void hitSoundPlay();
	void bossColi(int bossTypeDemage, tagAtkBox &bossTypeRect);		//보스 충돌처리
	void coliWaveAtk(int waveDamage);		//웨이브 충돌처리
	virtual animation* getAttackAnimation();
	virtual int getDownDirectionY();
	virtual float getEnemyScale() { return 3.f; }
};
