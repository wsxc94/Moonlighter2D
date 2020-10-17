#pragma once
#include "gameNode.h"
#include "npcManager.h"
#include "dungeonMap.h"
class townScene : public gameNode
{
private:
	potal* _potal;
	animation* _aniPotalInit;		//던전 시작 / 종료애니
	animation* _playerClone;	//던던 돌아왔을때 구르기 / 죽어서왔을때 주르륵...
	bool _isReturn;		//던전에서 돌아오는거냐?  맞으면 플레이어 클론애니로 굴려버려
private:
	//타일 가로세로길이
	int _tileX;
	int _tileY;

	//전체 타일 페어로 생성하여 first 가 타일정보 second가 오브젝트 정보를 담고있음
	//마을에선 타일을 사용하지 않으므로 second값만 이용할예정입니다
	vector<pair<tagTile, tagTile>> _vTiles;

	vector<tagTile> _vTest; // 사실 이거 쓰는게 맞는거임 - 팀장급 디렉터

	npcManager* _npcManager;

	vector<animation*> _objManager; // 건물 애니메이션 벡터 - 팀장급 디렉터
	//카메라 테스트용 피벗
	float _pivotX, _pivotY;

	RECT shopPortal;       // 상점 포탈 추가 - 팀장급 디렉터
	RECT gotoDungeonPortal;// 던전가는길 포탈 추가 - 팀장급 디렉터

	int _index;
	int _count;
public:

	HRESULT init();
	void release();
	void update();
	void render();

	void portalColl(); // 포탈 이동 추가 - 팀장급 디렉터
	void ObjectColl(); // 플레이어 -> 오브젝트 충돌처리 - 팀장급 디렉터
	void ObjectAnim(); // 오브젝트 애니메이션 함수 - 팀장급 디렉터

	void ObjectSetAnim(); // _objManager 세팅 - 팀장급 디렉터

	HRESULT initPotal();
	void updatePotal();
	void renderPotal();
};

