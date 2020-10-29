#include "stdafx.h"
#include "enemy.h"

HRESULT enemy::init(int x, int y)
{
	return E_NOTIMPL;
}

void enemy::release()
{
}

void enemy::update()
{
}

void enemy::render()
{
}

void enemy::initTileSize(int x, int y)
{
	_idx = x;
	_idy = y;
	_totalNode = new node*[y];
	for (int i = 0; i < y; i++)
	{
		_totalNode[i] = new node[x];
	}
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			_totalNode[j][i].init(i, j);
		}
	}

	for (int i = 0; i < _idy; i++)
	{
		for (int j = 0; j < _idx; j++)
		{
			POINT pt = { _x,_y };
			if (PtInRect(&_totalNode[i][j].rc, pt))
			{
				_startNode = &_totalNode[i][j];
			}
		}
	}

	_endNode = nullptr;
	_curNode = nullptr;
	_count = -1;
	_isFind = false;
	_openList.clear();
	_closeList.clear();
	_finalList.clear();

	_emPlayerColi = false;
	_coliSkillAroow = false;
	count = 0;
}

void enemy::setEndTile(int x, int y)
{
	POINT temp = { x,y };
	for (int i = 0; i < _idx; i++)
	{
		for (int j = 0; j < _idy; j++)
		{
			if (PtInRect(&_totalNode[j][i].rc, temp) && _totalNode[j][i].nodeState != NODE_WALL)
			{
				_endNode = &_totalNode[j][i];
			}
		}
	}
}

void enemy::setWallTile(vector<tagTile> vTile)
{
	for (int x = 0; x < _idx; x++)
	{
		for (int y = 0; y < _idy; y++)
		{
			if (_totalNode[y][x].nodeState == NODE_WALL)
			{
				_totalNode[y][x].nodeState = NODE_EMPTY;
			}
		}
	}
	for (int i = 0; i < vTile.size(); i++)
	{
		POINT pt;
		pt.x = (vTile[i].rc.right + vTile[i].rc.left) / 2;
		pt.y = (vTile[i].rc.top + vTile[i].rc.bottom) / 2;
		for (int x = 0; x < _idx; x++)
		{
			for (int y = 0; y < _idy; y++)
			{
				if (PtInRect(&_totalNode[y][x].rc, pt))
				{
					if (vTile[i].tState == TS_HOLE || vTile[i].tState == TS_MOVEBAN)
					{
						_totalNode[y][x].nodeState = NODE_WALL;
					}
				}
			}
		}

	}
	this->setRange(_startNode->idx, _startNode->idy);
}

void enemy::aStar()
{
	if (!_endNode) return;
	if (this->isAstarFail())return;

	_openList.clear();
	_closeList.clear();
	_finalList.clear();

	_openList.push_back(_startNode);
	while (_openList.size() > 0)
	{
		_curNode = _openList[0];

		for (int i = 1; i < _openList.size(); i++)
		{
			if (_openList[i]->F <= _curNode->F && _openList[i]->H < _curNode->H)
			{
				_curNode = _openList[i];
			}
		}

		for (int i = 0; i < _openList.size(); i++)
		{
			if (_curNode == _openList[i])
			{
				this->delOpenList(i);
				_closeList.push_back(_curNode);
			}
		}

		if (_curNode == _endNode)
		{
			node* endNode = _endNode;
			vector<node*> temp;
			while (endNode != _startNode)
			{
				temp.push_back(endNode);
				endNode = endNode->parentNode;
			}
			for (int i = temp.size() - 1; i > 0; i--)
			{
				_finalList.push_back(temp[i]);
			}
			_isFind = true;
			return;
		}
		if (_curNode == nullptr) return;
		addOpenList(_curNode->idx, _curNode->idy - 1);	//»ó
		addOpenList(_curNode->idx, _curNode->idy + 1);	//ÇÏ
		addOpenList(_curNode->idx - 1, _curNode->idy);	//ÁÂ
		addOpenList(_curNode->idx + 1, _curNode->idy);	//¿ì

		addOpenList(_curNode->idx - 1, _curNode->idy - 1);	//ÁÂ»ó
		addOpenList(_curNode->idx - 1, _curNode->idy + 1);	//ÁÂÇÏ
		addOpenList(_curNode->idx + 1, _curNode->idy - 1);	//¿ì»ó
		addOpenList(_curNode->idx + 1, _curNode->idy + 1);	//¿ìÇÏ

		if (_openList.size() == 0 && _closeList.size() > 0) return;
	}
}

