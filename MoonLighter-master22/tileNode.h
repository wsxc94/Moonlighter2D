#pragma once
#include "object.h"
//게임화면 타일
#define TILESIZE 35

//이미지 타일 (화면 우측상단에 붙여놓을 샘플타일)
#define SAMPLETILEX 5
#define SAMPLETILEY 5

#define SAMPLEOBJECT 32
#define SAMPLEOBJECTX 2
#define SAMPLEOBJECTY 2

#define SAMPLENPC 12
#define SAMPLENPCX 2
#define SAMPLENPCY 2

//맵툴에서 사용할 컨트롤
enum CTRL
{
	CTRL_DATA, CTRL_SAVE, CTRL_LOAD,
	CTRL_TERRAIN, CTRL_OBJECT, CTRL_NPC, CTRL_ERASER,
	CTRL_SIZE, CTRL_NONE, CTRL_TILESTATE
};

//지형
enum TERRAIN
{
	TR_DUNGEONWALL, TR_GRASS, TR_OTHERS,TR_HOLE,TR_BLOCK, TR_NONE
};

enum TILESTATE
{
	TS_MOVEBAN, TS_PORTAL, TS_EMPTY, TS_HOLE
};

//오브젝트
enum OBJECT
{
	OBJ_OBJECT,
	OBJ_NPC,
	OBJ_NONE
};
//타일구조체
struct tagTile
{
	TILESTATE tState;
	TERRAIN terrain;
	OBJECT obj;
	RECT rc;

	int terrainFrameX;
	int terrainFrameY;
	int objFrameX;
	int objFrameY;

	string key;		//타일 / 오브젝트 이미지의 키값
};

//이미지 타일 구조체
struct tagSampleTile
{
	RECT rc;
	int terrainFrameX;
	int terrainFrameY;
	string key;
};

struct tagSampleObject
{
	RECT rc;
	int terrainFrameX;
	int terrainFrameY;
};

//현재타일 구조체
struct tagCurrentTile
{
	TILESTATE tState;
	int x, y;
	string key;
	RECT rc;
};

struct tagTileUI
{
	RECT rc;
	int tileSizeX;
	int tileSizeY;
	wstring s;
};
