#include "stdafx.h"
#include "shopScene.h"

void shopScene::ItemPosSet()
{
	/*int displayX[4] = { 472 , 520 , 472 , 520 };
	int displayY[4] = { 646 , 646 , 694 , 694 };*/

	v_itemPos.push_back(make_pair(472, 646));
	v_itemPos.push_back(make_pair(520, 646));
	v_itemPos.push_back(make_pair(472, 694));
	v_itemPos.push_back(make_pair(520, 694));

	v_itemShadowPos.push_back(make_pair(486, 690));
	v_itemShadowPos.push_back(make_pair(534, 690));
	v_itemShadowPos.push_back(make_pair(486, 738));
	v_itemShadowPos.push_back(make_pair(534, 738));

	v_itemMoveSpeed.push_back(make_pair(6.5f, 0.08f));
	v_itemMoveSpeed.push_back(make_pair(6.5f, 0.08f));
	v_itemMoveSpeed.push_back(make_pair(6.5f, 0.08f));
	v_itemMoveSpeed.push_back(make_pair(6.5f, 0.08f));
	
	b.push_back(false);
	b.push_back(false);
	b.push_back(false);
	b.push_back(false);

	for(int i = 0 ; i < v_itemPos.size(); i++)
	_itemText[i] = RectMake(v_itemPos[i].first, v_itemPos[i].second , 50, 50);

}

HRESULT shopScene::init()
{
	//Ŭ���� �ʱ�ȭ
	if (ITEMMENU->getInitDisplayStand())
	{
		_displayStand->initShopInven();
	}
	else
	{
		_displayStand = new displayStand;
		_displayStand->init();
	}

	_npc = new ShopNpcManager();
	_npc->init(_displayStand);

	PLAYER->init();
	PLAYER->setX(700);
	PLAYER->setY(840);
	//PLAYER->setPlace(SHOP);

	CAMERAMANAGER->init(PLAYER->getX(), PLAYER->getY(), 2000, 1000, 0, 0, WINSIZEX / 2, WINSIZEY / 2);

	//675 870
	GoTownPortal = RectMake(670, 950, 80, 50);
	_doorRect = RectMake(670, 880, 50, 20);

	IMAGEMANAGER->addImage("temp", 2000, 1000);

	ItemPosSet(); //���ǿ� ��ġ�� ������ ��ġ �ʱ�ȭ

	_disMenuOn = false;

	_visit = false;
	_isShopOpen = false;

	_cashRegister = make_unique<animation>();
	_cashRegister->init(IMAGEMANAGER->findImage("å��ݰ�"), 0, 7);
	_cashRegister->aniStop();

	_button = make_unique<animation>(); //��÷ ��ġ ��ư
	_button->init(IMAGEMANAGER->findImage("������ġ"), 0, 7);

	_door = make_unique<animation>();
	_door->init(IMAGEMANAGER->findImage("����������"), 0, 7, false, false);
	//_door->aniStop();

	_cauldron = make_unique<animation>();
	_cauldron->init(IMAGEMANAGER->findImage("�ܴ���"), 0, 11, true, false);
	_cauldron->aniPlay();

	_sellButton = make_unique<animation>();
	_sellButton->init(IMAGEMANAGER->findImage("�ǸŹ�ư"), 0, 8);
	_sellNpcCheck = false;

	_shopDoorCheck = false;
	_doorOpen = false;
	_doorTime = 0;

	CAMERAMANAGER->FadeInit(80, FADE_IN);
	CAMERAMANAGER->FadeStart();

	_pixelBackGround = false;

	return S_OK;
}

void shopScene::release()
{
	/*_npc->release();
	SAFE_DELETE(_npc);*/
	//SAFE_DELETE(_cashRegister);
	//SAFE_DELETE(_button);
	//SAFE_DELETE(_door);

	//_displayStand->release();
	//SAFE_DELETE(_displayStand);
}

void shopScene::update()
{
	if (INPUT->GetKeyDown(VK_F1)) {
		if (_pixelBackGround)
		{
			_pixelBackGround = false;
		}
		else {
			_pixelBackGround = true;
		}
	}
	
	SoundUpdate();

	_npc->update();
	_displayStand->update();

	ITEMMENU->update();
	PLAYER->setPlace(SHOP);
	PLAYER->update();

	CAMERAMANAGER->movePivot(PLAYER->getX(), PLAYER->getY());
	CAMERAMANAGER->update(PLAYER->getX(), PLAYER->getY());

	PlayerCol();
	PortaltoTown();


	if (_isShopOpen)
	npcAI();

	PlayerSell();
	itemInfoUpdate();
	itemMove();
	doorOpen();

	_cashRegister->update();
	_button->update();
	_door->update();
	_cauldron->update();
	_sellButton->update();

}

