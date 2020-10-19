#include "stdafx.h"
#include "townScene.h"
#include "mapSaveLoad.h"		//헤더꼬임 방지를 위해 필요한곳에만 인클루드해서 사용

HRESULT townScene::init()
{
	_vTest = mapSaveLoad::getSingleton()->loadObject("maptool/town3", 74, 60);

	IMAGEMANAGER->addImage("test", 2590, 2100);

	_npcManager = new npcManager;
	_npcManager->init(_vTest);

	PLAYER->init();
	CAMERAMANAGER->init(PLAYER->getX(), PLAYER->getY(), 2590, 2100, 0, 0, WINSIZEX / 2, WINSIZEY / 2);
	ITEMMENU->init();

	_potionShop = new potionShop;
	_potionShop->init();

	shopPortal = RectMake(1650, 315 , 80 , 80);
	gotoDungeonPortal = RectMake(460, 0, 460, 20);

	_index = 0;
	_count = 0;

	ObjectSetAnim();

	CAMERAMANAGER->FadeInit(80, FADE_IN);
	CAMERAMANAGER->FadeStart();

	this->initPotal();
	return S_OK;
}

void townScene::release()
{
	//_npcManager->release();
}

void townScene::update()
{
	//PLAYER->setPlace(SHOP);
	if (!SOUNDMANAGER->isPlaySound("마을브금")) 
	{
		SOUNDMANAGER->play("마을브금", 0.3f);
	}
	else
	{
		if (ITEMMENU->getOpenMenu() || _potionShop->getMenuOn()) SOUNDMANAGER->setVolumn("마을브금", 0.08f);
		else SOUNDMANAGER->setVolumn("마을브금", 0.3f);
	}

	//====================================================

	if (INPUT->GetKeyDown('G')) {
		SOUNDMANAGER->stop("마을브금");
		SCENEMANAGER->loadScene("던전로딩");
	}
	if (INPUT->GetKeyDown('P')) {
		SOUNDMANAGER->stop("마을브금");
		SCENEMANAGER->loadScene("상점로딩");
	}
	if (INPUT->GetKeyDown('U')) {
		//SOUNDMANAGER->stop("마을브금");
		SCENEMANAGER->loadScene("던전가는길");
	}
	if (INPUT->GetKeyDown('U')) {
		//SOUNDMANAGER->stop("마을브금");
		SCENEMANAGER->loadScene("던전가는길");
	}

	if (_playerClone->getAniState() == ANIMATION_END)
	{
		if(!_potionShop->getMenuOn())
			PLAYER->update();
	}

	this->updatePotal();

	CAMERAMANAGER->movePivot(PLAYER->getX(), PLAYER->getY());
	CAMERAMANAGER->update(PLAYER->getX(), PLAYER->getY());
	
	//====================================================

	ITEMMENU->update();
	_npcManager->update();
	if(!ITEMMENU->getOpenMenu()) _potionShop->update();
 
	portalColl(); // 포탈 이동 추가 - 팀장급 디렉터
	ObjectAnim();
	ObjectColl();

}

