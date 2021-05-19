#pragma once
#include "image.h"

enum ANIMATIONPLAYSTATE
{
	ANIMATION_PLAY,
	ANIMATION_PAUSE,
	ANIMATION_REVERSE,
	ANIMATION_END
};
class animation
{
private:
	ANIMATIONPLAYSTATE _aniState;		// �ִϸ��̼� ��� ����
	image*		_img;					// ����� �������̹���
	vector<POINT> _aniList;				// �ִϸ��̼� ���� �ε���
	int			_curIndex;				// ���� �ִϸ��̼� �����ִ� �ε���
	int			_aniFrame;				// �ִϸ��̼� �ֱ�
	int			_aniCount;				// �ִϸ��̼� ī��Ʈ
	int			_maxFrameX;				// ������ �ִ� X ����
	int			_maxFrameY;				// ������ �ִ� Y ����
	int			_frameY;				// ������ y��
	bool		_isLoop;				// ���� �����ų�?
	bool		_isReverse;				// ������ ���ڿ� �ڷ� �����ų�?	1 -> 5 -> 1 ����
	bool		_isArray;				// �迭�̳�?
	RECT        _rc;                    // ��ǥ
	string      _key;                   // Ű��
 
public:
	animation() {}
	//~animation() {}

	//�Ϲ����� �ʱ�ȭ �̹����� ó�� -> ������ ������ ������
	// ���� ( �̹��� , ������ y��, ������ �ֱ�, ������ų�ų�?, ������ ��ų�ų�?)
	HRESULT init(image* img,int frameY,int aniFrame, bool isLoop = false, bool isReverse = false);
	HRESULT init(image* img,int frameY,int aniFrame,RECT rc , string key, bool isLoop = false, bool isReverse = false);
	HRESULT initReverse(image* img, int frameY, int aniFrame, bool isLoop = false, bool isReverse = false);
	//Ư�� �������� ������ ������ ex) 0,3,5 �̷���
	// ���� ( ������ ���� �迭, �̹���, ������ �ֱ�, ������ų?, ��������ų?)
	HRESULT initArray(vector<POINT> aniList,image* img,int aniFrame, bool isLoop = false, bool isReverse = false);
	// ������Ʈ�� ������������ �������� ���� �ʴ´�
	void update();
	// �̹��� ������ų ��ǥ left, top ��ǥ��
	void render(HDC hdc, int destX, int destY);
	void centerRender(HDC hdc, int centerX, int centerY);
	void stretchRender(HDC hdc, int centerX, int centerY,float scale);
	void CameraRender(HDC hdc, int destX, int destY);
	void ZoderRender(int z, int destX, int destY);
	void ZoderAlphaRender(int z, int destX, int destY, BYTE alpha);
	void ZorderRotateRender(HDC hdc, float z, int centerX, int centerY, float angle);
	void ZorderRotateAlphaRender(HDC hdc, float z, int centerX, int centerY, float angle, BYTE alpha);
	void ZorderStretchRender(float z, int centerX, int centerY, float scale);
	void ZorderRotateStretchRender(float z, int centerX, int centerY, float angle, float scale);

	void aniPlay();
	void aniPause();
	void aniReverse();
	void aniStop();
	void aniRestart();

	ANIMATIONPLAYSTATE getAniState() { return _aniState; }
	vector<POINT> getAniList() { return _aniList; }
	int getCurIndex() { return _curIndex; }
	void setCurIndex(int idx) { _curIndex = idx; }
	int getFrameY() { return _frameY; }
	void setFrameY(int y) { _frameY = y; }
	image* getImage() { return _img; }
	void changeImg(image* img) { _img = img; }
	RECT& getRect() { return _rc; }
	string getKey() { return _key; }
	int getAniFrame() { return _aniFrame; }
	bool getIsLoop() { return _isLoop; }
	bool getIsReverse() { return _isReverse; }
};

