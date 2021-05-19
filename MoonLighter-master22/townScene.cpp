#include "stdafx.h"
#include "townScene.h"
#include "mapSaveLoad.h"		//������� ������ ���� �ʿ��Ѱ����� ��Ŭ����ؼ� ���

HRESULT townScene::init()
{

	_vTest = mapSaveLoad::getSingleton()->loadObject("maptool/town3", 74, 60);

	IMAGEMANAGER->addImage("test", 2590, 2100);

	_npcManager = new npcManager;
	_npcManager->init(_vTest);

	PLAYER->init();
	
	CAMERAMANAGER->init(PLAYER->getX(), PLAYER->getY(), 2590, 2100, 0, 0, WINSIZEX >> 1, WINSIZEY >> 1);

	shopPortal = RectMake(1650, 315 , 80 , 80);
	gotoDungeonPortal = RectMake(460, 0, 460, 20);

	_index = 0;
	_count = 0;

	ObjectSetAnim();
	PLAYERDATA->setIsActivate(true);

	CAMERAMANAGER->FadeInit(80, FADE_IN);
	CAMERAMANAGER->FadeStart();

	// ��Ż 
	if (PLAYERDATA->getIsEmblemReturn() || PLAYERDATA->getIsBossReturn())
	{
		_potal = new potal;
		_potal->init(300, 200, POTAL_INIT);
		PLAYER->setX(300);
		PLAYER->setY(205);
		PLAYER->setPlayerDirection(0);
	}
	else _potal = nullptr;

	if (PLAYERDATA->getIsPendantReturn())
	{
		PLAYER->setX(300);
		PLAYER->setY(205);
		PLAYER->setPlayerState(PLAYER_DIE_PORTAL);
		PLAYER->setPlayerDirection(0);
		PLAYERDATA->setIsPendantReturn(false);
	}

	
	cloudInit();
	this->initPotal();
	return S_OK;
}

void townScene::release()
{
	/*SAFE_DELETE(_aniPotalInit);
	
	_npcManager->release();
	SAFE_DELETE(_npcManager);
	
	for (int i = 0; i < _objManager.size(); i++) {
		SAFE_DELETE(_objManager[i]);
	}*/
}

void townScene::update()
{
	//PLAYER->setPlace(SHOP);
	if (!SOUNDMANAGER->isPlaySound("�������")) 
	{
		SOUNDMANAGER->play("�������", 0.3f);
	}
	else 
	{
		if (ITEMMENU->getOpenMenu() /*|| _npcManager->getPotionShopNpc()*/ ) SOUNDMANAGER->setVolumn("�������", 0.08f);
		else SOUNDMANAGER->setVolumn("�������", 0.3f);
	}

	//====================================================
	if (INPUT->GetKeyDown('G')) {
		SOUNDMANAGER->stop("�������");
		SCENEMANAGER->loadScene("�����ε�");
	}
	if (INPUT->GetKeyDown('H')) {
		SOUNDMANAGER->stop("�������");
		SCENEMANAGER->loadScene("�����ε�");
	}
	//if (INPUT->GetKeyDown('P')) {
	//	SOUNDMANAGER->stop("�������");
	//	SCENEMANAGER->loadScene("�����ε�");
	//}
	//if (INPUT->GetKeyDown('U')) {
	//	//SOUNDMANAGER->stop("�������");
	//	SCENEMANAGER->loadScene("�������±�");
	//}

	if (!_potal || (_potal->getPotalState() != POTAL_INIT &&
		_potal->getPotalState() != POTAL_PLAYERIN &&
		_potal->getPotalState() != POTAL_PLAYEROUT))
	{
		if(!_npcManager->getPotionShopNpc()->getPotionShop()->getMenuOn()) PLAYER->update();
	}

	this->updatePotal();

	CAMERAMANAGER->movePivot(PLAYER->getX(), PLAYER->getY());
	CAMERAMANAGER->update(PLAYER->getX(), PLAYER->getY());
	//====================================================

	_npcManager->update();
    ITEMMENU->update();
	//if(!ITEMMENU->getOpenMenu()) _potionShop->update();

	portalColl(); // ��Ż �̵� �߰� - ����� ����
	ObjectAnim();
	ObjectColl();
	MapColl();
	this->collArrow();
	cloudMove();

}

