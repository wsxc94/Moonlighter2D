#pragma once
#include "gameNode.h"
#include "loading.h"
class bossLoading : public gameNode
{
private:
	loading* _loading;
public:
	bossLoading() : _loading(nullptr) {}
	~bossLoading() {}
	HRESULT init();
	void release();
	void update();
	void render();

	void loadImage();
	void loadSound();
};