void enemy::addOpenList(int idx, int idy)
{
	if (idx < 0 || idx >= _idx || idy < 0 || idy >= _idy) return;
	if (_totalNode[idy][idx].nodeState == NODE_WALL) return;
	for (int i = 0; i < _closeList.size(); i++)
	{
		if (&_totalNode[idy][idx] == _closeList[i]) return;
	}
	node* tempNode = &_totalNode[idy][idx];
	int moveCost = _curNode->G + ((_curNode->idx - idx == 0 || _curNode->idy - idy == 0) ? 10 : 14);

	if (_curNode->idx - idx != 0 || _curNode->idy - idy != 0)
	{
		int x = idx - _curNode->idx;
		int y = idy - _curNode->idy;
		if (_totalNode[idy][idx - x].nodeState == NODE_WALL)return;
		if (_totalNode[idy - y][idx].nodeState == NODE_WALL) return;
	}

	for (int i = 0; i < _openList.size(); i++)
	{
		if (_openList[i] == tempNode) return;
	}

	tempNode->G = moveCost;
	tempNode->H = (abs(tempNode->idx - _endNode->idx) + abs(tempNode->idy - _endNode->idy)) * 10;
	tempNode->F = tempNode->G + tempNode->H;
	tempNode->parentNode = _curNode;
	_openList.push_back(tempNode);
}

void enemy::delOpenList(int arr)
{
	_openList.erase(_openList.begin() + arr);
}

void enemy::isAttackRange(RECT rc)
{
	RECT temp;
	if (IntersectRect(&temp, &rc, &_attackRange))
	{
		_isAttackRange = true;
	}
	else _isAttackRange = false;
}

bool enemy::isAstarFail()
{
	if (!_endNode) return true;

	if(_emKind != EM_BOSS_SKELETON && _emKind != EM_SKULLHAMMER)
	if (getDistance(_x, _y, PLAYER->getX(), PLAYER->getY()) > 250) return true;

	for (int i = 0; i < _rangeList.size(); i++)
	{
		POINT pt = { _endNode->centerX,_endNode->centerY };
		if (PtInRect(&_rangeList[i]->rc, pt))
			return false;
	}
	return true;
}

void enemy::setRange(int x, int y)
{
 	if (x < 0 || x >= _idx || y < 0 || y >= _idy) return;
	if (_totalNode[y][x].nodeState == NODE_WALL) return;
	if (_totalNode[y][x].isRange) return;
	if (_totalNode[y][x].nodeState != NODE_WALL)
	{
		_totalNode[y][x].isRange = true;
		_rangeList.push_back(&_totalNode[y][x]);
	}
	setRange(x + 1, y);
	setRange(x - 1, y);
	setRange(x, y+1);
	setRange(x, y-1);
}

