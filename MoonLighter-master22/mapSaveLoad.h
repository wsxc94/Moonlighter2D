#pragma once
#include "singletonBase.h"
#include "tileNode.h"
class mapSaveLoad: public singletonBase<mapSaveLoad>
{
public:
	void saveMap(string fileName, vector<pair<tagTile,tagTile>> vTile, int tile_X, int tile_Y);
	vector<pair<tagTile, tagTile>> loadMap(string fileName, int tile_X, int tile_Y);
	vector<tagTile> loadObject(string fileName, int tile_X, int tile_Y);
	vector<tagTile> loadTile(string fileName, int tile_X, int tile_Y);
};