void townScene::render()
{
	CAMERAMANAGER->Render(getMemDC(), IMAGEMANAGER->findImage("townBack"), 0, 0);
	IMAGEMANAGER->findImage("죽음")->stretchFrameRender(getMemDC(), 200, 200, 0, 0, 2.f, 1.f);
	
	for (int i = 0; i < _objManager.size(); i++)
	{
		RECT tmp;
		if (!IntersectRect(&tmp, &_objManager[i]->getRect(), &CAMERAMANAGER->getRect())) continue;

		RECT temp;
		temp = RectMake(_objManager[i]->getRect().left + _objManager[i]->getImage()->getFrameWidth() / 8,
			_objManager[i]->getRect().top + (_objManager[i]->getRect().bottom - _objManager[i]->getRect().top)/8,
			(_objManager[i]->getRect().right - _objManager[i]->getRect().left) / 1.3,
			(_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) / 2);

		//CAMERAMANAGER->Rectangle(getMemDC(), temp); //디버그용 -> z오더렉트 

		RECT temp2;
		if (_objManager[i]->getKey() == "주황나무" || _objManager[i]->getKey() == "진한주황나무" || _objManager[i]->getKey() == "초록나무") {
			
			temp2 = RectMake(_objManager[i]->getRect().left + _objManager[i]->getImage()->getFrameWidth() / 4,
				_objManager[i]->getRect().top + (_objManager[i]->getRect().bottom - _objManager[i]->getRect().top)/2,
				(_objManager[i]->getRect().right - _objManager[i]->getRect().left) - _objManager[i]->getImage()->getFrameWidth() / 3,
				(_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) - _objManager[i]->getImage()->getFrameHeight()*0.6);
		}
		else {
			temp2 = RectMake(_objManager[i]->getRect().left + _objManager[i]->getImage()->getFrameWidth() / 40,
				_objManager[i]->getRect().top + (_objManager[i]->getRect().bottom - _objManager[i]->getRect().top)/4,
				//(_objManager[i]->getRect().right - _objManager[i]->getRect().left/10),
				_objManager[i]->getImage()->getFrameWidth()- _objManager[i]->getImage()->getFrameWidth() / 40,
				(_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) - _objManager[i]->getImage()->getFrameHeight()*0.3);

		}

		//CAMERAMANAGER->Rectangle(getMemDC(), temp2); //디버그용 -> 충돌렉트

		if (IntersectRect(&tmp, &PLAYER->getShadowRect(), &temp)) {
			if (_objManager[i]->getKey() == "주인공집" || _objManager[i]->getKey() == "눈있는집") {
				_objManager[i]->ZoderAlphaRender(_objManager[i]->getRect().top + _objManager[i]->getImage()->getFrameHeight() / 2,
					_objManager[i]->getRect().left - 15, _objManager[i]->getRect().top, 150);
			}
			else {
				_objManager[i]->ZoderAlphaRender(_objManager[i]->getRect().top + _objManager[i]->getImage()->getFrameHeight() / 2,
					_objManager[i]->getRect().left, _objManager[i]->getRect().top, 150);
			}
		}
		else {
			if (_objManager[i]->getKey() == "주인공집" || _objManager[i]->getKey() == "눈있는집") {
				_objManager[i]->ZoderRender(_objManager[i]->getRect().top + _objManager[i]->getImage()->getFrameHeight() / 2,
					_objManager[i]->getRect().left - 15, _objManager[i]->getRect().top);
			}
			else {
				_objManager[i]->ZoderRender(_objManager[i]->getRect().top + _objManager[i]->getImage()->getFrameHeight() / 2,
					_objManager[i]->getRect().left, _objManager[i]->getRect().top);
			}
		}
	}

	CAMERAMANAGER->ZorderTotalRender(getMemDC());
	_npcManager->render();

	if (_playerClone->getAniState() == ANIMATION_END)
		PLAYER->render(getMemDC());
	else
		_playerClone->ZoderRender(PLAYER->getY(), PLAYER->getX() - 60, PLAYER->getY() - 60);
	
	ITEMMENU->render(getMemDC());
	_potionShop->render();

	this->renderPotal();

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
		SCENEMANAGER->loadScene("던전가는길");
	}
	if (IntersectRect(&tmp, &PLAYER->getShadowRect(), &shopPortal))
	{
		SOUNDMANAGER->stop("마을브금");
		SCENEMANAGER->loadScene("상점로딩");
		SOUNDMANAGER->play("상점입장0");
	}
}

