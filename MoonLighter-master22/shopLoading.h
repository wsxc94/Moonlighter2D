#pragma once
#include "gameNode.h"
#include "loading.h"
class shopLoading : public gameNode
{
private:
	loading* _loading;

public:
	shopLoading() :_loading(nullptr) {}
	~shopLoading() {}

	HRESULT init();
	void release();
	void update();
	void render();

	void loadingImage();
	void loadingSound();
};

