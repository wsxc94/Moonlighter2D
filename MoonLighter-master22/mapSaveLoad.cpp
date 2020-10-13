#include "stdafx.h"
#include "mapSaveLoad.h"


void mapSaveLoad::saveMap(string fileName, vector<pair<tagTile, tagTile>> vTile, int tile_X, int tile_Y)
{
	HANDLE file;
	DWORD write;

	string temp = fileName + "tile.map";

	file = CreateFile(temp.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	tagTile* _tile = new tagTile[tile_X*tile_Y];
	for (int i = 0; i < vTile.size(); i++)
	{
		_tile[i] = vTile[i].first;
	}
	WriteFile(file, _tile, sizeof(tagTile) *  tile_X * tile_Y, &write, NULL);
	CloseHandle(file);
	SAFE_DELETE_ARRAY(_tile);

	string temp2 = fileName + "object.map";

	file = CreateFile(temp2.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	tagTile* _obj = new tagTile[tile_X*tile_Y];
	for (int i = 0; i < vTile.size(); i++)
	{
		_obj[i] = vTile[i].second;
	}
	WriteFile(file, _obj, sizeof(tagTile) *  tile_X * tile_Y, &write, NULL);
	CloseHandle(file);
	SAFE_DELETE_ARRAY(_obj);
}

vector<pair<tagTile, tagTile>> mapSaveLoad::loadMap(string fileName, int tile_X, int tile_Y)
{
	HANDLE file;
	DWORD read;
	string temp = fileName + "tile.map";
	vector<tagTile> _vTiles;
	_vTiles.resize(tile_X * tile_Y);
	vector<tagTile> _vObject;
	_vObject.resize(tile_X * tile_Y);
	vector<pair<tagTile, tagTile>> _result;
	_result.resize(tile_X * tile_Y);
	file = CreateFile(temp.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	tagTile* _tile = new tagTile[tile_X * tile_Y];
	ReadFile(file, _tile, sizeof(tagTile) *  tile_X * tile_Y, &read, NULL);
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		_vTiles[i] = _tile[i];
	}
	CloseHandle(file);
	string temp2 = fileName + "object.map";
	file = CreateFile(temp2.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	tagTile* _obj = new tagTile[tile_X * tile_Y];
	ReadFile(file, _obj, sizeof(tagTile) *  tile_X * tile_Y, &read, NULL);
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		_vObject[i] = _obj[i];
	}
	CloseHandle(file);

	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		_result.push_back(make_pair(_vTiles[i], _vObject[i]));
	}
	return _result;
}

vector<tagTile> mapSaveLoad::loadObject(string fileName, int tile_X, int tile_Y)
{
	HANDLE file;
	DWORD read;
	vector<tagTile> _vObject;
	_vObject.resize(tile_X * tile_Y);
	string temp2 = fileName + "object.map";
	file = CreateFile(temp2.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	tagTile* _obj = new tagTile[tile_X * tile_Y];
	ReadFile(file, _obj, sizeof(tagTile) *  tile_X * tile_Y, &read, NULL);
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		_vObject[i] = _obj[i];
	}
	CloseHandle(file);

	return _vObject;
}

vector<tagTile> mapSaveLoad::loadTile(string fileName, int tile_X, int tile_Y)
{
	HANDLE file;
	DWORD read;
	string temp = fileName + "tile.map";
	vector<tagTile> _vTiles;
	_vTiles.resize(tile_X * tile_Y);
	file = CreateFile(temp.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	tagTile* _tile = new tagTile[tile_X * tile_Y];
	ReadFile(file, _tile, sizeof(tagTile) *  tile_X * tile_Y, &read, NULL);
	for (int i = 0; i < tile_X * tile_Y; i++)
	{
		_vTiles[i] = _tile[i];
	}
	CloseHandle(file);
	
	return _vTiles;
}
