#pragma once
#include "gameNode.h"
#include "loading.h"
#include "townScene.h"

class townLoading : public gameNode
{
private:
	loading* _loading;
public:
	townLoading() :_loading(nullptr) {}
	~townLoading() {}

	HRESULT init();
	void release();
	void update();
	void render();

	void loadingImage();
	void loadingSound();

};