void townScene::ObjectColl()
{
	for (int i = 0; i < _objManager.size(); i++)
	{
	    RECT tmp;
		if (!IntersectRect(&tmp, &_objManager[i]->getRect(), &CAMERAMANAGER->getRect())) continue;
		
			RECT temp;
			
			temp = RectMake(_objManager[i]->getRect().left + _objManager[i]->getImage()->getFrameWidth() / 8,
				_objManager[i]->getRect().top + (_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) / 8,
				(_objManager[i]->getRect().right - _objManager[i]->getRect().left) / 1.3,
				(_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) / 2);


			if (_objManager[i]->getKey() == "주황나무" || _objManager[i]->getKey() == "진한주황나무" || _objManager[i]->getKey() == "초록나무") {
				temp = RectMake(_objManager[i]->getRect().left + _objManager[i]->getImage()->getFrameWidth() / 4,
					_objManager[i]->getRect().top + (_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) / 2,
					(_objManager[i]->getRect().right - _objManager[i]->getRect().left) - _objManager[i]->getImage()->getFrameWidth() / 3,
					(_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) - _objManager[i]->getImage()->getFrameHeight()*0.6);
			}
			else {
				temp = RectMake(_objManager[i]->getRect().left + _objManager[i]->getImage()->getFrameWidth() / 40,
					_objManager[i]->getRect().top + (_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) / 4,
					//(_objManager[i]->getRect().right - _objManager[i]->getRect().left/10),
					_objManager[i]->getImage()->getFrameWidth() - _objManager[i]->getImage()->getFrameWidth() / 40,
					(_objManager[i]->getRect().bottom - _objManager[i]->getRect().top) - _objManager[i]->getImage()->getFrameHeight()*0.3);
			}

			if(IntersectRect(&tmp, &PLAYER->getShadowRect(), &temp)) {
				int wid = tmp.right - tmp.left;
				int hei = tmp.bottom - tmp.top;

				if (wid > hei) // 위아래
				{
					if (tmp.top == PLAYER->getShadowRect().top) // 아래
					{
						PLAYER->setY(PLAYER->getY() + hei);
					}
					else  // 위
					{
						PLAYER->setY(PLAYER->getY() - hei);
					}
				}
				else  // 양옆
				{
					if (tmp.left == PLAYER->getShadowRect().left) // 오른쪽
					{
						PLAYER->setX(PLAYER->getX() + wid);
					}
					else // 왼쪽
					{
						PLAYER->setX(PLAYER->getX() - wid);
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

HRESULT townScene::initPotal()
{
	if (ITEMMENU->getGoToTownEmblem())
	{
		_potal.ani = new animation;
		_potal.ani->init(IMAGEMANAGER->findImage("potalUpdate"), 0, 7,true);
		_potal.ani->aniPause();
		_potal.x = 200;
		_potal.y = 200;
		_potal.isActivate = true;
		_potal.isRange = false;
		_potal.isUpdate = false;
		PLAYER->setX(200);
		PLAYER->setY(195);
		_aniPotalInit = new animation;
		_aniPotalInit->init(IMAGEMANAGER->findImage("potalInit"), 0, 7, false, true);
		_playerClone = new animation;
		_playerClone->init(IMAGEMANAGER->findImage("던전구르기"), 0, 5);
	}
	else if (ITEMMENU->getGoToTownPendant())
	{

	}
	else
	{
		_playerClone = new animation;
		_playerClone->init(IMAGEMANAGER->findImage("던전구르기"), 0, 5);
		_playerClone->aniStop();
		_potal.isActivate = false;
		
	}

	return S_OK;
}

void townScene::updatePotal()
{
	if (_potal.isActivate == false) return;
	_aniPotalInit->update();
	if (_aniPotalInit->getCurIndex() == _aniPotalInit->getImage()->getMaxFrameX() && _playerClone->getAniState() != ANIMATION_END)
	{
		_playerClone->update();
		PLAYER->setY(PLAYER->getY() + 4);
		_aniPotalInit->aniPause();
		_potal.ani->aniPlay();
	}

	if (getDistance(PLAYER->getX(), PLAYER->getY(), _potal.x, _potal.y) < 30 && _potal.isUpdate)
	{
		_potal.isRange = true;
	}
	else _potal.isRange = false;


	if (_potal.isRange)
	{
		if (INPUT->GetKeyDown('J'))
		{
			
		}
	}
}

void townScene::renderPotal()
{

	if (_potal.isActivate)
	{
		_potal.ani->ZorderStretchRender(_potal.y, _potal.x, _potal.y, 2.f);
		if (_potal.isRange)
		{
			RECT txtRC = RectMake(_potal.x + 80, _potal.y - 90, 110, 50);
			HFONT hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
				0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
			//IMAGEMANAGER.findImage("messegeBox_potal").render(getMemDC(), _potal.x + 30, _potal.y - 90);
			CAMERAMANAGER->ZorderRender(IMAGEMANAGER->findImage("messegeBox_potal"), 1999, _potal.x + 30, _potal.y - 90);
			CAMERAMANAGER->ZorderDrawText("To Town", 2000, txtRC, hFont, RGB(0, 0, 0), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
}
