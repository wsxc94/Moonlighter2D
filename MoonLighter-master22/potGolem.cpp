#include "stdafx.h"
#include "enemy.h"

HRESULT potGolem::init(int x, int y)
{
	_attack = new animation;
	_attackHit = new animation;
	_die = new animation;

	_attack->init(IMAGEMANAGER->findImage("항아리"), 1, 5);
	_attackHit->init(IMAGEMANAGER->findImage("항아리Hit"), 1, 5);
	_die->init(IMAGEMANAGER->findImage("potDie"), 0, 6);

	_attack->aniStop();
	_attackHit->aniStop();

	_x = x;
	_y = y;
	_emRC = RectMakeCenter(x, y, 35, 35);

	_emDirection = EM_LEFT;
	_emState = EM_MOVE;

	_isActivate = true;
	_isHit = false;
	_isDieSoundPlay = false;
	_isFireSoundPlay = false;
	_isItemDrop = false;
	_hitCount = 0;

	_emHp = 30;
	_emAtk = 7;

	_bullet = new tagPotBullet[30];

	_isFire = false;
	for (int i = 0; i < 30; i++)
	{
		_bullet[i].x = x;
		_bullet[i].y = y;
		_bullet[i].rc = RectMakeCenter(x, y, 17, 11);
		_bullet[i].isFire = false;
	}

	_hpBar = new progressBar;
	_hpBar->init("enemyProgressBarFront", "enemyProgressBarBack", _emHp);
	_hpBarAlpha = 255;
	_isHpBarRender = true;

	//아이템 초기화
	_itemIndexSize = 4;
	_itemDropSize = RANDOM->range(3) + 1;
	_itemIndex = new int[_itemIndexSize];
	_itemNum = new int[_itemDropSize];

	//나올수 있는 아이템인덱스 초기화
	_itemIndex[0] = 4;
	_itemIndex[1] = 5;
	_itemIndex[2] = 6;
	_itemIndex[3] = 7;

	// 드랍될 아이템의 인덱스를 랜덤으로 지정
	for (int i = 0; i < _itemDropSize; i++)
	{
		_itemNum[i] = _itemIndex[RANDOM->range(_itemIndexSize)];
	}

	_emPlayerColi = false;
	count = 0;

	return S_OK;
}

void potGolem::release()
{
	SAFE_DELETE(_attack);
	SAFE_DELETE(_attackHit);
	SAFE_DELETE(_die);
	SAFE_DELETE_ARRAY(_bullet);
}

void potGolem::update()
{
	this->collision();
	if (_emHp <= 0)
	{
		_emState = EM_DIE;
	}
	this->emDirectionUpdate();
	this->aniUpdate();
	this->emBulletMove();
	this->potGolemCollision();

	_hpBar->update(_emHp);
	if (_isHpBarRender)
	{
		_hpBarAlpha -= 7;
		if (_hpBarAlpha <= 0) _isHpBarRender = false;
	}


}

void potGolem::render()
{
	this->aniRender();

	for (int i = 0; i < 30; i++)
	{
		if (!_bullet[i].isFire) continue;
		//_bullet[i].img->frameRender(getMemDC(), _bullet[i].rc.left, _bullet[i].rc.top,0,_bullet[i].frameY);
		//CAMERAMANAGER->FrameRender(getMemDC(), _bullet[i].img, _bullet[i].rc.left, _bullet[i].rc.top, 0, _bullet[i].frameY);
		CAMERAMANAGER->ZorderFrameRender(_bullet[i].img, _bullet[i].y, _bullet[i].rc.left, _bullet[i].rc.top, 0, _bullet[i].frameY);
	}
	if (_isHpBarRender) _hpBar->cameraAlphaRender(_x, _y - 50, _hpBarAlpha);
}

void potGolem::aniUpdate()
{
	switch (_emState)
	{
	case EM_MOVE:
		break;
	case EM_ATTACK:
		_attack->update();
		_attackHit->update();
		if (_attack->getAniState() == ANIMATION_END)
		{
			_isFireSoundPlay = false;
			if (!_isAttackRange)
			{
				_emState = EM_MOVE;
				_attack->aniStop();
				_attackHit->aniStop();
			}
			else
			{
				_attack->aniRestart();
				_attackHit->aniRestart();
				_isFire = false;
			}
		}
		this->emBulletFire();

		break;
	case EM_DIE:
		_die->update();
		if (_isDieSoundPlay == false)
		{
			SOUNDMANAGER->play("enemyDie");
			_isDieSoundPlay = true;
		}
		if (_die->getAniState() == ANIMATION_END)
		{
			if (this->bulletEmpty())
			{
				_isActivate = false;
			}
		}
		break;
	case EM_HIT:
		break;
	default:
		break;
	}

	if (_isHit)
	{
		_hitCount++;
		if (_hitCount > 10)
		{
			_hitCount = 0;
			_isHit = false;
		}
	}
}

