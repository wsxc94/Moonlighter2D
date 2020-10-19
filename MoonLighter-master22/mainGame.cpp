#include "stdafx.h"
#include "mainGame.h"

//=============================================================
//	## 초기화 ## init()
//=============================================================
HRESULT mainGame::init()
{
	gameNode::init();
	
	AddFontResource("Fonts\JejuGothic.ttf");
	hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("JejuGothic"));
	oldFont = (HFONT)SelectObject(getMemDC(), hFont);
	DeleteObject(oldFont);
	/*앞으로 메인게임은 각각의 씬들만 관리를 한다*/
	/*이곳에서 각각의 씬들을 추가하고 현재씬을 설정한다*/

	SCENEMANAGER->addScene("맵툴로딩", new maptoolLoading);
	SCENEMANAGER->addScene("맵툴화면", new maptoolScene);
	SCENEMANAGER->addScene("타이틀화면", new startScene);
	SCENEMANAGER->addScene("타운로딩", new townLoading);
	SCENEMANAGER->addScene("타운화면", new townScene);
	SCENEMANAGER->addScene("노말던전", new nomalDungeonScene);
	SCENEMANAGER->addScene("보스던전", new bossDungeonScene);
	SCENEMANAGER->addScene("던전로딩", new dungeonLoading);
	SCENEMANAGER->addScene("상점로딩", new shopLoading);
	SCENEMANAGER->addScene("상점화면", new shopScene);
	SCENEMANAGER->addScene("던전가는길", new gotoDungeon);
	SCENEMANAGER->addScene("상점가판대", new displayStand);
	SCENEMANAGER->addScene("보스로딩", new bossLoading);

	PLAYER->init();

	SCENEMANAGER->loadScene("타운로딩");

	CAMERAMANAGER->FadeInit(1, FADE_IN);

	return S_OK;
}

//=============================================================
//	## 해제 ## release()
//=============================================================
void mainGame::release()
{
	//PLAYER->release();
	gameNode::release();
}

//=============================================================
//	## 업데이트 ## update()
//=============================================================
void mainGame::update()
{
	gameNode::update();

	SCENEMANAGER->update();
	//사운드매니져 업데이트 (이게 없으면 사운드매니져 제대로 동작하지 않는다!!!)
	SOUNDMANAGER->update();

	//데미지 폰트
	DAMAGEFONT->update();
	//이펙트 매니저 업데이트
	EFFECTMANAGER->update();

	CAMERAMANAGER->FadeUpdate();

	PLAYERDATA->update();
}

//=============================================================
//	## 렌더 ## render()
//=============================================================
void mainGame::render()
{
	//흰색 빈 비트맵 (이것은 렌더에 그냥 두기)
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
//=============================================================
	//글자색 배경 없애기
	SetBkMode(getMemDC(), TRANSPARENT);
	SCENEMANAGER->render();

	INPUT->Update();
	//이펙트 매니저렌더
	EFFECTMANAGER->render();
	//데미지 폰트 렌더
	DAMAGEFONT->render(getMemDC());
	//프레임을 보고싶으면 주석해제
	TIMEMANAGER->render(getMemDC());
	
	PLAYERDATA->render(getMemDC());
	CAMERAMANAGER->FadeRender(getMemDC());

//=============================================================
	//백버퍼의 내용을 화면DC에 그린다 (이것도 렌더에 그냥 두기)
	this->getBackBuffer()->render(getHDC());
}

