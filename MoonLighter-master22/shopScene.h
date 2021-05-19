#pragma once
#include "gameNode.h"
#include "ShopNpcManager.h"
#include "displayStand.h"

#define ITEMDESKCOUNT 4 //���� ����

class shopScene : public gameNode
{
private:
	ShopNpcManager* _npc; // npc�Ŵ���
	displayStand* _displayStand; //������ ���� ����

	RECT GoTownPortal; // �����ΰ��� ��Ż

	bool _visit; //�÷��̾� �湮����
	bool _disMenuOn; // ���Ǹ޴��� �����ֳ� �������ֳ�
	int _doorTime;
	vector <pair<int, int>> v_itemPos; //��������ġ
	vector <pair<int, int>> v_itemShadowPos;//�����۱׸�����ġ

	vector < pair<float, float>> v_itemMoveSpeed; // ������ ������ �ӵ�, ���ӵ� ����
	
	vector<bool> b; // ������ ������ true false 

	RECT _desk; // ����
	RECT _stand; // ����
	RECT _doorRect; // ������ �ݴ� ��Ʈ

	bool _shopDoorCheck; 
	bool _doorOpen;
	void ItemPosSet(); // ������ ��ġ ����
	bool _isShopOpen;

	RECT _itemText[4]; // ������ ��ο� �ؽ�Ʈ ��Ʈ
	HFONT hFont; // ������ ���� ��Ʈ
	HFONT oldFont; // ������ ���� ��Ʈ


	unique_ptr<animation> _cashRegister; // å�� �� �ݰ�
	unique_ptr<animation> _button; // ���� ��ġ ��ư
	unique_ptr<animation> _door; // ��
	unique_ptr<animation> _cauldron; //�ܴ���
	unique_ptr<animation> _sellButton; // �Ǹ� ��ư

	bool _sellNpcCheck;
	bool _pixelBackGround;


public:
	HRESULT init();
	void release();
	void update();
	void render();

	void PortaltoTown(); // ������ ��Ż
	void PlayerCol(); // �÷��̾� �ȼ��浹 �Լ� 
	void PlayerSell(); // ������ �Ĵ� �Լ�
	void itemInfoUpdate(); // ������ ������������ �˻��� npc�� �ʱ�ȭ��Ű�� �Լ�

	void npcAI(); // npc update ����� ����
	void npcInit(int idx); //npc �ʱ�ȭ�Լ�
	void doorOpen(); //������ ���� �Լ�
	void doorPlay(); //�� �ִϸ��̼� ���� ON �Լ�
	void doorReverseplay(); // �� �ִϸ��̼� �ݴ��

	void sellStandAction(); // ���� �׼�
	void sellDeskAction(); // ���� �׼�

	void itemMove(); // ���ǿ� ��ġ�� ������ ������ �Լ�

	void SoundUpdate(); // ���� ������Ʈ

	void backGroundRender(); // ��� �� ���� �����Լ�
	void InterfaceRender(); //��ư �� �������̽� , ������ ���� �Լ�
};

