#include "stdafx.h"
#include "itemManager.h"

//일반 아이템 초기화 
HRESULT gameItem::init(string itemKey, string nameKey, string desKey, const char* name, int itemIdx, int cheapPrice, int reasonablePrice, int expensivePrice, int outrageousPrice, int maxCount)
{
	//전체 변수값 0으로 초기화 
	memset(&_item, 0, sizeof(_item));

	_item.name = name;
	_item.imgItem = IMAGEMANAGER->findImage(itemKey);
	_item.imgName = IMAGEMANAGER->findImage(nameKey);
	_item.imgDes = IMAGEMANAGER->findImage(desKey);

	_item.type = ITEM_NORMAL;
	_item.priceRange[PRICE_CHEAP] = cheapPrice;
	_item.priceRange[PRICE_REASONABLE] = reasonablePrice;
	_item.priceRange[PRICE_EXPENSIVE] = expensivePrice;
	_item.priceRange[PRICE_OUTRAGEOUS] = outrageousPrice;

	_item.itemIdx = itemIdx;
	_item.count = 1;
	_item.maxCount = maxCount;
	_item.holeAlpha = 255;
	_item.initSpeed = 3.f;
	_item.initAngle = RANDOM->range(PI2);

	return S_OK;
}

//포션 아이템 초기화 
HRESULT gameItem::init(string itemKey, string nameKey, string desKey, const char* name, int itemIdx, int potionValue, int cheapPrice, int reasonablePrice, int expensivePrice, int outrageousPrice, int maxCount)
{
	//전체 변수값 0으로 초기화 
	memset(&_item, 0, sizeof(_item));

	_item.name = name;
	_item.imgItem = IMAGEMANAGER->findImage(itemKey);
	_item.imgName = IMAGEMANAGER->findImage(nameKey);
	_item.imgDes = IMAGEMANAGER->findImage(desKey);

	_item.type = ITEM_POTION;
	_item.potionValue = potionValue;
	_item.priceRange[PRICE_CHEAP] = cheapPrice;
	_item.priceRange[PRICE_REASONABLE] = reasonablePrice;
	_item.priceRange[PRICE_EXPENSIVE] = expensivePrice;
	_item.priceRange[PRICE_OUTRAGEOUS] = outrageousPrice;

	_item.itemIdx = itemIdx;
	_item.count = 1;
	_item.maxCount = maxCount;
	_item.holeAlpha = 255;
	_item.initSpeed = 3.f;

	return S_OK;
}

//장비 아이템 초기화 
HRESULT gameItem::init(string itemKey, string nameKey, string desKey, const char* name, ITEM type, int itemIdx, int hpValue, int atkValue, int defValue, int spdValue, int cheapPrice, int reasonablePrice, int expensivePrice, int outrageousPrice, int maxCount)
{
	//전체 변수값 0으로 초기화 
	memset(&_item, 0, sizeof(_item));

	_item.name = name;
	_item.imgItem = IMAGEMANAGER->findImage(itemKey);
	_item.imgName = IMAGEMANAGER->findImage(nameKey);
	_item.imgDes = IMAGEMANAGER->findImage(desKey);

	_item.type = type;
	_item.ability[ABILITY_HP] = hpValue;
	_item.ability[ABILITY_ATK] = atkValue;
	_item.ability[ABILITY_DEF] = defValue;
	_item.ability[ABILITY_SPD] = spdValue;

	_item.priceRange[PRICE_CHEAP] = cheapPrice;
	_item.priceRange[PRICE_REASONABLE] = reasonablePrice;
	_item.priceRange[PRICE_EXPENSIVE] = expensivePrice;
	_item.priceRange[PRICE_OUTRAGEOUS] = outrageousPrice;

	_item.itemIdx = itemIdx;
	_item.count = 1;
	_item.maxCount = maxCount;
	_item.holeAlpha = 255;
	_item.initSpeed = 3.f;

	return S_OK;
}

//클래스 복사해서 초기화 
HRESULT gameItem::init(gameItem * copyItem)
{
	_item = copyItem->getItem();
	_item.initAngle = RANDOM->range(PI2);
	return S_OK;
}

