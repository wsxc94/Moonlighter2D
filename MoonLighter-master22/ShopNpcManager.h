#pragma once
#include "gameNode.h"
#include "PotionShopNpc.h"

class displayStand;

// ������ npc �Ŵ���

class ShopNpcManager : public gameNode
{
private:

	int _npcCnt;

	tagPosF _pos;

	vector<npc*> v_npcs;

	shared_ptr<displayStand> _displayStand;

	int _npcTime;

	vector<pair<string, string>> _npcName = {
		make_pair("����" , "�賶�ǿ���"),
		make_pair("���" , "����������"),
		make_pair("����յ�" , "���ϰ�����"),
		make_pair("��" , "�����")
	};

public:

	ShopNpcManager() {};
	~ShopNpcManager();

	HRESULT init(shared_ptr<displayStand> dis);
	void release();
	void update();
	void render();
	void getInit(int idx , int rnd); // �ε����� �޾ƿͼ� npc�� �ʱ�ȭ�ϴ� �Լ�
	void UseNpcCheck(); // ���� �ε����� npc�� ��������� �˻��ϴ� �Լ�

	vector<npc*>& getVector() { return v_npcs; }
	vector<pair<string, string>>& getNpcNamePair() { return _npcName; }
};

