#include "stdafx.h"
#include "gameNode.h"

//=============================================================
//	## 초기화 ## init()
//=============================================================
HRESULT gameNode::init()
{
	//_CrtSetBreakAlloc(401);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	_hdc = GetDC(_hWnd);				//HDC 얻기
	INPUT->init();						//입력매니져 초기화
	RANDOM->init();						//랜덤매니져 초기화
	IMAGEMANAGER->init();				//이미지매니져 초기화
	PLAYERDATA->init();					//플레이어데이터 초기화
	SCENEMANAGER->init();				//씬매니져 초기화
	INIDATA->init();					//INI데이터 초기화
	SOUNDMANAGER->init();				//사운드매니져 초기화
	TIMEMANAGER->init();				//타임매니저 초기화

	return S_OK;
}

//=============================================================
//	## 해제 ## release()
//=============================================================
void gameNode::release()
{
	//입력매니져 해제
	INPUT->release();
	INPUT->releaseSingleton();
	//랜덤매니져 해제
	RANDOM->release();
	RANDOM->releaseSingleton();
	//이미지매니져 해제
	IMAGEMANAGER->release();
	IMAGEMANAGER->releaseSingleton();
	//플레이어데이터 해제
	PLAYERDATA->release();
	PLAYERDATA->releaseSingleton();
	//씬매니져 해제
	SCENEMANAGER->release();
	SCENEMANAGER->releaseSingleton();
	//INI데이터 해제
	INIDATA->release();
	INIDATA->releaseSingleton();
	//사운드매니져 해제
	SOUNDMANAGER->release();
	SOUNDMANAGER->releaseSingleton();
	//타임매니저 해제
	TIMEMANAGER->release();
	TIMEMANAGER->releaseSingleton();
	//플레이어 해제
	PLAYER->release();
	PLAYER->releaseSingleton();
	//카메라매니저 해제
	CAMERAMANAGER->release();
	CAMERAMANAGER->releaseSingleton();
	//이펙트매니저 해제
	EFFECTMANAGER->release();
	EFFECTMANAGER->releaseSingleton();
	//아이템메뉴 해제
	ITEMMENU->release();
	ITEMMENU->releaseSingleton();
	


	//HDC 해제
	ReleaseDC(_hWnd, _hdc);
}

//=============================================================
//	## 업데이트 ## update()
//=============================================================
void gameNode::update()
{
	//새로고침 (나중에 고성능 타이머를 만든후에는 사용하지 않는다)
	//더블버퍼링 이후 사용하지 않는다 (true => false)
}

//=============================================================
//	## 렌더 ## render()
//=============================================================
void gameNode::render()
{
}

//=============================================================
//	## 메인프로시져 ## MainProc()
//=============================================================
LRESULT gameNode::MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOUSEMOVE:
		_ptMouse.x = LOWORD(lParam);
		_ptMouse.y = HIWORD(lParam);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, message, wParam, lParam));
}