void gameItem::render(HDC hdc)
{
	switch (_item.moveState)
	{
	case ITEM_FALL:
		//CAMERAMANAGER->AlphaRender(hdc, _item.imgItem, _item.rc.left, _item.rc.top,_item.holeAlpha);
		CAMERAMANAGER->ZorderAlphaRender(_item.imgItem, _item.itemPos.y, _item.rc.left, _item.rc.top, _item.holeAlpha);
		break;
	default:
		//CAMERAMANAGER->Render(hdc, _item.imgItem, _item.rc.left, _item.rc.top);
		CAMERAMANAGER->ZorderRender(_item.imgItem, _item.itemPos.y, _item.rc.left, _item.rc.top);
		break;
	}
}

void gameItem::moveUpdate(float x, float y)
{
	switch (_item.moveState)
	{
	case ITEM_INIT:
	{
		_item.itemPos.x += cosf(_item.initAngle) * _item.initSpeed;
		_item.itemPos.y -= sinf(_item.initAngle) * _item.initSpeed;

		_item.initSpeed -= 0.1f;
		if (_item.initSpeed <= 0)
		{
			_item.moveState = ITEM_CHECKSTATE;
		}
	}
	break;
	case ITEM_FALL:
		_item.holeAlpha -= 3;
		if (_item.holeAlpha <= 0)
		{
			_item.isPop = true;
		}
		if (getDistance(_item.itemPos.x, _item.itemPos.y, x, y) < 100 && getDistance(_item.itemPos.x, _item.itemPos.y, x, y) > 7)
		{
			_item.holeAlpha = 255;
			_item.moveState = ITEM_FOLLOW;
		}
		break;
	case ITEM_FOLLOW:
	{
		if (getDistance(_item.itemPos.x, _item.itemPos.y, x, y) < 100 && getDistance(_item.itemPos.x, _item.itemPos.y, x, y) > 7)
		{
			float angle = getAngle(_item.itemPos.x, _item.itemPos.y, x, y);
			_item.itemPos.x += cosf(angle) * 2.f;
			_item.itemPos.y -= sinf(angle) * 2.f;
		}

		if (getDistance(_item.itemPos.x, _item.itemPos.y, x, y) >= 100)
		{
			_item.holeAlpha = 255;
			_item.moveState = ITEM_CHECKSTATE;
		}
	}
	break;
	case ITEM_IDLE:
	{
		if (getDistance(_item.itemPos.x, _item.itemPos.y, x, y) < 100 && getDistance(_item.itemPos.x, _item.itemPos.y, x, y) > 7)
		{
			_item.moveState = ITEM_FOLLOW;
		}
	}
	break;
	case ITEM_CHECKSTATE:
	{
	}
	break;
	default:
		break;
	}
	_item.rc = RectMakeCenter(_item.itemPos.x, _item.itemPos.y, 35, 35);
}

bool gameItem::checkCollision(RECT rc)
{
	RECT tempRect;

	if (IntersectRect(&tempRect, &_item.rc, &rc)) return true;
	else false;
}

//=============================================================
//	## itemManager ## (아이템매니져)
//=============================================================

HRESULT itemManager::init()
{
	this->addGameItems();

	return S_OK;
}

void itemManager::release()
{
}

void itemManager::update()
{
}

void itemManager::render(HDC hdc)
{
}

//일반 아이템 추가 
void itemManager::addNormalItem(string itemKey, string nameKey, string desKey, const char* name, int itemIdx, int cheapPrice, int reasonablePrice, int expensivePrice, int outrageousPrice, int maxCount)
{
	gameItem *item = new gameItem;
	item->init(itemKey, nameKey, desKey, name, itemIdx,
		cheapPrice, reasonablePrice, expensivePrice, outrageousPrice, maxCount);

	_vItem.push_back(item);
}