void shopScene::render()
{
	RECT tmp;

	backGroundRender(); // ���� �� ��� ��׶��� ������Ʈ ����
	InterfaceRender(); //��ư �� �������̽� , ������ ���� �Լ�
	
	CAMERAMANAGER->ZorderTotalRender(getMemDC());

	/*POINT point = CAMERAMANAGER->getRelativeMouse(_ptMouse);
	char str[256];
	wsprintf(str, "X: %d", point.x);
	TextOut(getMemDC(), 5, 150, str, strlen(str));
	wsprintf(str, "Y: %d", point.y);
	TextOut(getMemDC(), 5, 170, str, strlen(str));*/

	PLAYER->render(getMemDC());
	ITEMMENU->render(getMemDC());
	_displayStand->render();

	//CAMERAMANAGER->Rectangle(getMemDC(), _doorRect);
}

void shopScene::PortaltoTown() // ������ ��Ż
{
	RECT tmp;
	if (IntersectRect(&tmp, &PLAYER->getShadowRect(), &GoTownPortal))
	{
		SOUNDMANAGER->stop("�������");
		SCENEMANAGER->loadScene("Ÿ��ȭ��");

		PLAYER->setX(1680);
		PLAYER->setY(450);
	}
}

void shopScene::PlayerCol() // �÷��̾� �ȼ��浹�Լ�
{
	if (INPUT->GetKey(VK_SPACE))
	{
		PLAYER->setPlayerState(PLAYER_STATE::PLAYER_IDLE);
	}

	if (GetPixel(IMAGEMANAGER->findImage("temp")->getMemDC(),
		(PLAYER->getShadowRect().left + PLAYER->getShadowRect().right) / 2, PLAYER->getShadowRect().top) == RGB(255, 0, 0) ||
		GetPixel(IMAGEMANAGER->findImage("temp")->getMemDC(),
		(PLAYER->getShadowRect().left + PLAYER->getShadowRect().right) / 2, PLAYER->getShadowRect().bottom) == RGB(255, 0, 0) ||
		GetPixel(IMAGEMANAGER->findImage("temp")->getMemDC(),
			PLAYER->getShadowRect().left, (PLAYER->getShadowRect().bottom + PLAYER->getShadowRect().top) / 2) == RGB(255, 0, 0) ||
		GetPixel(IMAGEMANAGER->findImage("temp")->getMemDC(),
			PLAYER->getShadowRect().right, (PLAYER->getShadowRect().bottom + PLAYER->getShadowRect().top) / 2) == RGB(255, 0, 0)) {

		if (PLAYER->getLeft()) {
			PLAYER->setX(PLAYER->getX() + PLAYER->getSpeed());
		}
		if (PLAYER->getRight()) {
			PLAYER->setX(PLAYER->getX() - PLAYER->getSpeed());
		}
		if (PLAYER->getUp()) {
			PLAYER->setY(PLAYER->getY() + PLAYER->getSpeed());
		}
		if (PLAYER->getDown()) {
			PLAYER->setY(PLAYER->getY() - PLAYER->getSpeed());
		}
	}
}

void shopScene::PlayerSell()
{
	RECT tmp;
	// ����å��տ��� ����ϴ� npc�鿡�Ը� ����
	//646 ,660
	_desk = RectMake(657, 660,
		IMAGEMANAGER->findImage("����å��")->getWidth(),
		IMAGEMANAGER->findImage("����å��")->getHeight()/2);

	_stand = RectMake(472, 674,
		IMAGEMANAGER->findImage("��������")->getWidth(),
		IMAGEMANAGER->findImage("��������")->getHeight());

	if (_cashRegister->getAniState() == ANIMATION_END) _cashRegister->setCurIndex(0);

	sellStandAction();
	sellDeskAction();
	if (IntersectRect(&tmp, &_stand, &PLAYER->getRect()))
	{
		_button->aniPlay();
		if (INPUT->GetKeyDown('J'))
		{
			_displayStand->openDisplayStand();
			_displayStand->setCanGrab(false);
		}
	}
	else 
	{
		_button->setCurIndex(0);
	}

}

