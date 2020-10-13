#pragma once
#include "gameNode.h"
//=============================================================
//	## progressBar ## (프로그래스바 = 체력바 = 로딩바 등등)
//=============================================================

class progressBar : public gameNode
{
private:
	image* _progressBarFront;		//체력바 앞 이미지(렉트변경됨)
	image* _progressBarBack;		//체력바 뒤 이미지(변경안함)
	RECT _rcProgress;				//체력바 렉트
	float _width;					//체력바 가로길이
	float _height;					//체력바 세로길이
	float _maxGauge;

public:
	HRESULT init(const char* frontImg, const char* backImg, float maxGauge);
	void release();
	void update(float currentHp);
	// 센터점
	void render(int x, int y);
	void alphaRender(int x, int y, BYTE alpha);
	void cameraRender(int x, int y);
	void cameraAlphaRender(int x, int y, BYTE alpha);
	void ZorderRender(float z, int x, int y);
	void ZorderAlphaRender(float z, int x, int y);

};