void enemy::collision()
{
	int damage = RANDOM->range(PLAYERDATA->getAtk() - 2, PLAYERDATA->getAtk() + 2);
	RECT temp;
	if (IntersectRect(&temp, &PLAYER->getPlayerAttackBox().rc, &_emRC))
	{
		if (PLAYER->getPlayerAttackBox().isHit == false && _emState != EM_DIE)
		{
			EFFECTMANAGER->addEffect("°ø°ÝÀÌÆåÆ®", 2000,
				(_emRC.right + _emRC.left) / 2,
				(_emRC.bottom + _emRC.top) / 2, PLAYER->getPlayerDirection(), 10);		
			DAMAGEFONT->init(_x, _y - 30, damage);
			_emHp -= damage;
			_isHit = true;
			this->setProgressBar();
			this->hitSoundPlay();
		}
	}

	if (IntersectRect(&temp, &PLAYER->getPlayerAttackTwoBox().rc, &_emRC))
	{
		if (PLAYER->getPlayerAttackTwoBox().isHit == false && _emState != EM_DIE)
		{
			EFFECTMANAGER->addEffect("°ø°ÝÀÌÆåÆ®", 2000,
				(_emRC.right + _emRC.left) / 2,
				(_emRC.bottom + _emRC.top) / 2, PLAYER->getPlayerDirection(), 10);
			DAMAGEFONT->init(_x, _y - 30, damage);
			_emHp -= damage;
			_isHit = true;
			this->setProgressBar();
			this->hitSoundPlay();
	
		}
	}

	if (IntersectRect(&temp, &PLAYER->getArrow()->getRect(), &_emRC) && PLAYER->getArrow()->getIsShoot())
	{
		if (!_coliSkillAroow)
		{
			_coliSkillAroow = true;
			EFFECTMANAGER->addEffect("È­»ìÀÌÆåÆ®", (_emRC.bottom + _emRC.top) / 2 + 3,
				(_emRC.right + _emRC.left) / 2,
				(_emRC.bottom + _emRC.top) / 2, PLAYER->getPlayerDirection(), 10);
			DAMAGEFONT->init(_x, _y - 30, damage);

			if (PLAYER->getSkill())
			{
				_emHp -= (10 + damage);
				_isHit = true;
				this->setProgressBar();
				this->hitSoundPlay();
			}
			else
			{
				_emHp -= damage;
				_isHit = true;
				this->setProgressBar();
				this->hitSoundPlay();
				PLAYER->getArrow()->setIsShoot(false);
			}
		}	
	}
	else
	{
		_coliSkillAroow = false;
	}

}

void enemy::soundUpdate()
{
}

void enemy::setProgressBar()
{
	_hpBarAlpha = 255;
	_isHpBarRender = true;
}

void enemy::hitSoundPlay()
{
}

bool enemy::checkDirection()
{
	if (PLAYER->getPlayerDirection() == _emDirection)
	{
		return true;
	}
	return false;
}

void enemy::allEnemyColi(int emDemage, bool checkDirection)
{
	if (PLAYER->getPlayerState() == PLAYER_SHILED && checkDirection)
	{
		PLAYER->playerPush();
	}
	else
	{
		PLAYERDATA->setInDungeonHp(PLAYERDATA->getInDungeonHp() - emDemage);
		PLAYER->setPlayerState(HIT_IDLE);
		PLAYER->setHit(true);
		DAMAGEFONT->init(PLAYER->getX(), PLAYER->getY(), emDemage);
	}
}

void enemy::aStarRender()
{
	if (INPUT->GetToggleKey(VK_F2))
	{
		for (int i = 0; i < _idy; i++)
		{
			for (int j = 0; j < _idx; j++)
			{
				if (_totalNode[i][j].nodeState == NODE_WALL)
					CAMERAMANAGER->FrameRect(getMemDC(), _totalNode[i][j].rc, RGB(255, 255, 0));
			}
		}
		if (_startNode)
			CAMERAMANAGER->FrameRect(getMemDC(), _startNode->rc, RGB(0, 0, 255));
		if (_endNode)
			CAMERAMANAGER->FrameRect(getMemDC(), _endNode->rc, RGB(255, 0, 0));

		for (int i = 0; i < _finalList.size(); i++)
		{
			CAMERAMANAGER->FrameRect(getMemDC(), _finalList[i]->rc, RGB(0, 255, 0));
		}
	}
}


