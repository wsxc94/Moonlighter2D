#pragma once
#include "gameNode.h"
#include "loading.h"
#include "nomalDungeonScene.h"
class dungeonLoading : public gameNode
{
private:
	unique_ptr<loading> _loading;
public:
	dungeonLoading() : _loading(nullptr) {}
	~dungeonLoading() {}
	HRESULT init();
	void release();
	void update();
	void render();

	void loadImage();
	void loadSound();
};