void townScene::render()
{
	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("townBack"), 0, 0);
	//Rectangle(getMemDC(), CAMERAMANAGER->getRect());
	RECT tmp;
	for (int i = 0; i < _objManager.size(); i++)
	{
		//RECT tmp;
		if (!IntersectRect(&tmp, &_objManager[i]->getRect(), &CAMERAMANAGER->getRect())) continue;

		RECT temp;
		temp = RectMake(_objManager[i]->getRect().left + (_objManager[i]->getImage()->getFrameWidth() >> 3),
			_objManager[i]->getRect().top + ((_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) >> 3),
			(_objManager[i]->getRect().right - _objManager[i]->getRect().left) / 1.3,
			(_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) >> 1);
		
		//CAMERAMANAGER->Rectangle(getMemDC(), temp); //����׿� -> z������Ʈ 

		RECT temp2;
		if (_objManager[i]->getKey() == "��Ȳ����" || _objManager[i]->getKey() == "������Ȳ����" || _objManager[i]->getKey() == "�ʷϳ���") {
			
			temp2 = RectMake(_objManager[i]->getRect().left + (_objManager[i]->getImage()->getFrameWidth() >> 2),
				_objManager[i]->getRect().top + ((_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) >> 1) ,
				(_objManager[i]->getRect().right - _objManager[i]->getRect().left) - _objManager[i]->getImage()->getFrameWidth() / 3,
				(_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) - _objManager[i]->getImage()->getFrameHeight()*0.6);
		}
		else {
			temp2 = RectMake(_objManager[i]->getRect().left + _objManager[i]->getImage()->getFrameWidth() / 40,
				_objManager[i]->getRect().top + ((_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) >> 2) ,
				//(_objManager[i]->getRect().right - _objManager[i]->getRect().left/10),
				_objManager[i]->getImage()->getFrameWidth()- _objManager[i]->getImage()->getFrameWidth() / 40,
				(_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) - _objManager[i]->getImage()->getFrameHeight()*0.3);

		}

		//CAMERAMANAGER->Rectangle(getMemDC(), temp2); //����׿� -> �浹��Ʈ

		if (IntersectRect(&tmp, &PLAYER->getShadowRect(), &temp)) {
			if (_objManager[i]->getKey() == "���ΰ���" || _objManager[i]->getKey() == "���ִ���") {
				_objManager[i]->ZoderAlphaRender(_objManager[i]->getRect().top + (_objManager[i]->getImage()->getFrameHeight() >> 1),
					_objManager[i]->getRect().left - 15, _objManager[i]->getRect().top, 150);
			}
			else {
				_objManager[i]->ZoderAlphaRender(_objManager[i]->getRect().top + (_objManager[i]->getImage()->getFrameHeight() >> 1),
					_objManager[i]->getRect().left, _objManager[i]->getRect().top, 150);
			}
		}
		else {
			if (_objManager[i]->getKey() == "���ΰ���" || _objManager[i]->getKey() == "���ִ���") {
				_objManager[i]->ZoderRender(_objManager[i]->getRect().top + (_objManager[i]->getImage()->getFrameHeight() >> 1),
					_objManager[i]->getRect().left - 15, _objManager[i]->getRect().top);
			}
			else {
				_objManager[i]->ZoderRender(_objManager[i]->getRect().top + (_objManager[i]->getImage()->getFrameHeight() >> 1),
					_objManager[i]->getRect().left, _objManager[i]->getRect().top);
			}
		}
	}

	CAMERAMANAGER->ZorderTotalRender(getMemDC());
	_npcManager->render();

	if (!_potal || (_potal->getPotalState() != POTAL_INIT && _potal->getPotalState() != POTAL_PLAYERIN && _potal->getPotalState() != POTAL_PLAYEROUT))
		PLAYER->render(getMemDC());


	this->renderPotal();
	cloudRender();

	ITEMMENU->render(getMemDC());

	//CAMERAMANAGER->Rectangle(getMemDC(), shopPortal);
	//CAMERAMANAGER->Rectangle(getMemDC(), gotoDungeonPortal);

	//Rectangle(getMemDC(), PLAYER->getShadowRect());
	/*char str[256];
	POINT tmp = CAMERAMANAGER->getRelativeMouse(_ptMouse);

	wsprintf(str, "X: %d", tmp.x);
	TextOut(getMemDC(), 5, 150, str, strlen(str));
	wsprintf(str, "Y: %d", tmp.y);
	TextOut(getMemDC(), 5, 170, str, strlen(str));*/

}

