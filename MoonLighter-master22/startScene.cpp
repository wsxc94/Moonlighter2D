#include "stdafx.h"
#include "startScene.h"

HRESULT startScene::init()
{
	IMAGEMANAGER->addImage("로고", "Images/타이틀화면/intro_logoImg.bmp", 560, 266, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("인트로1", "Images/타이틀화면/intro0.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("아무키", "Images/타이틀화면/pressAnyKey.bmp", 318, 18, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("뒤버블", "Images/타이틀화면/mainBack16x1.bmp", 20480, 720, 16, 1);
	IMAGEMANAGER->addFrameImage("앞버블", "Images/타이틀화면/mainFront16x1.bmp", 20480, 720, 16, 1);
	//3840
	IMAGEMANAGER->addFrameImage("타이틀화면", "Images/타이틀화면/maintitle2.bmp", 3840, 720, 3, 1, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("새로운게임", "Images/타이틀화면/newgame.bmp", 200, 45, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("옵션", "Images/타이틀화면/option.bmp", 130, 30, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("나가기", "Images/타이틀화면/out.bmp", 130, 30, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("선택메뉴", "Images/타이틀화면/selectMain.bmp", 145, 30, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("맵툴", "Images/타이틀화면/maptool.bmp", 36, 20, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("계속하기", "Images/타이틀화면/continue.bmp", 71, 18, true, RGB(255, 0, 255));

	//IMAGEMANAGER->addFrameImage("로딩아이콘", "Images/타이틀화면/load67.bmp", 2948, 45, 67, 1, true, RGB(255, 0, 255));

	_count = 0;
	_index = 0;
	SOUNDMANAGER->addSound("introBGM", "Sound/introBGM.mp3", true, true);
	SOUNDMANAGER->addSound("doorBGM", "Sound/door.wav");
	_alpha = 0;
	a_alpha = 0;
	t_count = 0;
	t_index = 0;

	r_index = 0;
	state = 0;

	titleLoad = false;

	return S_OK;

}

void startScene::release()
{
}

void startScene::update()
{

	if (_alpha < 254)
	{
		_alpha++;
	}
	if (INPUT->GetKeyDown(VK_RETURN) && !titleLoad) {
		titleLoad = true;
		IMAGEMANAGER->findImage("선택메뉴")->setX(WINSIZEX / 2 - 75);
		IMAGEMANAGER->findImage("선택메뉴")->setY(WINSIZEY - 310);

	}


	if (!SOUNDMANAGER->isPlaySound("introBGM"))
	{

		SOUNDMANAGER->play("introBGM", 0.3f);
	}


	bubbleAnim();


	if (titleLoad)
	{
		currentPos();
		titleAnim();
	}



	if (a_alpha > 254)
	{
		a_alpha = 0;
	}
	else {
		a_alpha++;
	}

}

void startScene::render()
{
	IMAGEMANAGER->frameRender("뒤버블", getMemDC(), 0, 0);
	IMAGEMANAGER->frameRender("앞버블", getMemDC(), 0, 0);

	if (!titleLoad)
	{
		IMAGEMANAGER->render("인트로1", getMemDC(), 0, 0);
		IMAGEMANAGER->alphaRender("아무키", getMemDC(), WINSIZEX / 2 - 150, WINSIZEY - 100, a_alpha);
	}
	else {
		IMAGEMANAGER->frameRender("타이틀화면", getMemDC(), 0, 0);

		IMAGEMANAGER->alphaRender("계속하기", getMemDC(), WINSIZEX / 2 - 40, WINSIZEY - 300, 255);
		IMAGEMANAGER->alphaRender("맵툴", getMemDC(), WINSIZEX / 2 - 25, WINSIZEY - 270, 255);
		IMAGEMANAGER->alphaRender("옵션", getMemDC(), WINSIZEX / 2 - 73, WINSIZEY - 245, 255);
		IMAGEMANAGER->alphaRender("나가기", getMemDC(), WINSIZEX / 2 - 75, WINSIZEY - 215, 255);

		IMAGEMANAGER->alphaRender("선택메뉴", getMemDC(), IMAGEMANAGER->findImage("선택메뉴")->getX(), IMAGEMANAGER->findImage("선택메뉴")->getY(), 255);
	}
	//IMAGEMANAGER->render("로고", getMemDC(), 298, 80);
	IMAGEMANAGER->alphaRender("로고", getMemDC(), 360, 80, _alpha);

	

}

void startScene::bubbleAnim()
{
	_count++;
	IMAGEMANAGER->findImage("뒤버블")->setFrameY(0);
	IMAGEMANAGER->findImage("앞버블")->setFrameY(0);

	if (_count % 5 == 0)
	{
		_index++;
		if (_index >= 16)
		{
			_index = 0;
		}
		IMAGEMANAGER->findImage("뒤버블")->setFrameX(_index);
		IMAGEMANAGER->findImage("앞버블")->setFrameX(_index);
	}
}

void startScene::titleAnim()
{
	t_count++;

	IMAGEMANAGER->findImage("타이틀화면")->setFrameY(0);

	if (t_count % 20 == 0)
	{
		SOUNDMANAGER->stop("doorBGM");
		if (t_index < 3)
		{
			SOUNDMANAGER->play("doorBGM");
			t_index++;
		}
		IMAGEMANAGER->findImage("타이틀화면")->setFrameX(t_index);


	}
}

void startScene::currentPos()
{
	if (INPUT->GetKeyDown(VK_DOWN))
	{
		if (state < 3)
		{
			state++;
			IMAGEMANAGER->findImage("선택메뉴")->setY(IMAGEMANAGER->findImage("선택메뉴")->getY() + 30);
			return;
		}
	}
	if (INPUT->GetKeyDown(VK_UP))
	{
		if (state > 0)
		{
			state--;
			IMAGEMANAGER->findImage("선택메뉴")->setY(IMAGEMANAGER->findImage("선택메뉴")->getY() - 30);
			return;
		}
	}

	if (INPUT->GetKeyDown(VK_SPACE)) {
		SOUNDMANAGER->stop("introBGM");

		switch (state)
		{
		case 0: // 계속하기
			SCENEMANAGER->loadScene("타운로딩");
			break;
		case 1: // 맵툴
			SCENEMANAGER->loadScene("맵툴로딩");
			break;
		case 2: // 옵션
			break;
		case 3: // 나가기
			PostQuitMessage(0);
			break;
		default:
			break;
		}
	}

}