void shopScene::itemInfoUpdate()  // ������ ������������ �˻��� npc�� �ʱ�ȭ��Ű�� �Լ�
{
	if (!_disMenuOn && PLAYER->getDisplayOn())
	{
		_disMenuOn = true;
	}
	else if (_disMenuOn && !PLAYER->getDisplayOn()){
		
		  _disMenuOn = false;

		  for (int i = 0; i < _npc->getVector().size(); i++)
		  {
			  if (_npc->getVector()[i]->getCurrentTargetIdx() == 2) {
				  _npc->getVector()[i]->setCurrentTargetIdx(2);
				  _npc->getVector()[i]->setState(NPC_MOVE);
				  _npc->getVector()[i]->setDelay(false);
			 }
		  }
	}
	for (int i = 0; i < _npc->getVector().size(); i++)
	{

		if (!_npc->getVector()[i]->getActive()){
			 
			npcInit(i);
		}
	}
}

void shopScene::npcAI() // npc ������Ʈ �Լ�
{
	for (int i = 0; i < _npc->getVector().size(); i++)
	{
		if (_disMenuOn) continue;
		if (_npc->getVector()[i]->getActive()) {
			_npc->getVector()[i]->update(NPC_SHOP);
		}
	}
}

void shopScene::npcInit(int idx) // �ε��� ��ȣ�� npc �ʱ�ȭ
{
	/*int rnd = 0;

	for (int i = 0; i < 4; i++) {
		rnd = RANDOM->range(0, 3);
		if (_npc->getNpcNamePair()[rnd].second == false) {
			break;
		}
	}*/
	
	_npc->getInit(idx, idx);
	_npc->getVector()[idx]->setActive(true);
	_npc->getVector()[idx]->setState(NPC_START);

}

void shopScene::doorOpen() // ������
{
	RECT tmp;

	_doorTime++;

	if (IntersectRect(&tmp, &_doorRect, &PLAYER->getShadowRect())) _doorOpen = true;
	
	int cnt = 0;
	for (int i = 0; i < _npc->getVector().size(); i++) {
		if ((IntersectRect(&tmp, &_doorRect, &_npc->getVector()[i]->getRect())) && _npc->getVector()[i]->getState() == NPC_MOVE)
		{	
			cnt--;
			_doorOpen = true;
		}
	}
	//if (cnt == 0 && !IntersectRect(&tmp, &_doorRect, &PLAYER->getShadowRect())) _doorOpen = false;

	if (cnt == 0 && _doorOpen && _doorTime % 160 == 0) _doorOpen = false;

	if (_doorOpen && !_shopDoorCheck) doorPlay();
	if(!_doorOpen && _shopDoorCheck) doorReverseplay();
	
}

void shopScene::doorPlay()
{
	if (_door->getAniState() != ANIMATION_PLAY)
	{
		//_door->setCurIndex(0);
		_door->aniPlay();
		/*if(!SOUNDMANAGER->isPlaySound("��������0"))
		SOUNDMANAGER->play("��������0");*/
		_shopDoorCheck = true;	

	}
	
}

void shopScene::doorReverseplay()
{
	if (_door->getAniState() == ANIMATION_END)
	{
		_door->aniReverse();
		SOUNDMANAGER->play("���ݾ�");
		_shopDoorCheck = false;
		return;
	}
	
}

void shopScene::sellStandAction()
{
	RECT tmp;
	if (IntersectRect(&tmp, &_stand, &PLAYER->getRect()))
	{
		_button->aniPlay();
		if (INPUT->GetKeyDown('J'))
		{
			_displayStand->openDisplayStand();
			_isShopOpen = true;
			_displayStand->setCanGrab(false);
		}
	}
	else
	{
		_button->setCurIndex(0);
	}
}

void shopScene::sellDeskAction()
{
	RECT tmp;
	if (IntersectRect(&tmp, &_desk, &PLAYER->getRect())) {

		for (int i = 0; i < _npc->getVector().size(); i++) {

			if (_npc->getVector()[i]->getState() == NPC_WAIT) {
				_sellNpcCheck = true;
				if (INPUT->GetKeyDown('J')) {
					_cashRegister->setCurIndex(3);
					_cashRegister->aniPlay();
					_npc->getVector()[i]->setState(NPC_MOVE);
					_sellNpcCheck = false;
					_sellButton->setCurIndex(0);
					_npc->getVector()[i]->setCurrentTargetIdxPlus();
					PLAYERDATA->addGold(_npc->getVector()[i]->getPeekItemCnt() * _npc->getVector()[i]->getPeekItemGold());
					SOUNDMANAGER->play("�������ȸ�");
				}
			}
		}
	}

	if (_sellNpcCheck && !IntersectRect(&tmp, &_desk, &PLAYER->getRect())) {
		_sellButton->setCurIndex(3);
	}
	else if (!_sellNpcCheck && !IntersectRect(&tmp, &_desk, &PLAYER->getRect())) {
		_sellButton->setCurIndex(0);
	}

	if (IntersectRect(&tmp, &_desk, &PLAYER->getRect())) {
		if (_sellNpcCheck && _sellButton->getCurIndex() == 5)  _sellButton->aniPause();
		else if (!_sellNpcCheck && _sellButton->getCurIndex() == 2) _sellButton->aniPause();
		else _sellButton->aniPlay();
	}
}