void potGolem::aniRender()
{
	switch (_emState)
	{
	case EM_DIE:
		_die->ZoderRender(_y, _x - 40, _y - 80);
		break;
	default:
		switch (_emDirection)
		{
		case EM_LEFT:
			if (_isHit)
			{
				_attackHit->setFrameY(1);
				_attackHit->ZoderRender(_y, _emRC.left - 10, _emRC.top - 20);
			}
			else
			{
				_attack->setFrameY(1);
				_attack->ZoderRender(_y, _emRC.left - 10, _emRC.top - 20);
			}
			break;
		case EM_RIGHT:
			if (_isHit)
			{
				_attackHit->setFrameY(2);
				_attackHit->ZoderRender(_y, _emRC.left - 10, _emRC.top - 20);
			}
			else
			{
				_attack->setFrameY(2);
				_attack->ZoderRender(_y, _emRC.left - 10, _emRC.top - 20);
			}
			break;
		case EM_TOP:
			if (_isHit)
			{
				_attackHit->setFrameY(3);
				_attackHit->ZoderRender(_y, _emRC.left - 10, _emRC.top - 20);
			}
			else
			{
				_attack->setFrameY(3);
				_attack->ZoderRender(_y, _emRC.left - 10, _emRC.top - 20);
			}
			break;
		case EM_BOTTOM:
			if (_isHit)
			{
				_attackHit->setFrameY(0);
				_attackHit->ZoderRender(_y, _emRC.left - 10, _emRC.top - 20);
			}
			else
			{
				_attack->setFrameY(0);
				_attack->ZoderRender(_y, _emRC.left - 10, _emRC.top - 20);
			}
			break;
		}
		break;
	}

}

void potGolem::emDirectionUpdate()
{
	_attackRangeL = RectMake(_emRC.left - WINSIZEX, _emRC.top, WINSIZEX, 35);
	_attackRangeR = RectMake(_emRC.right, _emRC.top, WINSIZEX, 35);
	_attackRangeT = RectMake(_emRC.left, _emRC.top - WINSIZEY, 35, WINSIZEY);
	_attackRangeB = RectMake(_emRC.left, _emRC.bottom, 35, WINSIZEY);
}

void potGolem::emBulletFire()
{
	if (_attack->getCurIndex() == 8 && !_isFire)	// 발사 이미지랑 맞을때
	{
		if (_isFireSoundPlay == false)
		{
			SOUNDMANAGER->play("potFire");
			_isFireSoundPlay = true;
		}
		for (int i = 0; i < 30; i++)
		{
			if (_bullet[i].isFire) continue;
			switch (_emDirection)
			{
			case EM_LEFT:
				_bullet[i].direction = BM_LEFT;
				_bullet[i].x = _emRC.left;
				_bullet[i].y = _y;
				_bullet[i].angle = PI;
				_bullet[i].rc = RectMakeCenter(_bullet[i].x, _bullet[i].y, 17, 11);
				_bullet[i].img = IMAGEMANAGER->findImage("항아리총알W");
				_bullet[i].frameY = 0;
				break;
			case EM_RIGHT:
				_bullet[i].direction = BM_RIGHT;
				_bullet[i].x = _emRC.right;
				_bullet[i].y = _y;
				_bullet[i].angle = 0;
				_bullet[i].rc = RectMakeCenter(_bullet[i].x, _bullet[i].y, 17, 11);
				_bullet[i].img = IMAGEMANAGER->findImage("항아리총알W");
				_bullet[i].frameY = 1;
				break;
			case EM_TOP:
				_bullet[i].direction = BM_UP;
				_bullet[i].x = _x;
				_bullet[i].y = _emRC.top;
				_bullet[i].angle = PI / 2;
				_bullet[i].rc = RectMakeCenter(_bullet[i].x, _bullet[i].y, 11, 17);
				_bullet[i].img = IMAGEMANAGER->findImage("항아리총알H");
				_bullet[i].frameY = 0;
				break;
			case EM_BOTTOM:
				_bullet[i].direction = BM_DOWN;
				_bullet[i].x = _x;
				_bullet[i].y = _emRC.bottom;
				_bullet[i].angle = PI / 2 * 3;
				_bullet[i].rc = RectMakeCenter(_bullet[i].x, _bullet[i].y, 11, 17);
				_bullet[i].img = IMAGEMANAGER->findImage("항아리총알H");
				_bullet[i].frameY = 1;
				break;
			default:
				break;
			}
			_bullet[i].isFire = true;
			_isFire = true;				// 한번만발사
			break;
		}
	}


}

