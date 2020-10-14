#pragma once
#include "singletonBase.h"
#include "arrow.h"
#include "animation.h"

enum PLACE
{
	SHOP,
	TOWN_DUNGEON
};
enum PLAYER_STATE
{
	PLAYER_IDLE,
	PLAYER_RUN,
	PLAYER_ROLL,
	PLAYER_ATTACK_SWORD, PLAYER_ATTACK_SWORD_SECOND, PLAYER_ATTACK_BOW,
	PLAYER_TALK,
	PLAYER_DIE,
	PLAYER_FALL,
	PLAYER_SWIM,
	HIT_IDLE,
	HIT_RUN,
	HIT_SWORD,
	HIT_SWORD_TWO
};

enum WEAPONSTATE
{
	EMPTY,
	SHORT_SOWRD,
	BOW
};

struct tagPlayer
{
	RECT rc;
	RECT shadowRc;
	float x;
	float y;
	int direction;
	WEAPONSTATE weapon;
	int count;
	int index;
};

struct tagPlayerAttackBox
{
	RECT rc;
	bool isHit;
};

struct tagDamegeFont
{
	int damage;
	bool iaActivate;
};

class player : public singletonBase <player>
{
private:
	vector<tagDamegeFont*> _damageFont;
private:
	arrow* _arrow;

	PLAYER_STATE _state;
	PLACE _place;

	tagPlayerAttackBox _playerAttackBox;
	tagPlayerAttackBox _playerAttackTwoBox;

	bool _right;
	bool _left;
	bool _up;
	bool _down;
	bool _isShoot;
	bool _isTalk;
	bool _isHit;

	int _frameX;
	int _frameY;

	tagPlayer _player;

	int _rollCount;
	int _rollIndex;

	int _lastRollX;			// 구르기전 x
	int _lastRollY;			// 구르기전 y
	int	_holeAlpha;			// 구르기 알파값

	int _attackCount;
	int _attackIndex;

	int _playerHp;
	int _hitAlpha;

	float _rollJumpPower;
	float _rollGravity;
private:

	animation* _aniTownIdle;
	animation* _aniTownRun;
	animation* _aniTownRoll;
	animation* _aniDgIdle;
	animation* _aniDgRun;
	animation* _aniDgRoll;
	animation* _aniFall;
	animation* _aniSword;
	animation* _aniSwordTwo;
	animation* _aniBow;
	animation* _aniDie;
	animation* _aniSwim;

	animation* _aniHit;
	animation* _aniRunHit;
	animation* _aniSwordHit;
	animation* _aniSwordTwoHit;

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	//인벤토리 관련 함수 
	int getMaxItemSlot();

	void animation(int frameY);
	void playerState();
	void attackRCUpdate();
	void updateWeaponState();	//플레이어의 무기 장착 상태 업데이트 
	void keyInput();		//플레이어의 상태에 상관없이 키 입력을 받는 함수 
	void npcTalk(bool& isTalk);
	void hitPlayer();

	float getX() { return _player.x; }
	float getY() { return _player.y; }

	RECT& getRect() { return _player.rc; }
	RECT getShadowRect() { return _player.shadowRc; }
	
	arrow *getArrow() { return _arrow; }

	int getPlayerDirection() { return _player.direction; }

	bool& getRight() { return _right; }
	bool& getLeft() { return _left; }
	bool& getUp() { return _up; }
	bool& getDown() { return _down; }
	bool getShoot() { return _isShoot; }

	WEAPONSTATE getWeaponState() { return _player.weapon; }

	PLAYER_STATE getPlayerState() { return _state; }

	void setPlayerState(PLAYER_STATE state) { _state = state; }

	void setX(float x) { _player.x = x; }
	void setY(float y) { _player.y = y; }
	void setRollPower(float p) { _rollJumpPower = p; }

	void setShoot(bool isShoot) { _isShoot = isShoot; }

	tagPlayerAttackBox& getPlayerAttackBox() { return _playerAttackBox; }
	void setPlayerAttackBoxHit(bool hit) { _playerAttackBox.isHit = hit; }

	tagPlayerAttackBox& getPlayerAttackTwoBox() { return _playerAttackTwoBox; }
	void setPlayerAttackTwoBoxHit(bool hit) { _playerAttackTwoBox.isHit = hit; }

};