void shopScene::itemMove()
{

	for (int i = 0; i < v_itemMoveSpeed.size(); i++) {

		if (_displayStand->getDisplayItem()[i].getType() != ITEM_EMPTY && _displayStand->getDisplayItem()[i].getPrice() != 0) {
			if (v_itemMoveSpeed[i].first >= 0.0f && !b[i])
			{
				v_itemMoveSpeed[i].first -= v_itemMoveSpeed[i].second;
				if (v_itemMoveSpeed[i].first <= 1.0f) {
					b[i] = true;
				}
			}

			if (v_itemMoveSpeed[i].first <= 6.5f && b[i])
			{
				v_itemMoveSpeed[i].first += v_itemMoveSpeed[i].second;
				if (v_itemMoveSpeed[i].first >= 6.5f) {
					b[i] = false;
				}
			}
		}
	}
}

void shopScene::SoundUpdate()
{
	if (!CAMERAMANAGER->getFadeIsStart() && !_visit) {
		
		
		if (_door->getAniState() != ANIMATION_PLAY) {
			_door->aniReverse();
			SOUNDMANAGER->play("���ݾ�");
			_visit = true;
		}
	}

	if (!SOUNDMANAGER->isPlaySound("�������"))
	{
		SOUNDMANAGER->play("�������", 0.3f);
	}
	else
	{
		if (ITEMMENU->getOpenMenu()) SOUNDMANAGER->setVolumn("�������", 0.08f);
		else SOUNDMANAGER->setVolumn("�������", 0.3f);
	}
}

void shopScene::backGroundRender()
{
	IMAGEMANAGER->findImage("�����ȼ�")->render(IMAGEMANAGER->findImage("temp")->getMemDC(), 304, 132);

	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("������"), 304, 132);

	CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("�������"), 458 + IMAGEMANAGER->findImage("�������")->getFrameHeight(), 654, 458);

	CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("������"), 870 + IMAGEMANAGER->findImage("������")->getFrameHeight(), 638, 870);

	CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("����ħ��"), 302 + IMAGEMANAGER->findImage("����ħ��")->getFrameHeight() / 4, 752, 302);

	CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("��������"), 674 + IMAGEMANAGER->findImage("��������")->getFrameHeight() / 4, 472, 674);

	CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("����å��"), 670 + IMAGEMANAGER->findImage("����å��")->getFrameHeight() / 4, 646, 670);

	//CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("����������"), 1000 + IMAGEMANAGER->findImage("����������")->getFrameHeight() / 4, 665, 823);

	if (_pixelBackGround) {
		CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("�����ȼ�"), 304, 132);
	}

	_door->ZoderRender(1000, 665, 823);
	//732, 664
	_cashRegister->ZoderRender(700, 732, 664);

	_cauldron->ZoderRender(500, 478, 594);
}

