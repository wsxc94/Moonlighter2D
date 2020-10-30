#pragma once
#include "gameNode.h"
#include "itemManager.h"
#define MAX_NPC 4

class displayStand;

enum NPC_ACTION // npc 상태 스테이트
{
	NPC_START, //대기상태
	NPC_MOVE,  //움직임
	NPC_STOP,  //멈춤
	NPC_CHECK_PRICE, //가격 체크
	NPC_ITEM_PICK, // 아이템선택
	NPC_WAIT,  // 결제 대기
	NPC_GO_HOME, // 돌아가기
	NPC_TALK // 플레이어와 대화
};


enum NPC_MAP // npc가 어디있는지 기본 = 마을
{
	NPC_TOWN,
	NPC_SHOP
};

class npc : public gameNode
{
protected:
	unique_ptr<animation> _aniNpc; // npc 애니메이션 변수
	unique_ptr<animation> _aniPriceCheck; // 이모티콘 애니메이션 변수
	image* _peekItemImg; // 현재 잡고 있는 아이템 이미지

	int _peekItemGold;
	int _peekItemCnt;
	string thinkInfo; // 엄청싸다 싸다 비싸다 엄청비싸다
protected:
	int _count;
	int _idx;
	int _time;
	int rndStopTime;

	int _spawnTime; // npc 젠 타임
	float _speed;   // npc 이동속도
	float _angle;   // npc 가 바라볼 각도

	string _key;    // 이미지 키 이름

	tagPosF _pos;   // x y 축 구조체

	tagPosF _target[4]; // 마을 npc 움직일 좌표

	vector<vector<tagPosF>> shop_target; // 타겟 좌표 배열

	int _targetIdx; // 좌표 타겟 인덱스

	int shop_targetIdx; // 좌판 번호 타겟 인덱스
	int shop_currentTargetIdx; // 현재 좌표 위치정보 인덱스

	RECT _rc;  // npc 렉트
	RECT _talkRect; // 말걸수 있는 범위 렉트
	bool _stop; // 멈췄는가
	bool _Istalk; // 대화중인가
	bool _delay; // 시간 딜레이 변수

	bool _isActive; // 활성화 상태
	bool _isSpawn;  // 젠 활성화 상태

	int _boxidx; //박스 애니메이션 인덱스
	int _boxCnt; //박스 애니메이션 시간

	string _talk; // 멘트
	string _name; // npc 이름

	HFONT hFont;  
	HFONT oldFont;

	RECT _nameRect; // 이름 drawtext용 렉트
	RECT _textRect; // 멘트 drawtext용 렉트

	displayStand* _displayStand; // 상점 좌판 정보
	gameItem _itemEmpty;		//비어있는 아이템 

	string _illustrator = "일러"; // 이름 + 일러 = 일러스트 이미지

	NPC_ACTION _state; // npc 상태

	bool _isBarking; // 강아지가 짖고 있는가

	void setshopTargetPos(); // 상점에서 타겟 좌표 설정


public:
    virtual HRESULT init(tagPosF pos, string key); // 마을 npc 초기화
	HRESULT init(tagPosF pos, string key, NPC_MAP NPC_SHOP, int idx, displayStand* dis);  //상점 npc 초기화

	void release();
	virtual void update(); // 마을 NPC 업데이트
	void update(NPC_MAP NPC_SHOP); // 상점NPC 업데이트

	virtual void render(); // 기본 npc 렌더링
	void render(NPC_MAP NPC_SHOP); // 상점 npc 렌더링

	void anim(); // npc 애니메이션
	void boxAnim(); // 대화박스 애니메이션

	void move(); // 마을 npc 움직임
	void move(NPC_MAP NPC_SHOP); // 상점에서의 npc 움직임

	void DistanceCheck();
	virtual void action(string talk); //NPC 멘트 및 말풍선 업데이트
	void action(); //강아지 전용
	void collision(); // NPC 충돌렉트 설정
	void lookPlayer(); // 플레이어를 바라보도록 조정 강아지전용
	void npcSpawn(); // npc 스폰

	tagPosF& getPos() { return _pos; } // npc x , y 좌표 가져오기
	RECT& getRect() { return _rc; } // npc 렉트 가져오기
	string getKey() { return _key; } // npc 이미지 KEY값 가져오기

	void setPosX(float x) { _pos.x = x; } // x 좌표 세팅
	void setPosY(float y) { _pos.x = y; } // y 좌표 세팅

	bool& getActive() { return _isActive; } // npc가 살아있는가
	void setActive(bool ac) { _isActive = ac; } // npc 생존여부 세팅

	int& getCurrentTargetIdx() { return shop_currentTargetIdx; } // 현재 가야할 좌표 인덱스
	void setCurrentTargetIdxPlus() { shop_currentTargetIdx++; }
	void priceCheck(); // 상점 아이템 가격에 따른 세팅
	void PriceCheckAnim(); // 이모티콘 애니메이션
	void ItemGet(); // 아이템 정보 가져오기
	void ItemActive();
	void ShadowPosRender(); //그림자 렌더
	void ShadowPosSet(int x, int y, int div); //npc 그림자 위치잡아주는 함수

	void TalkInterfaceRender(); //대화에 대한 인터페이스 렌더

	int& getPeekItemGold() { return _peekItemGold; } // 현재 가지고 있는 아이템 가격
	int& getPeekItemCnt() { return _peekItemCnt; }   // 현재 가지고 있는 아이템 개수

	NPC_ACTION& getState() { return _state; } // npc 현재 스테이트 반환
	void setState(NPC_ACTION st) { _state = st; } // npc 상태 업데이트
	string& getThinkInfo() { return thinkInfo; } // 싼지 안싼지 상태에 대한 문자열 반환

	npc() {}
	~npc() {}
};

