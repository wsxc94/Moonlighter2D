#include "stdafx.h"
#include "playerData.h"

HRESULT playerData::init()
{
	initPlayerStatus();
	initDungeonHp();
	_isActivate = true;
	_renderGold = true;
	_renderHP = true;
	_renderKeyInfo = true;
	_renderItemCount = true;
	
	_killEnemy = nullptr;
	_isEmblemReturn = false;
	_isPendantReturn = false;
	_isBossReturn = false;
	_startChangeGold = false; 
	_changeType = GOLD_ADD;
	_changeValue = 0;
	_destGold = 0;
	_changeFrame = 20;

	return S_OK;
}

void playerData::release()
{
}

void playerData::update()
{
	changeGold();
}

void playerData::render(HDC hdc)
{
	if (!_isActivate) return;

	//좌측 UI 출력
	if(_renderGold) goldRender(hdc);
	if(_renderHP) hpRender(hdc);

	//우측 UI 출력 
	if(_renderKeyInfo) equipInfoRender(hdc);
	if(_renderItemCount) itemCountRender(hdc);
}

void playerData::initPlayerStatus()
{
	_defaultHp = 100;
	_defaultAtk = 5;
	_defaultDef = 10;
	_defaultSpd = 185;

	_hp = _defaultHp;
	_atk = _defaultAtk;
	_def = _defaultDef;
	_spd = _defaultSpd;

	_hpBarLength = 130;
	_gold = 2450;
	_itemCount = 0;

	_isInDungeon = false;
	
	//ITEMMENU->getInventory()->updateStatus();
}

void playerData::initDungeonHp()
{
	_inDungeonHp = _hp;
}

void playerData::plusInDungeonHp(int plusHp)
{
	_inDungeonHp += plusHp;

	if (_inDungeonHp > _hp) _inDungeonHp = _hp;
}

void playerData::minusInDungeonHp(int minusHp)
{
	_inDungeonHp -= minusHp;

	if (_inDungeonHp < 0) _inDungeonHp = 0;
}

void playerData::startChangeGold(int changeValue, GOLD_CHANGE type)
{
	_startChangeGold = true; 
	_changeValue = changeValue;
	_changeType = type;

	if (_changeType == GOLD_ADD)
	{
		_destGold = _gold + changeValue;
	}
	else
	{
		_destGold = _gold - changeValue;
	}
}

void playerData::changeGold()
{
	if (!_startChangeGold) return; 

	if (_changeType == GOLD_ADD)
	{
		_gold += _changeFrame;

		if (_gold > _destGold)
		{
			_gold = _destGold;
			_startChangeGold = false; 
		}
	}
	else
	{
		_gold -= _changeFrame;

		if (_gold < _destGold)
		{
			_gold = _destGold;
			_startChangeGold = false; 
		}
	}
}

void playerData::goldRender(HDC hdc)
{
	//골드 주머니 및 코인 아이콘 출력 
	IMAGEMANAGER->render("icon_goldPouch", hdc, 20, 24);
	IMAGEMANAGER->render("icon_coin", hdc, 20, 106);

	//골드 카운트 출력 
	goldCountRender(hdc);
}

void playerData::goldCountRender(HDC hdc)
{
	//자리수에 맞게 첫 번째 숫자부터 출력 

	int multiplyCount = 0;
	int gold = _gold;

	while (true)
	{
		if (gold / 10 != 0)
		{
			gold /= 10;
			multiplyCount++;
		}
		else break; 
	}
	  
	for (int i = 1; i <= _gold; i *= 10)
	{
		int num = (_gold / i) % 10;

		char keyName[24];
		wsprintf(keyName, "%d_white_small", num);
		IMAGEMANAGER->render(keyName, hdc, 46 + (multiplyCount * 8), 108);

		multiplyCount--;
	}
}

void playerData::hpRender(HDC hdc)
{
	//hp바(배경) 이미지 출력 
	IMAGEMANAGER->render("hp_barBack", hdc, 156, 34);

	//hp바(게이지) 이미지 출력 
	hpGaugeRender(hdc);

	//hp아이콘 이미지 출력 
	IMAGEMANAGER->render("icon_hp", hdc, 112, 26);
}

void playerData::hpGaugeRender(HDC hdc)
{
	//만약 던전 안에 있다면 인던전 체력 출력 
	if (_isInDungeon)
	{
		float currentGauge = (_hpBarLength * _inDungeonHp) / _hp;

		//cout << currentGauge << endl; 

		IMAGEMANAGER->render("hp_barFront", hdc, 156, 34, 0, 0, (int)currentGauge, 24);
		hpCountRender(hdc, _inDungeonHp);
	}
	//던전 바깥에 있다면 장비 능력치가 반영된 체력 출력(풀체력) 
	else
	{
		IMAGEMANAGER->render("hp_barFront", hdc, 156, 34);
		hpCountRender(hdc, _hp);
	}
}