void shopScene::InterfaceRender()
{
	RECT tmp;

	if (IntersectRect(&tmp, &_stand, &PLAYER->getRect())) {
		_button->ZoderRender(1000,
			_stand.right - 10,
			_stand.top - _button->getImage()->getFrameHeight() + 10);
	}

	char str[256];

	for (int i = 0; i < ITEMDESKCOUNT; i++)
	{
		
			if (IntersectRect(&tmp, &_desk, &PLAYER->getRect())) {
				_sellButton->ZoderRender(900, 766, 628);
			}
		
		if (_displayStand->getDisplayItem()[i].getType() != ITEM_EMPTY && _displayStand->getDisplayItem()[i].getPrice() != 0) {

			CAMERAMANAGER->ZorderRender(_displayStand->getDisplayItem()[i].getItemImg(),
				680 + _displayStand->getDisplayItem()[i].getItemImg()->getHeight() / 2
				, v_itemPos[i].first, v_itemPos[i].second + v_itemMoveSpeed[i].first);

			CAMERAMANAGER->ZorderAlphaRender(IMAGEMANAGER->findImage("�����۱׸���")
				, 700 + IMAGEMANAGER->findImage("�����۱׸���")->getHeight()
				, v_itemShadowPos[i].first, v_itemShadowPos[i].second, 60);

			int _price = _displayStand->getDisplayItem()[i].getPrice() * _displayStand->getDisplayItem()[i].getCount();
			wsprintf(str, "%d", _price);

			int tmp = strlen(str);

			CAMERAMANAGER->ZorderStretchRender(IMAGEMANAGER->findImage("npc��ǳ����"),
				750,
				v_itemPos[i].first + _displayStand->getDisplayItem()[i].getItemImg()->getWidth() / 2,
				v_itemPos[i].second - 10 + v_itemMoveSpeed[i].first,
				15 * tmp,
				IMAGEMANAGER->findImage("npc��ǳ������")->getHeight());

			CAMERAMANAGER->ZorderRender(IMAGEMANAGER->findImage("npc��ǳ������"), 751,
				v_itemPos[i].first + _displayStand->getDisplayItem()[i].getItemImg()->getWidth() / 2 - 7 * tmp - IMAGEMANAGER->findImage("npc��ǳ������")->getWidth(),
				v_itemPos[i].second - 22 + v_itemMoveSpeed[i].first);

			CAMERAMANAGER->ZorderRender(IMAGEMANAGER->findImage("npc��ǳ��������"), 751,
				v_itemPos[i].first + _displayStand->getDisplayItem()[i].getItemImg()->getWidth() / 2 + 7 * tmp,
				v_itemPos[i].second - 22 + v_itemMoveSpeed[i].first);
			CAMERAMANAGER->ZorderRender(IMAGEMANAGER->findImage("npc��ǳ������"), 751,
				v_itemPos[i].first + _displayStand->getDisplayItem()[i].getItemImg()->getWidth() / 3,
				v_itemPos[i].second + 1 + v_itemMoveSpeed[i].first);

			int _x;


			if (_price >= 1000000)
			{
				_x = v_itemPos[i].first - _displayStand->getDisplayItem()[i].getItemImg()->getWidth() / 4;
			}
			else if (_price >= 100000) {
				_x = v_itemPos[i].first - _displayStand->getDisplayItem()[i].getItemImg()->getWidth() / 8;
			}
			else if (_price >= 10000) {
				_x = v_itemPos[i].first - _displayStand->getDisplayItem()[i].getItemImg()->getWidth() / 20;
			}
			else if (_price >= 1000) {
				_x = v_itemPos[i].first + _displayStand->getDisplayItem()[i].getItemImg()->getWidth() / 10;
			}
			else if (_price >= 100)
			{
				_x = v_itemPos[i].first + _displayStand->getDisplayItem()[i].getItemImg()->getWidth() / 6;
			}
			else if ((_price >= 10)) {
				_x = v_itemPos[i].first + _displayStand->getDisplayItem()[i].getItemImg()->getWidth() / 4 + 2;
			}
			else {
				_x = v_itemPos[i].first + _displayStand->getDisplayItem()[i].getItemImg()->getWidth() / 3 + 2;
			}
			CAMERAMANAGER->ZorderTextOut(str, 752,
				_x
				, v_itemPos[i].second - 20 + v_itemMoveSpeed[i].first
				, strlen(str), RGB(20, 157, 114));

			wsprintf(str, "%d", _displayStand->getDisplayItem()[i].getCount());
			if (_displayStand->getDisplayItem()[i].getCount() >= 10) {
				_x = v_itemPos[i].first + _displayStand->getDisplayItem()[i].getItemImg()->getWidth() / 2;
			}
			else {
				_x = v_itemPos[i].first + _displayStand->getDisplayItem()[i].getItemImg()->getWidth() / 2 + _displayStand->getDisplayItem()[i].getItemImg()->getWidth() / 6;

			}
			CAMERAMANAGER->ZorderTextOut(str, 753,
				_x,
				v_itemPos[i].second + _displayStand->getDisplayItem()[i].getItemImg()->getHeight(), strlen(str), RGB(255, 255, 255));

			/*CAMERAMANAGER->ZorderRender(IMAGEMANAGER->findImage("npc��ǳ����"),
				780, v_itemPos[i].first, v_itemPos[i].second);*/

		}

		if (_npc->getVector()[i]->getActive())
			_npc->getVector()[i]->render(NPC_SHOP);

		/*wsprintf(str, "X: %d", _displayStand->getDisplayItem()[i].getCount());
		TextOut(getMemDC(), 5, 150 * (i), str, strlen(str));*/


	}

}