//포션 아이템 추가 
void itemManager::addPotionItem(string itemKey, string nameKey, string desKey, const char* name, int itemIdx, int potionValue, int cheapPrice, int reasonablePrice, int expensivePrice, int outrageousPrice, int maxCount)
{
	gameItem *item = new gameItem;
	item->init(itemKey, nameKey, desKey, name, itemIdx, potionValue,
		cheapPrice, reasonablePrice, expensivePrice, outrageousPrice, maxCount);

	_vItem.push_back(item);
}

//장비 아이템 추가 
void itemManager::addEquipmentItem(string itemKey, string nameKey, string desKey, const char* name, ITEM type, int itemIdx, int hpValue, int atkValue, int defValue, int spdValue, int cheapPrice, int reasonablePrice, int expensivePrice, int outrageousPrice, int maxCount)
{
	gameItem *item = new gameItem;
	item->init(itemKey, nameKey, desKey, name, type,
		itemIdx, hpValue, atkValue, defValue, spdValue,
		cheapPrice, reasonablePrice, expensivePrice, outrageousPrice, maxCount);

	_vItem.push_back(item);
}

void itemManager::csvItemDataLoad(string path, CSVITEMTYPE _type)
{
	FILE* fp = fopen(path.c_str(), "rt");

	vector<string> column;
	vector<string> DataType;

	CSVData::parse(fp, column);
	CSVData::parse(fp, DataType);

	vector<string> Data;
	int cnt = 1;
	while (CSVData::parse(fp, Data))
	{
		::Item data;

		data.FillData(Data, _type);

		m_itemData.insert(make_pair(cnt, data));
		cnt++;
		Data.clear();
	}

	fclose(fp);

	for (int i = 1; i <= m_itemData.size(); i++)
	{
		switch (_type)
		{
		case CSV_NOMAL:
			addNormalItem(m_itemData[i].getItemKey(), m_itemData[i].getNameKey(), m_itemData[i].getDesKey(), m_itemData[i].getName().c_str(), m_itemData[i].getItemIdx(), m_itemData[i].getCheapPrice(), m_itemData[i].getReasonablePrice(), m_itemData[i].getExpensivePrice(), m_itemData[i].getOutrageousPrice(), m_itemData[i].getMaxCount());
			break;
		case CSV_POTION:
			addPotionItem(m_itemData[i].getItemKey(), m_itemData[i].getNameKey(), m_itemData[i].getDesKey(), m_itemData[i].getName().c_str(), m_itemData[i].getItemIdx(), m_itemData[i].getpotionValue(), m_itemData[i].getCheapPrice(), m_itemData[i].getReasonablePrice(), m_itemData[i].getExpensivePrice(), m_itemData[i].getOutrageousPrice(), m_itemData[i].getMaxCount());
			break;
		case CSV_WEAPON:
			addEquipmentItem(m_itemData[i].getItemKey(), m_itemData[i].getNameKey(), m_itemData[i].getDesKey(), m_itemData[i].getName().c_str(), ITEM_WEAPON, m_itemData[i].getItemIdx(), m_itemData[i].gethpValue(), m_itemData[i].getatkValue(), m_itemData[i].getdefValue(), m_itemData[i].getspdValue(), m_itemData[i].getCheapPrice(), m_itemData[i].getReasonablePrice(), m_itemData[i].getExpensivePrice(), m_itemData[i].getOutrageousPrice(), m_itemData[i].getMaxCount());
			break;
		default:
			break;
		}
	}
	m_itemData.clear();

}

//게임에 사용하는 아이템들 추가하는 함수 
void itemManager::addGameItems()
{

	csvItemDataLoad("CSVdata/itemData_nomal.csv", CSVITEMTYPE::CSV_NOMAL);
	csvItemDataLoad("CSVdata/itemData_potion.csv", CSVITEMTYPE::CSV_POTION);
	csvItemDataLoad("CSVdata/itemData_weapon.csv", CSVITEMTYPE::CSV_WEAPON);

}

gameItem itemManager::getItemByIdx(int idx)
{
	for (int i = 0; i < _vItem.size(); i++)
	{
		if (_vItem[i]->getItemIdx() != idx) continue;

		return *(_vItem[i]);
	}
}
