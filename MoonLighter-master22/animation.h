#pragma once
#include "image.h"

enum ANIMATIONPLAYSTATE
{
	ANIMATION_PLAY,
	ANIMATION_PAUSE,
	ANIMATION_END
};
class animation
{
private:
	ANIMATIONPLAYSTATE _aniState;		// 애니메이션 재생 종류
	image*		_img;					// 재생할 프레임이미지
	vector<POINT> _aniList;				// 애니메이션 돌릴 인덱스
	int			_curIndex;				// 현제 애니메이션 돌고있는 인덱스
	int			_aniFrame;				// 애니메이션 주기
	int			_aniCount;				// 애니메이션 카운트
	int			_maxFrameX;				// 프레임 최대 X 개수
	int			_maxFrameY;				// 프레임 최대 Y 개수
	int			_frameY;				// 프레임 y축
	bool		_isLoop;				// 루프 돌릴거냐?
	bool		_isReverse;				// 끝까지 간뒤에 뒤로 돌릴거냐?	1 -> 5 -> 1 순서
	bool		_isArray;				// 배열이냐?
	RECT        _rc;                    // 좌표
	string      _key;                   // 키값


public:
	animation() {}
	~animation() {}

	//일반적인 초기화 이미지의 처음 -> 끝까지 프레임 돌린다
	// 인자 ( 이미지 , 프레임 y축, 프레임 주기, 루프시킬거냐?, 리버스 시킬거냐?)
	HRESULT init(image* img,int frameY,int aniFrame, bool isLoop = false, bool isReverse = false);
	HRESULT init(image* img,int frameY,int aniFrame,RECT rc , string key, bool isLoop = false, bool isReverse = false);
	//특정 프레임을 돌리고 싶을때 ex) 0,3,5 이런식
	// 인자 ( 프레임 돌릴 배열, 이미지, 프레임 주기, 루프시킬?, 리버스시킬?)
	HRESULT initArray(vector<POINT> aniList,image* img,int aniFrame, bool isLoop = false, bool isReverse = false);
	// 업데이트를 해주지않으면 프레임이 돌지 않는다
	void update();
	// 이미지 렌더시킬 좌표 left, top 좌표임
	void render(HDC hdc,int destX, int destY);
	void CameraRender(HDC hdc, int destX, int destY);
	void ZoderRender(int z, int destX, int destY);
	void ZoderAlphaRender(int z, int destX, int destY , BYTE alpha);

	void aniPlay();
	void aniPause();
	void aniStop();
	void aniRestart();

	ANIMATIONPLAYSTATE getAniState() { return _aniState; }
	vector<POINT> getAniList() { return _aniList; }
	int getCurIndex() { return _curIndex; }
	int getFrameY() { return _frameY; }
	image* getImage() { return _img; }
	RECT& getRect() { return _rc; }
	void setFrameY(int y) { _frameY = y; }
	string getKey() { return _key; }
};