void townScene::portalColl()
{
	RECT tmp;
	if (IntersectRect(&tmp , &PLAYER->getShadowRect() , &gotoDungeonPortal))
	{
		SCENEMANAGER->loadScene("�������±�");
	}
	if (IntersectRect(&tmp, &PLAYER->getShadowRect(), &shopPortal))
	{
		SOUNDMANAGER->stop("�������");
		SCENEMANAGER->loadScene("�����ε�");
		SOUNDMANAGER->play("��������0");
	}
}

void townScene::ObjectColl()
{
	for (int i = 0; i < _objManager.size(); i++)
	{
	    RECT tmp;
		if (!IntersectRect(&tmp, &_objManager[i]->getRect(), &CAMERAMANAGER->getRect())) continue;
		
			RECT temp;
			
			temp = RectMake(_objManager[i]->getRect().left + (_objManager[i]->getImage()->getFrameWidth() >> 3),
				_objManager[i]->getRect().top + ((_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) >> 3),
				(_objManager[i]->getRect().right - _objManager[i]->getRect().left) / 1.3,
				((_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) >> 1));


			if (_objManager[i]->getKey() == "��Ȳ����" || _objManager[i]->getKey() == "������Ȳ����" || _objManager[i]->getKey() == "�ʷϳ���") {
				temp = RectMake(_objManager[i]->getRect().left + (_objManager[i]->getImage()->getFrameWidth() >> 2),
					_objManager[i]->getRect().top + ((_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) >> 1),
					(_objManager[i]->getRect().right - _objManager[i]->getRect().left) - _objManager[i]->getImage()->getFrameWidth() / 3,
					(_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) - _objManager[i]->getImage()->getFrameHeight()*0.6);
			}
			else {
				temp = RectMake(_objManager[i]->getRect().left + _objManager[i]->getImage()->getFrameWidth() / 40,
					_objManager[i]->getRect().top + ((_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) >> 2),
					//(_objManager[i]->getRect().right - _objManager[i]->getRect().left/10),
					_objManager[i]->getImage()->getFrameWidth() - _objManager[i]->getImage()->getFrameWidth() / 40,
					(_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) - _objManager[i]->getImage()->getFrameHeight()*0.3);
			}

			if (IntersectRect(&tmp, &PLAYER->getShadowRect(), &temp)) {
				int wid = tmp.right - tmp.left;
				int hei = tmp.bottom - tmp.top;
				int pwid = ((PLAYER->getShadowRect().right - PLAYER->getShadowRect().left) >> 1);
				int phei = ((PLAYER->getShadowRect().bottom - PLAYER->getShadowRect().top) >> 1);

				if (wid > hei) // ���Ʒ�
				{
					if (tmp.top == PLAYER->getShadowRect().top) // �Ʒ�
					{
						PLAYER->setY(temp.bottom + phei);
					}
					else  // ��
					{
						PLAYER->setY(temp.top - phei);
					}
				}
				else  // �翷
				{
					if (tmp.left == PLAYER->getShadowRect().left) // ������
					{
						PLAYER->setX(temp.right + pwid);
					}
					else // ����
					{
						PLAYER->setX(temp.left - pwid);
					}
				}
			}
	}
}

void townScene::ObjectAnim()
{
	RECT tmp;
	for (int i = 0; i < _objManager.size(); i++)
	{
		if (!IntersectRect(&tmp, &_objManager[i]->getRect(), &CAMERAMANAGER->getRect())) continue;
		_objManager[i]->update();
	}
}

