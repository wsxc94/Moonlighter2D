#pragma once
#include "singletonBase.h"
#include "itemManager.h"

class animation;
class DungeonMap;

//결과창에서 보여줄 에너미 공격애니메이션
typedef struct tagResultEnemyAnimation
{
	animation* attack;
	int frameY;
	float scale;
}RESULTENEMY;

class playerData : public singletonBase <playerData>
{
private:
	int _defaultHp;				//플레이어 디폴트 체력 
	int _defaultAtk;			//플레이어 디폴트 공격력 
	int _defaultDef;			//플레이어 디폴트 방어력 
	int _defaultSpd;			//플레이어 디폴트 스피드 

	int _hp;					//장비 능력값 반영된 체력 
	int _atk;					//장비 능력값 반영된 공격력  
	int _def;					//장비 능력값 반영된 방어력 
	int _spd;					//장비 능력값 반영된 스피드 

	int _inDungeonHp;			//던전에서 사용하는 체력 
	int _hpBarLength;			//체력바의 총 길이 

	int _gold;					//소지금 
	int _itemCount;				//현재 소지하고 있는 아이템 개수 
	int _weaponIdx;				//장착 설정이 되어있는 무기의 인덱스(1 or 2)
	float _x, _y;

	bool _isInDungeon;			//플레이어가 던전에 있는지 여부 확인
	bool _isActivate;			//UI 렌더할건지

	DungeonMap* _mapData;		//엠블렘으로 집을오면 던전맵데이터를 저장한다
	int _dgFloor;				//몇층이었는지
	bool _isEmblemReturn;		//엠블렘리턴인지 확인
	bool _isPendantReturn;		//팬던트리턴 / 죽음인지 확인
private:
	vector<RESULTENEMY> _vEnemy;		//결과창 에너미
	RESULTENEMY* _killEnemy;				//플레이어 죽인에너미
private:
	gameItem _potionEquipped;	//현재 장착 중인 포션 아이템 
	gameItem _weaponEquipped;	//현재 장착 중인 무기 아이템 

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	void initPlayerStatus();		//플레이어 스탯값 초기화 
	void initDungeonHp();			//던전 HP값 초기화 

	//get함수 
	const int getDefaultHp() { return _defaultHp; }
	const int getDefaultAtk() { return _defaultAtk; }
	const int getDefaultDef() { return _defaultDef; }
	const int getDefaultSpd() { return _defaultSpd; }
	int getHp() { return _hp; }
	int getAtk() { return _atk; }
	int getDef() { return _def; }
	int getSpd() { return _spd; }
	int getInDungeonHp() { return _inDungeonHp; }
	int getGold() { return _gold; }
	int getItemCount() { return _itemCount; }
	float getX() { return _x; }
	float getY() { return _y; }
	bool getIsInDungeon() { return _isInDungeon; }
	bool getIsActivate() { return _isActivate; }
	vector<RESULTENEMY> getVEnemy() { return _vEnemy; }
	void pushVEnemy(RESULTENEMY enemy) { _vEnemy.push_back(enemy); }
	void vEnemyClear() { _vEnemy.clear(); }
	RESULTENEMY* getKillEnemy() { return _killEnemy; }
	void setKillEnemy(RESULTENEMY* em) { _killEnemy = em; }
	bool isKillenemy() {
		if (_killEnemy) return true;
		return false;
	} // 킬에너미가 존재하냐?

	DungeonMap* getMapData() { return _mapData; }
	bool getIsEmblemReturn() { return _isEmblemReturn; }
	bool getIsPendantReturn() { return _isPendantReturn; }
	int getDungeonFloor() { return _dgFloor; }

	//set함수 
	void setHp(int hp) { _hp = hp; }
	void setAtk(int atk) { _atk = atk; }
	void setDef(int def) { _def = def; }
	void setSpd(int spd) { _spd = spd; }
	void setInDungeonHp(int hp) { _inDungeonHp = hp; }
	void setGold(int gold) { _gold = gold; }
	void setItemCount(int itemCount) { _itemCount = itemCount; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
	void setIsInDungeon(bool value) { _isInDungeon = value; }
	void setPotionEquipped(gameItem item) { _potionEquipped = item; }
	void setWeaponIdx(int index) { _weaponIdx = index; }
	void setWeaponEquipped(gameItem item) { _weaponEquipped = item; }
	void setIsActivate(bool isActivate) { _isActivate = isActivate; }

	void plusInDungeonHp(int plusHp);	//매개변수 값만큼 던전체력 더하기 
	void minusInDungeonHp(int minusHp);	//매개변수 값만큼 던전체력 빼기 
	void addGold(int addValue) { _gold += addValue; }	//매개변수 값만큼 골드 더하기 
	void subGold(int subValue) { _gold -= subValue; if (_gold < 0) _gold = 0; }	//매개변수 값만큼 골드 빼기 
	void saveDungeonMap(DungeonMap* data) { _mapData = data; }
	void setIsEmblemReturn(bool value) { _isEmblemReturn = value; }
	void setIsPendantReturn(bool value) { _isPendantReturn = value; }
	void setDungeonFloor(int floor) { _dgFloor = floor; }

	//인터페이스 렌더함수 
	void goldRender(HDC hdc);					//현재 소지하고 있는 골드 출력 
	void goldCountRender(HDC hdc);				//현재 소지하고 있는 골드 카운트 출력 
	void hpRender(HDC hdc);						//플레이어 체력 출력 
	void hpGaugeRender(HDC hdc);				//플레이어 현재 체력(게이지) 출력 
	void hpCountRender(HDC hdc, int currentHp);	//플레이어 체력 카운트 출력 
	void equipInfoRender(HDC hdc);				//화면 우측의 장착정보 출력 
	void itemCountRender(HDC hdc);				//아이템 소지 개수 및 최대 소지 개수 출력 
	void weaponEquippedRender(HDC hdc);			//현재 장착중인 무기 출력 
	void potionEquippedRender(HDC hdc);			//현재 장착중인 포션 출력 
};