void playerData::hpCountRender(HDC hdc, int currentHp)
{
	//현재 체력 출력 
	for (int i = 1, distance = 0; i <= currentHp; i *= 10)
	{
		int num = (currentHp / i) % 10;

		char keyName[24];
		wsprintf(keyName, "%d_white_small", num);
		IMAGEMANAGER->render(keyName, hdc, 248 - (distance * 8), 64);

		distance++;
	}

	//슬래시 출력 
	IMAGEMANAGER->render("slash_white_small", hdc, 256, 64);

	//전체 체력 출력 
	for (int i = 1, distance = 0; i <= _hp; i *= 10)
	{
		int num = (_hp / i) % 10;

		char keyName[24];
		wsprintf(keyName, "%d_white_small", num);
		IMAGEMANAGER->render(keyName, hdc, 280 - (distance * 8), 64);

		distance++;
	}
}

void playerData::equipInfoRender(HDC hdc)
{
	//장착중인 무기 출력 
	weaponEquippedRender(hdc);

	IMAGEMANAGER->render("interface_key", hdc, 1084, 8);
	IMAGEMANAGER->render("icon_bag", hdc, 1168, 166);

	//장착중인 포션 출력 
	potionEquippedRender(hdc);
}

void playerData::itemCountRender(HDC hdc)
{
	int itemCount = ITEMMENU->getInventory()->getCurItemCount();

	//현재 소지하고 있는 아이템 개수 출력 
	for (int i = 1, distance = 0; i <= itemCount; i *= 10)
	{
		int num = (itemCount / i) % 10;

		char keyName[24];
		wsprintf(keyName, "%d_white_small", num);
		IMAGEMANAGER->render(keyName, hdc, 1206 - (distance * 8), 242);

		distance++;
	}

	//슬래시 출력 
	IMAGEMANAGER->render("slash_white_small", hdc, 1214, 242);

	//최대 소지가능한 아이템 개수 출력 
	for (int i = 1, distance = 0; i <= PLAYER->getMaxItemSlot(); i *= 10)
	{
		int num = (PLAYER->getMaxItemSlot() / i) % 10;

		char keyName[24];
		wsprintf(keyName, "%d_white_small", num);
		IMAGEMANAGER->render(keyName, hdc, 1230 - (distance * 8), 242);

		distance++;
	}
}

void playerData::weaponEquippedRender(HDC hdc)
{
	//무기 슬롯(후면) 출력 
	IMAGEMANAGER->frameRender("weapon_slot", hdc, 1132, 60, 1, 0);
	
	switch (_weaponIdx)
	{
		case 1:
			IMAGEMANAGER->frameRender("weapon_backIdx", hdc, 1132, 68, 1, 0);
			break;

		case 2: 
			IMAGEMANAGER->frameRender("weapon_backIdx", hdc, 1132, 68, 0, 0);
			break; 
	}

	//무기 슬롯(전면) 출력 
	IMAGEMANAGER->frameRender("weapon_slot", hdc, 1146, 60, 0, 0);

	switch (_weaponIdx)
	{
		case 1:
			IMAGEMANAGER->frameRender("weapon_frontIdx", hdc, 1146, 68, 0, 0);
			break;

		case 2:
			IMAGEMANAGER->frameRender("weapon_frontIdx", hdc, 1146, 68, 1, 0);
			break;
	}

	//무기를 장착 중이라면 무기 이미지 출력 
	if (_weaponEquipped.getType() != ITEM_WEAPON) return; 

	_weaponEquipped.getItemImg()->render(hdc, 1192, 70);
}

void playerData::potionEquippedRender(HDC hdc)
{
	//현재 포션 슬롯에 장착 중인 아이템이 없다면 함수 종료
	if (_potionEquipped.getType() == ITEM_POTION)
	{
		//포션 이미지 출력 
		_potionEquipped.getItemImg()->render(hdc, 1234, 20);

		//포션 개수 출력 
		for (int i = 1; i <= _potionEquipped.getCount(); i *= 10)
		{
			int num = (_potionEquipped.getCount() / i) % 10;

			char keyName[24];
			wsprintf(keyName, "%d_white_small", num);
			IMAGEMANAGER->render(keyName, hdc, 1238, 50);
		}
	}
}

