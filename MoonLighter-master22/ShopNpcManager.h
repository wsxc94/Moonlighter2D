#pragma once
#include "gameNode.h"
#include "PotionShopNpc.h"

class displayStand;

// 상점용 npc 매니저

class ShopNpcManager : public gameNode
{
private:

	int _npcCnt;

	tagPosF _pos;

	vector<npc*> v_npcs;

	displayStand* _displayStand;

	int _npcTime;

	vector<pair<string, string>> _npcName = {
		make_pair("마사" , "배낭맨여자"),
		make_pair("토모" , "원형아저씨"),
		make_pair("레드먼드" , "도둑강아지"),
		make_pair("모리" , "히어로")
	};

public:
	HRESULT init(displayStand* dis);
	void release();
	void update();
	void render();
	void getInit(int idx , int rnd); // 인덱스를 받아와서 npc를 초기화하는 함수
	void UseNpcCheck(); // 현재 인덱스의 npc가 사용중인지 검사하는 함수

	vector<npc*>& getVector() { return v_npcs; }
	vector<pair<string, string>>& getNpcNamePair() { return _npcName; }

	ShopNpcManager(){}
	~ShopNpcManager(){}
};

