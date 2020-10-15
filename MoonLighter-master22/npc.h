#pragma once
#include "gameNode.h"
#define MAX_NPC 4

class displayStand;
enum NPC_ACTION
{
	NPC_MOVE,
	NPC_STOP,
	NPC_CHECK_PRICE,
	NPC_WAIT,
	NPC_GO_HOME,
	NPC_ITEM_PICK
};

enum NPC_MAP
{
	NPC_SHOP
};
class npc : public gameNode
{
private:
	animation* _aniNpc;
	animation* _aniPriceCheck;
	image* _peekItemImg;
private:
	int _count;
	int _idx;
	int _time;
	float _speed;
	float _angle;
	
	string _key;

	tagPosF _pos;
	tagPosF _probe;

	tagPosF _target[4];

	vector<vector<tagPosF>> shop_target;
	int _targetIdx;

	int shop_targetIdx;
	int shop_currentTargetIdx;
	
	RECT _rc;
	RECT _talkRect;
	bool _stop;
	bool _Istalk;
	bool _delay;

	bool _isActive;
	bool _isSpawn;

	int _boxidx;
	int _boxCnt;

	string _talk;
	string _name;

	HFONT hFont;
	HFONT oldFont;

	RECT _nameRect;
	RECT _textRect;

	displayStand* _displayStand;

	string _illustrator = "일러";

	NPC_ACTION _state;

	bool _isBarking;
	void setshopTargetPos();
	
public:
	HRESULT init(tagPosF pos , string key);
	HRESULT init(tagPosF pos , string key , NPC_MAP NPC_SHOP , int idx , displayStand* dis);

	void release();
	void update();
	void update(NPC_MAP NPC_SHOP);

	void render();
	void render(NPC_MAP NPC_SHOP);

	void anim();
	void boxAnim();
	void move();
	void move(NPC_MAP NPC_SHOP);
	void action(string talk);
	void action(); //강아지 전용
	void collision();
	void lookPlayer(); // 플레이어를 바라보도록 조정 강아지전용
	void npcSpawn();

	tagPosF& getPos() { return _pos; }
	RECT& getRect() { return _rc; }
	string getKey() { return _key; }

	void setPosX(float x) { _pos.x = x; }
	void setPosY(float y) { _pos.x = y; }

	bool& getActive() { return _isActive; }
	void setActive(bool ac) { _isActive = ac; }

	int& getCurrentTargetIdx() { return shop_currentTargetIdx; }
	void priceCheck();
	void PriceCheckAnim();
	void ItemGet();

	npc(){}
	~npc() {}
};

