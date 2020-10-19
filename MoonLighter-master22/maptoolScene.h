#pragma once
#include "gameNode.h"
#include "tileNode.h"
class maptoolScene : public gameNode
{
private:
	image* _backImg;
	int _count; // 애니메이션 시간변수
	int _alpha; // 알파렌더 변수
	object* _obj; //오브젝트 , NPC

	vector<tagTile> _vTiles; // 타일 벡터

	vector<tagTile> _vObject;  // 오브젝트 벡터

	tagSampleTile _sampleTile[SAMPLETILEX * SAMPLETILEY]; // 샘플타일 5 * 5 개

	tagSampleTile _sampleObject[SAMPLEOBJECT]; // 샘플 오브젝트 12 개

	tagSampleTile _sampleNpc[SAMPLENPC]; //샘플 엔피씨 8개

	tagCurrentTile _currentTile; //현재타일

	RECT _tagSize;		//사이즈 태그메뉴
	RECT _tagTile;		//타일 태그메뉴
	RECT _tagData;		//데이터 태그메뉴 

	RECT _rcSave;		//세이브
	RECT _rcLoad;		//로드
	RECT _rcTerrain;	//지형
	RECT _rcObject;		//오브젝트
	RECT _rcNpc;		//npc
	RECT _rcEraser;		//지우개 (오브젝트만 지운다, 지형은 덮어씌운다)
	RECT _rcReset;		//리셋 
	RECT _rcMoveBan;
	RECT _rcMovePosible;
	RECT _rcPortal;
	RECT _rcState;		//타일 스테이트 버튼 

	RECT _button30;		//30 * 30 렉트
	RECT _button50;		//50 * 50 렉트

	RECT _rcArrowUp;	//스크롤 위 화살표
	RECT _rcArrowDown;

	CTRL _ctrlSelect;	//현재 어떤 컨트롤 상태인지
                  
	int tile_X;			
	int tile_Y;

	int _index;

	float _pivotX, _pivotY;

	RECT _menuBar;    //메뉴바
	bool _menuBarOn;  //메뉴바 ON OFF
	bool _eraserOn;  //지우개 ON OFF bool 변수

	float _curScrollY;	//스크롤바의 현재 y좌표 값	
	float _maxScrollY;	//스크롤바의 최대 y값(스크롤바의 길이)		
	float _barLength;	//스크롤바의 바길이 
	float _menuLength;	//출력되는 맵툴 메뉴의 길이 
	float _startY;		//맵툴 이미지가 출력되는 시작좌표값 y 
	int _page;			//현재 보여지고 있는 페이지
	
	bool isleftdown;
	bool isleft;
	bool isleftUp;

	bool objectHit;

	POINT _dragStartPos;		//드래그 시작지점
	POINT _dragEndPos;			//드래그 끝나는 지점
	POINT _cmMouse;				//카메라 상대좌표를 이용한 마우스
	int _dragSizeX;				//드래그 가로갯수	
	int _dragSizeY;				//드래그 세로갯수
	bool _isAutoDrag;
	image* _alphaRect;
	image* _miniMap;

	static char fileName[128];

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//맵툴세팅
	void maptoolSetup();
	void maptoolSetup(int x, int y);
	void setMap();
	void save();
	void save(string fileName);
	void load();
	void load(string fileName);
	bool checkTileMap(int arr);
	void moveRect();
	void checkButton();
	void animation();
	void objecrRectRender();
	void eraseMap();
	void eraseTerrain();
	void eraseObject();
	void eraseNPC();
	void eraseState();
	//맵툴메뉴
	void toggleMenu();
	void toggleEraser();
	void setCtrlBtns();
	void clickSaveBtn();
	void clickLoadBtn();
	void clickCtrlType();
	void clickTileSizeThirty();
	void clickTileSizeFifty();
	void clickTileTown();
	void clickTileDungeon();
	void clickTileGotoDungeon();
	void clickTileBossDungeon();
	void clickResetBtn();
	void clickMoveBanBtn();
	void clickPortalBtn();
	void eraserRender();
	void ClickTagMenu();
	void menuBarRender();
	void scrollBarRender(); //스크롤바 출력 
	void tileArrowUp(); //스크롤바의 위방향 화살표 클릭	
	void tileArrowDown(); //스크롤바의 아래방향 화살표 클릭	
	bool checkObject(); //오브젝트 충돌 검사

    // npc object 변수들
	int npc_index[SAMPLENPC];
	int object_index[SAMPLEOBJECT];

	const char* npcStr[SAMPLENPC] = {
		"강아지npc","토모","에리스","마사","레드먼드","가스총맨","모리","원형탈모아저씨","해골전사" , "항아리",
		"골렘전사" , "babySlime"
	};

	//몇가지 오브젝트 추가함. - 팀장급 디렉터
	const char* objectStr[SAMPLEOBJECT] = {
		"빨랫줄집","눈있는집","주인공집","포션상점","게시판","노란지붕집","의자","판자집","초록나무","진한주황나무","주황나무","분수대",
		"던전돌","던전돌2","던전돌3","던전돌4","던전돌5","나무뿌리","나무뿌리2","던전텐트","던전책","던전의자","던전통","던전보물상자",
		"대각선의자","대장간","이건무슨집","해먹집" , "울타리집","고장난분수","작은나무","오나이테"
	};

	//지형, 오브젝트 선택하기
	TERRAIN terrainSelect(int frameX, int frameY, string key);
	OBJECT objectSelect(int frameX, int frameY);
	OBJECT NPCSelect(int frameX, int frameY);

	//드래그 시작점 세팅
	void setDragStartPos(POINT ptMouse);
	//드래그 끝나는점 업데이트
	void setDragEndPos(POINT ptMouse);
	//드래그 끝난 지점까지 타일깔기
	void setDragTerrain(bool isAuto);
	//드래그 범위안에 타일이 있냐?
	bool isTileInDragRange(int i);
	//드래그 프레임렉트 렌더해주자
	void dragFrameRectRender(COLORREF color);
	void autoDragFrameRectRender();
	//드래그 타일 입력 알고리즘
	void autoDungeonSet(vector<tagTile> &vTile, int index, POINT size, POINT frame, RECT range);
	void autoGrassSet(vector<tagTile> &vTile, int index, POINT size, POINT frame, RECT range);
	void autoHoleSet(vector<tagTile> &vTile, int index, POINT size, POINT frame, RECT range);
	//드래그중인 타일의 가로 세로 개수 구해주기
	void dragCount();
	//드래그중 마우스로 카메라 이동
	void dragMouseFix();
	//카메라 마우스 업데이트
	void cmMouseUpdate();
	//알파 렉트 렌더
	void alphaRectangle(RECT rc, BYTE alpha, COLORREF color,bool isCameraRender);

	//이동금지 상태설정
	void setTileState();
	void autoSetTileState();
	void tileStateRender();

	void changeSampletile();
	void miniMapRender();

	void saveImage();

	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