void townScene::ObjectSetAnim()
{
	for (int i = 0; i < _vTest.size(); i++) {

		if (_vTest[i].obj != OBJ_NPC && _vTest[i].key != "") {
			animation* anim;
			anim = new animation;
			anim->init(IMAGEMANAGER->findImage(_vTest[i].key), 0, 7, _vTest[i].rc, _vTest[i].key, true, false);
			_objManager.push_back(anim);
		}
	}
}

void townScene::MapColl()
{

	if (30 > CAMERAMANAGER->getRelativeX(PLAYER->getX()))
	{
		PLAYER->setX(PLAYER->getX() + PLAYER->getJumpPower() + 0.2);

		PLAYER->setRollGravity(0);
		PLAYER->setRollPower(0);

		PLAYER->setX(PLAYER->getX() + PLAYER->getSpeed());
		
		
	}
	if (WINSIZEX-30 < CAMERAMANAGER->getRelativeX(PLAYER->getX()))
	{
		PLAYER->setX(PLAYER->getX() - PLAYER->getJumpPower() - 0.2);

		PLAYER->setRollGravity(0);
		PLAYER->setRollPower(0);

		PLAYER->setX(PLAYER->getX() - PLAYER->getSpeed());
		
	}
	if (30 > CAMERAMANAGER->getRelativeY(PLAYER->getY()))
	{
		PLAYER->setY(PLAYER->getY() + PLAYER->getJumpPower() + 0.2);

		PLAYER->setRollGravity(0);
		PLAYER->setRollPower(0);

		PLAYER->setY(PLAYER->getY() + PLAYER->getSpeed());
	}
	if (WINSIZEY-30 < CAMERAMANAGER->getRelativeY(PLAYER->getY()))
	{
		PLAYER->setY(PLAYER->getY() - PLAYER->getJumpPower() - 0.2);

		PLAYER->setRollGravity(0);
		PLAYER->setRollPower(0);

		PLAYER->setY(PLAYER->getY() - PLAYER->getSpeed());
	}
}

void townScene::collArrow()
{
	RECT temp;
	if (!IntersectRect(&temp, &PLAYER->getArrow()->getRect(), &CAMERAMANAGER->getRect()))
	{
		PLAYER->getArrow()->setIsShoot(false);
	}
}

void townScene::cloudInit()
{
	for (int i = 0; i < 3; i++)
	{
		Cloud* tmp_cloud;
		tmp_cloud = new Cloud;
		v_cloud.push_back(tmp_cloud);
		tagPosF tmp;
		tmp.x = RANDOM->range(400, IMAGEMANAGER->findImage("townBack")->getWidth() + 200);
		tmp.y = RANDOM->range(-1800, -3500);
		v_cloud[i]->init(tmp);
	}
}

void townScene::cloudMove()
{
	for (int i = 0; i < v_cloud.size(); i++)
	{
		v_cloud[i]->update();
	}
}

void townScene::cloudRender()
{
	RECT tmp;

	for (int i = 0; i < v_cloud.size(); i++)
	{
		if (!IntersectRect(&tmp, &v_cloud[i]->getRect(), &CAMERAMANAGER->getRect())) continue;
		v_cloud[i]->render(getMemDC());
	}
}

HRESULT townScene::initPotal()
{
	

	return S_OK;
}

void townScene::updatePotal()
{
	if (!_potal) return;
	_potal->update();
	if (PLAYERDATA->getIsBossReturn())
	{
		if (_potal->getPotalState() == POTAL_UPDATE)
		{
			_potal->setPotalState(POTAL_BREAK);
		}
		if (_potal->getIsActivate() == false)
		{
			PLAYERDATA->setIsBossReturn(false);
			_potal->release();
			SAFE_DELETE(_potal);
		}
	}
	else
	{
		if (INPUT->GetKeyDown('J') && _potal->getIsInRange())
		{
			_potal->setPotalState(POTAL_PLAYERIN);
		}

		if (_potal->getPotalState() == POTAL_PLAYERIN && _potal->getAnimation()->getAniState() == ANIMATION_END)
		{
			SOUNDMANAGER->stop("�������");
			SCENEMANAGER->loadScene("�����ε�");
		}
	}



}

void townScene::renderPotal()
{
	if (!_potal) return;
	_potal->render();
}
