#pragma once
#include "gameNode.h"
#include "loading.h"

class itemMenuLoading : public gameNode
{
private:
	loading* _loading;
public:
	itemMenuLoading() :_loading(nullptr) {}
	~itemMenuLoading() {}

	HRESULT init();
	void release();
	void update();
	void render();

	void loadingImage();
	void loadingSound();
};