void potGolem::emBulletMove()
{
	for (int i = 0; i < 30; i++)
	{
		if (!_bullet[i].isFire) continue;
		_bullet[i].x += cosf(_bullet[i].angle) * 4;
		_bullet[i].y -= sinf(_bullet[i].angle) * 4;
		switch (_bullet[i].direction)
		{
		case potGolem::BM_UP:
			_bullet[i].rc = RectMakeCenter(_bullet[i].x, _bullet[i].y, 11, 17);
			break;
		case potGolem::BM_DOWN:
			_bullet[i].rc = RectMakeCenter(_bullet[i].x, _bullet[i].y, 11, 17);
			break;
		case potGolem::BM_LEFT:
			_bullet[i].rc = RectMakeCenter(_bullet[i].x, _bullet[i].y, 17, 11);
			break;
		case potGolem::BM_RIGHT:
			_bullet[i].rc = RectMakeCenter(_bullet[i].x, _bullet[i].y, 17, 11);
			break;
		}
	}



	for (int i = 0; i < 30; i++)
	{
		if (!_bullet[i].isFire) continue;
		RECT temp;
		RECT sour = RectMake(140, 105, 35 * _idx, 35 * _idy);
		if (IntersectRect(&temp, &_bullet[i].rc, &sour)) continue;

		int wid = temp.right - temp.left;
		int hei = temp.bottom - temp.top;
		int bWid = _bullet[i].rc.right - _bullet[i].rc.left;
		int bHei = _bullet[i].rc.bottom - _bullet[i].rc.top;

		if (wid != bWid || hei != bHei)
		{
			_bullet[i].isFire = false;
			SOUNDMANAGER->play("potBulletPop", 0.5f);
			break;
		}
	}

	//총알 오브젝트랑 충돌
	for (int i = 0; i < 30; i++)
	{
		if (!_bullet[i].isFire) continue;
		for (int x = 0; x < _idx; x++)
		{
			for (int y = 0; y < _idy; y++)
			{
				RECT rc;
				if (IntersectRect(&rc, &_totalNode[y][x].rc, &_bullet[i].rc))
				{
					if (_totalNode[y][x].nodeState == NODE_WALL)
					{
						_bullet[i].isFire = false;
						SOUNDMANAGER->play("potBulletPop", 0.5f);
					}
				}
			}
		}
	}
}

bool potGolem::bulletEmpty()
{
	for (int i = 0; i < 30; i++)
	{
		if (_bullet[i].isFire) return false;
	}
	return true;
}

void potGolem::isAttackRange(RECT rc)
{
	if (_attack->getAniState() == ANIMATION_END)
	{
		float angle = getAngle(_x, _y, (rc.left + rc.right) / 2, (rc.top + rc.bottom) / 2);
		if (angle > DEGREE(45) && angle <= DEGREE(135))
		{
			_emDirection = EM_TOP;
		}
		if (angle > DEGREE(135) && angle <= DEGREE(225))
		{
			_emDirection = EM_LEFT;
		}
		if (angle > DEGREE(225) && angle <= DEGREE(315))
		{
			_emDirection = EM_BOTTOM;
		}
		if (angle > DEGREE(315) || angle <= DEGREE(45))
		{
			_emDirection = EM_RIGHT;
		}
	}

	RECT temp;
	if (IntersectRect(&temp, &rc, &_attackRangeL))
	{
		if (_attack->getCurIndex() == _attack->getAniList().size() - 1) _emDirection = EM_LEFT;
		_emState = EM_ATTACK;
		_isAttackRange = true;
		return;
	}
	if (IntersectRect(&temp, &rc, &_attackRangeR))
	{
		if (_attack->getCurIndex() == _attack->getAniList().size() - 1) _emDirection = EM_RIGHT;
		_emState = EM_ATTACK;
		_isAttackRange = true;
		return;
	}
	if (IntersectRect(&temp, &rc, &_attackRangeT))
	{
		if (_attack->getCurIndex() == _attack->getAniList().size() - 1) _emDirection = EM_TOP;
		_emState = EM_ATTACK;
		_isAttackRange = true;
		return;
	}
	if (IntersectRect(&temp, &rc, &_attackRangeB))
	{
		if (_attack->getCurIndex() == _attack->getAniList().size() - 1) _emDirection = EM_BOTTOM;
		_emState = EM_ATTACK;
		_isAttackRange = true;
		return;
	}

	_isAttackRange = false;

}

void potGolem::setWallTile(vector<tagTile> vTile)
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
					if (vTile[i].tState == TS_MOVEBAN)
					{
						_totalNode[y][x].nodeState = NODE_WALL;
					}
				}
			}
		}

	}
}

void potGolem::hitSoundPlay()
{
	SOUNDMANAGER->play("golemHit", 0.5f);
}

void potGolem::potGolemCollision()
{
	collisionEnemyBody();
	RECT temp;

	for (int i = 0; i < 30; i++)
	{
		if (IntersectRect(&temp, &PLAYER->getRect(), &_bullet[i].rc) && _bullet[i].isFire)
		{

			PLAYERDATA->setInDungeonHp(PLAYERDATA->getInDungeonHp() - _emAtk);
			if (PLAYERDATA->getInDungeonHp() < 0)
			{
				PLAYERDATA->setInDungeonHp(1);
			}
			_bullet[i].isFire = false;
		}
	}

}
