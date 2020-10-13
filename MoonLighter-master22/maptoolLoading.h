#pragma once
#include "gameNode.h"
#include "loading.h"
#include "maptoolScene.h"

class maptoolLoading : public gameNode
{
private:
	loading* _loading;

public:
	maptoolLoading() : _loading(nullptr) {}
	~maptoolLoading() {}

	HRESULT init();
	void release();
	void update();
	void render();

	//로딩이미지 함수 (이곳에 이미지를 전부 넣어라)
	void loadingImage();
	//로딩사운드 함수 (이곳에 사운드를 전부 넣어라)
	void loadingSound();
};

