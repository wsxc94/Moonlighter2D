#pragma once
#include "gameNode.h"
#include "itemManager.h"
#define MAX_NPC 4

class displayStand;

enum NPC_ACTION // npc ���� ������Ʈ
{
	NPC_START, //������
	NPC_MOVE,  //������
	NPC_STOP,  //����
	NPC_CHECK_PRICE, //���� üũ
	NPC_ITEM_PICK, // �����ۼ���
	NPC_WAIT,  // ���� ���
	NPC_GO_HOME, // ���ư���
	NPC_TALK // �÷��̾�� ��ȭ
};


enum NPC_MAP // npc�� ����ִ��� �⺻ = ����
{
	NPC_TOWN,
	NPC_SHOP
};

class npc : public gameNode
{
protected:
	unique_ptr<animation> _aniNpc; // npc �ִϸ��̼� ����
	shared_ptr<animation> _aniPriceCheck; // �̸�Ƽ�� �ִϸ��̼� ����

	image* _peekItemImg; // ���� ��� �ִ� ������ �̹���

	int _peekItemGold;
	int _peekItemCnt;
	string thinkInfo; // ��û�δ� �δ� ��δ� ��û��δ�
protected:
	int _count;
	int _idx;
	int _time;
	int rndStopTime;

	int _spawnTime; // npc �� Ÿ��
	float _speed;   // npc �̵��ӵ�
	float _angle;   // npc �� �ٶ� ����

	string _key;    // �̹��� Ű �̸�

	tagPosF _pos;   // x y �� ����ü

	tagPosF _target[4]; // ���� npc ������ ��ǥ

	vector<vector<tagPosF>> shop_target; // Ÿ�� ��ǥ �迭

	int _targetIdx; // ��ǥ Ÿ�� �ε���

	int shop_targetIdx; // ���� ��ȣ Ÿ�� �ε���
	int shop_currentTargetIdx; // ���� ��ǥ ��ġ���� �ε���

	RECT _rc;  // npc ��Ʈ
	RECT _talkRect; // ���ɼ� �ִ� ���� ��Ʈ
	bool _stop; // ����°�
	bool _Istalk; // ��ȭ���ΰ�
	bool _delay; // �ð� ������ ����

	bool _isActive; // Ȱ��ȭ ����
	bool _isSpawn;  // �� Ȱ��ȭ ����

	int _boxidx; //�ڽ� �ִϸ��̼� �ε���
	int _boxCnt; //�ڽ� �ִϸ��̼� �ð�

	string _talk; // ��Ʈ
	string _name; // npc �̸�

	HFONT hFont;  
	HFONT oldFont;

	RECT _nameRect; // �̸� drawtext�� ��Ʈ
	RECT _textRect; // ��Ʈ drawtext�� ��Ʈ

	shared_ptr<displayStand> _displayStand; // ���� ���� ����
	gameItem _itemEmpty;		//����ִ� ������ 

	string _illustrator = "�Ϸ�"; // �̸� + �Ϸ� = �Ϸ���Ʈ �̹���

	NPC_ACTION _state; // npc ����

	bool _isBarking; // �������� ¢�� �ִ°�

	void setshopTargetPos(); // �������� Ÿ�� ��ǥ ����


public:
    virtual HRESULT init(tagPosF pos, string key); // ���� npc �ʱ�ȭ
	HRESULT init(tagPosF pos, string key, NPC_MAP NPC_SHOP, int idx, shared_ptr<displayStand> dis);  //���� npc �ʱ�ȭ

	void release();
	virtual void update(); // ���� NPC ������Ʈ
	void update(NPC_MAP NPC_SHOP); // ����NPC ������Ʈ

	virtual void render(); // �⺻ npc ������
	void render(NPC_MAP NPC_SHOP); // ���� npc ������

	void anim(); // npc �ִϸ��̼�
	void boxAnim(); // ��ȭ�ڽ� �ִϸ��̼�

	void move(); // ���� npc ������
	void move(NPC_MAP NPC_SHOP); // ���������� npc ������

	void DistanceCheck();
	virtual void action(string talk); //NPC ��Ʈ �� ��ǳ�� ������Ʈ
	void action(); //������ ����
	void collision(); // NPC �浹��Ʈ ����
	void lookPlayer(); // �÷��̾ �ٶ󺸵��� ���� ����������
	void npcSpawn(); // npc ����

	tagPosF& getPos() { return _pos; } // npc x , y ��ǥ ��������
	RECT& getRect() { return _rc; } // npc ��Ʈ ��������
	string& getKey() { return _key; } // npc �̹��� KEY�� ��������

	void setPosX(float x) { _pos.x = x; } // x ��ǥ ����
	void setPosY(float y) { _pos.x = y; } // y ��ǥ ����

	bool& getActive() { return _isActive; } // npc�� ����ִ°�
	void setActive(bool ac) { _isActive = ac; } // npc �������� ����

	int& getCurrentTargetIdx() { return shop_currentTargetIdx; } // ���� ������ ��ǥ �ε���
	void setCurrentTargetIdxPlus() { shop_currentTargetIdx++; }
	void setCurrentTargetIdxMinus() { shop_currentTargetIdx--; }
	void setCurrentTargetIdx(int idx) { shop_currentTargetIdx = idx; }
	void priceCheck(); // ���� ������ ���ݿ� ���� ����
	void PriceCheckAnim(); // �̸�Ƽ�� �ִϸ��̼�
	void ItemGet(); // ������ ���� ��������
	void ItemActive();
	void ShadowPosRender(); //�׸��� ����
	void ShadowPosSet(int x, int y, int div); //npc �׸��� ��ġ����ִ� �Լ�

	void TalkInterfaceRender(); //��ȭ�� ���� �������̽� ����

	int& getPeekItemGold() { return _peekItemGold; } // ���� ������ �ִ� ������ ����
	int& getPeekItemCnt() { return _peekItemCnt; }   // ���� ������ �ִ� ������ ����

	NPC_ACTION& getState() { return _state; } // npc ���� ������Ʈ ��ȯ
	void setState(NPC_ACTION st) { _state = st; } // npc ���� ������Ʈ
	string& getThinkInfo() { return thinkInfo; } // ���� �Ƚ��� ���¿� ���� ���ڿ� ��ȯ

	shared_ptr<animation>& getAniPriceCheck() { return _aniPriceCheck; } // �̸�Ƽ�� �ִϸ��̼� ��ȯ�Լ�

	void setDelay(bool b) { _delay = b; }
	npc() {}
	virtual ~npc();
};

