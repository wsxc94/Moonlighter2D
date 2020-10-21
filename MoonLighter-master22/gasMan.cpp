#include "stdafx.h"
#include "enemy.h"

HRESULT gasMan::init(int x, int y)
{
	_move = new animation;
	_moveHit = new animation;
	_attack = new animation;
	_attackHit = new animation;
	_die = new animation;

	_move->init(IMAGEMANAGER->findImage("가스맨"), 0, 6, true);
	_moveHit->init(IMAGEMANAGER->findImage("가스맨피격"), 0, 6, true);
	_attack->init(IMAGEMANAGER->findImage("가스맨공격"), 0, 6);
	_attackHit->init(IMAGEMANAGER->findImage("가스맨공격피격"), 0, 6);
	_die->init(IMAGEMANAGER->findImage("enemyDie"), 0, 6);

	_x = x;
	_y = y;
	_aStarRC = RectMakeCenter(_x, _y, 35, 35);
	_emRC = RectMakeCenter(_x, _y - 20, 50, 80);

	_emState = EM_MOVE;
	_emDirection = EM_LEFT;

	_emHp = 40;
	_emAtk = 13;

	_isActivate = true;
	_isAttackRange = false;
	_isHit = false;
	_isFire = false;
	_isDieSoundPlay = false;
	_isFireSoundPlay = false;
	_isItemDrop = false;
	_knockBackSpeed = 2;
	_hitCount = 0;

	_hpBar = new progressBar;
	_hpBar->init("enemyProgressBarFront", "enemyProgressBarBack", _emHp);
	_hpBarAlpha = 255;
	_isHpBarRender = true;

	_itemIndexSize = 2;
	_itemDropSize = RANDOM->range(3) + 1;
	_itemIndex = new int[_itemIndexSize];
	_itemNum = new int[_itemDropSize];

	//나올수 있는 아이템인덱스 초기화
	_itemIndex[0] = RICHJELLY_IDX;
	_itemIndex[1] = VENOMJELLY_IDX;

	// 드랍될 아이템의 인덱스를 랜덤으로 지정
	for (int i = 0; i < _itemDropSize; i++)
	{
		_itemNum[i] = _itemIndex[RANDOM->range(_itemIndexSize)];
	}

	_emPlayerColi = false;
	count = 0;

	return S_OK;
}

void gasMan::release()
{

}

void gasMan::update()
{
	this->collision();
	if (_emHp <= 0)
	{
		_emState = EM_DIE;
	}
	_hpBar->update(_emHp);
	if (_isHpBarRender)
	{
		_hpBarAlpha -= 7;
		if (_hpBarAlpha <= 0) _isHpBarRender = false;
	}
	switch (_emState)
	{
	case EM_MOVE:
		_move->update();
		_moveHit->update();
		// 시작타일
		this->setStartNode();
		//에이스타
		this->aStar();
		// 무빙
		this->enemyMove();
		// 에너미 보는방향
		this->setEmDirection();
		// 공격범위
		this->attackRangeUpdate();
		if (_isAttackRange)
		{
			_emState = EM_ATTACK;
		}

		break;
	case EM_ATTACK:
		_attack->update();
		_attackHit->update();

		this->arrowFire();

		if (_isHit)
		{
			_hitCount++;
			if (_hitCount > 15)
			{
				_hitCount = 0;
				_isHit = false;
			}
		}

		if (_attack->getAniState() == ANIMATION_END)
		{
			_isFireSoundPlay = false;
			if (!_isAttackRange)
			{
				_attack->aniStop();
				_attackHit->aniStop();
				_emState = EM_MOVE;
			}
			else
			{
				_attack->aniRestart();
				_attackHit->aniRestart();
				_isFire = false;
			}
		}
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
			if (_vArrow.size() == 0)
			{
				_isActivate = false;
			}
		}
		break;
	case EM_HIT:
		break;
	}
	gasManCollision();
	this->arrowMove();
}

void gasMan::render()
{
	this->arrowRender();
	this->aniRender();
	if (_isHpBarRender) _hpBar->cameraAlphaRender(_x, _y - 80, _hpBarAlpha);
}

void gasMan::aniRender()
{
	switch (_emState)
	{
	case EM_MOVE:
		switch (_emDirection)
		{
		case EM_LEFT:
			if (_isHit)
			{
				_moveHit->setFrameY(0);
				_moveHit->ZoderRender(_y, _x - 65, _y - 75);
			}
			else
			{
				_move->setFrameY(0);
				_move->ZoderRender(_y, _x - 65, _y - 75);
			}
			break;
		case EM_RIGHT:
			if (_isHit)
			{
				_moveHit->setFrameY(1);
				_moveHit->ZoderRender(_y, _x - 65, _y - 75);
			}
			else
			{
				_move->setFrameY(1);
				_move->ZoderRender(_y, _x - 65, _y - 75);
			}
			break;
		case EM_TOP:
			if (_isHit)
			{
				_moveHit->setFrameY(2);
				_moveHit->ZoderRender(_y, _x - 65, _y - 75);
			}
			else
			{
				_move->setFrameY(2);
				_move->ZoderRender(_y, _x - 65, _y - 75);
			}
			break;
		case EM_BOTTOM:
			if (_isHit)
			{
				_moveHit->setFrameY(3);
				_moveHit->ZoderRender(_y, _x - 65, _y - 75);
			}
			else
			{
				_move->setFrameY(3);
				_move->ZoderRender(_y, _x - 65, _y - 75);
			}
			break;
		}
		break;
	case EM_ATTACK:
		switch (_emDirection)
		{
		case EM_LEFT:
			if (_isHit)
			{
				_attackHit->setFrameY(0);
				_attackHit->ZoderRender(_y, _x - 65, _y - 75);
			}
			else
			{
				_attack->setFrameY(0);
				_attack->ZoderRender(_y, _x - 65, _y - 75);
			}
			break;
		case EM_RIGHT:
			if (_isHit)
			{
				_attackHit->setFrameY(1);
				_attackHit->ZoderRender(_y, _x - 65, _y - 75);
			}
			else
			{
				_attack->setFrameY(1);
				_attack->ZoderRender(_y, _x - 65, _y - 75);
			}
			break;
		case EM_TOP:
			if (_isHit)
			{
				_attackHit->setFrameY(2);
				_attackHit->ZoderRender(_y, _x - 65, _y - 75);
			}
			else
			{
				_attack->setFrameY(2);
				_attack->ZoderRender(_y, _x - 65, _y - 75);
			}
			break;
		case EM_BOTTOM:
			if (_isHit)
			{
				_attackHit->setFrameY(3);
				_attackHit->ZoderRender(_y, _x - 65, _y - 75);
			}
			else
			{
				_attack->setFrameY(3);
				_attack->ZoderRender(_y, _x - 65, _y - 75);
			}
			break;
		}
		break;
	case EM_DIE:
		_die->ZoderRender(_y, _x - 60, _y - 85);
		break;
	}
}

void gasMan::attackRangeUpdate()
{
	switch (_emDirection)
	{
	case EM_LEFT:
		_attackRange = RectMakeCenter(_emRC.left - 100, (_emRC.bottom + _emRC.top) / 2 + 20, 200, 50);
		break;
	case EM_RIGHT:
		_attackRange = RectMakeCenter(_emRC.right + 100, (_emRC.bottom + _emRC.top) / 2 + 20, 200, 50);
		break;
	case EM_TOP:
		_attackRange = RectMakeCenter((_emRC.left + _emRC.right) / 2 + 10, _emRC.top - 100, 50, 200);
		break;
	case EM_BOTTOM:
		_attackRange = RectMakeCenter((_emRC.left + _emRC.right) / 2 - 10, _emRC.bottom + 100, 50, 200);
		break;
	}
}

void gasMan::setEmDirection()
{
	float angle = getAngle(_x, _y, PLAYER->getX(), PLAYER->getY());
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

void gasMan::enemyMove()
{
	if (_finalList.size() > 0 && !_isHit)
	{
		float angle = getAngle(_x, _y, _finalList.front()->centerX, _finalList.front()->centerY);
		_x += cosf(angle) * 1.5f;
		_y -= sinf(angle) * 1.5f;
	}
	if (_isHit)
	{
		_knockBackSpeed -= 0.3f;
		if (_knockBackSpeed < 0)
		{
			_isHit = false;
			_knockBackSpeed = 3.f;
		}
		switch (_emDirection)
		{
		case EM_LEFT:
			_x += _knockBackSpeed;
			break;
		case EM_RIGHT:
			_x -= _knockBackSpeed;
			break;
		case EM_TOP:
			_y += _knockBackSpeed;
			break;
		case EM_BOTTOM:
			_y -= _knockBackSpeed;
			break;
		}
	}

	_aStarRC = RectMakeCenter(_x, _y, 35, 35);
	_emRC = RectMakeCenter(_x, _y - 20, 50, 80);
}

void gasMan::setStartNode()
{
	POINT pt = { _x,_y };
	for (int x = 0; x < _idx; x++)
	{
		for (int y = 0; y < _idy; y++)
		{
			if (PtInRect(&_totalNode[y][x].rc, pt))
			{
				_startNode = &_totalNode[y][x];
			}
		}
	}
}

void gasMan::arrowFire()
{
	if (_isFire) return;
	if (_attack->getCurIndex() < 5) return;
	if (_isFireSoundPlay == false)
	{
		SOUNDMANAGER->play("potFire", 0.2f);
	}
	switch (_emDirection)
	{
	case EM_LEFT:
		_arrow.direction = AR_LEFT;
		_arrow.angle = PI;
		_arrow.x = _emRC.left;
		_arrow.y = _y;
		_arrow.rc = RectMakeCenter(_arrow.x, _arrow.y, 24, 5);
		_arrow.speed = 4.f;
		_arrow.isFire = true;
		_isFire = true;
		break;
	case EM_RIGHT:
		_arrow.direction = AR_RIGHT;
		_arrow.angle = 0;
		_arrow.x = _emRC.right;
		_arrow.y = _y;
		_arrow.rc = RectMakeCenter(_arrow.x, _arrow.y, 24, 5);
		_arrow.speed = 4.f;
		_arrow.isFire = true;
		_isFire = true;
		break;
	case EM_TOP:
		_arrow.direction = AR_UP;
		_arrow.angle = PI / 2;
		_arrow.x = _x + 20;
		_arrow.y = _emRC.top;
		_arrow.rc = RectMakeCenter(_arrow.x, _arrow.y, 5, 24);
		_arrow.speed = 4.f;
		_arrow.isFire = true;
		_isFire = true;
		break;
	case EM_BOTTOM:
		_arrow.direction = AR_DOWN;
		_arrow.angle = PI / 2 * 3;
		_arrow.x = _x - 10;
		_arrow.y = _emRC.bottom;
		_arrow.rc = RectMakeCenter(_arrow.x, _arrow.y, 5, 24);
		_arrow.speed = 4.f;
		_arrow.isFire = true;
		_isFire = true;
		break;
	}
	_vArrow.push_back(_arrow);
}

void gasMan::arrowMove()
{
	for (int i = 0; i < _vArrow.size(); i++)
	{
		if (!_vArrow[i].isFire) continue;
		_vArrow[i].x += cosf(_vArrow[i].angle) * _vArrow[i].speed;
		_vArrow[i].y -= sinf(_vArrow[i].angle) * _vArrow[i].speed;
		_vArrow[i].rc = RectMakeCenter(_vArrow[i].x, _vArrow[i].y, 24, 5);
	}

	RECT dunRC = RectMake(140, 105, 35 * _idx, 35 * _idy);

	for (int i = 0; i < _vArrow.size(); i++)
	{
		RECT temp;
		if (IntersectRect(&temp, &_vArrow[i].rc, &dunRC)) continue;
		_vArrow.erase(_vArrow.begin() + i);
		SOUNDMANAGER->play("gasArrowPop",0.2f);
	}
}

void gasMan::arrowRender()
{
	for (int i = 0; i < _vArrow.size(); i++)
	{
		if (_vArrow[i].isFire == false)	 continue;
		switch (_vArrow[i].direction)
		{
		case gasMan::AR_LEFT:
			//IMAGEMANAGER->findImage("가스화살W")->frameRender(getMemDC(), _vArrow[i].rc.left, _vArrow[i].rc.top, 1, 0);
			//CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("가스화살W"), _vArrow[i].rc.left, _vArrow[i].rc.top, 1, 0);
			CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("가스화살W"), _vArrow[i].y, _vArrow[i].rc.left, _vArrow[i].rc.top, 1, 0);
			break;
		case gasMan::AR_RIGHT:
			//IMAGEMANAGER->findImage("가스화살W")->frameRender(getMemDC(), _vArrow[i].rc.left, _vArrow[i].rc.top, 0, 0);
			//CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("가스화살W"), _vArrow[i].rc.left, _vArrow[i].rc.top, 0, 0);
			CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("가스화살W"), _vArrow[i].y, _vArrow[i].rc.left, _vArrow[i].rc.top, 0, 0);
			break;
		case gasMan::AR_UP:
			//IMAGEMANAGER->findImage("가스화살H")->frameRender(getMemDC(), _vArrow[i].rc.left, _vArrow[i].rc.top, 0, 0);
			//CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("가스화살H"), _vArrow[i].rc.left, _vArrow[i].rc.top, 0, 0);
			CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("가스화살H"), _vArrow[i].y, _vArrow[i].rc.left, _vArrow[i].rc.top, 0, 0);
			break;
		case gasMan::AR_DOWN:
			//IMAGEMANAGER->findImage("가스화살H")->frameRender(getMemDC(), _vArrow[i].rc.left, _vArrow[i].rc.top, 1, 0);
			//CAMERAMANAGER->FrameRender(getMemDC(), IMAGEMANAGER->findImage("가스화살H"), _vArrow[i].rc.left, _vArrow[i].rc.top, 1, 0);
			CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage("가스화살H"), _vArrow[i].y, _vArrow[i].rc.left, _vArrow[i].rc.top, 1, 0);
			break;
		default:
			break;
		}
	}


}

void gasMan::hitSoundPlay()
{
	SOUNDMANAGER->play("gasManHit", 0.2f);
}

void gasMan::gasManCollision()
{
	RECT temp;

	for (int i = 0; i < _vArrow.size(); i++)
	{
		if (IntersectRect(&temp, &PLAYER->getRect(), &_vArrow[i].rc) && PLAYER->getPlayerState() != PLAYER_ROLL)
		{	

			allEnemyColi(_emAtk, PLAYER->getPlayerDirection() == _arrow.direction);
			if (PLAYERDATA->getInDungeonHp() <= 0)
			{
				RESULTENEMY* em = new RESULTENEMY;
				em->attack = new animation;
				em->attack->init(_attack->getImage(), 0, 7, true);
				em->frameY = 3;
				em->scale = 1.f;
				PLAYERDATA->setKillEnemy(em);
				PLAYERDATA->setInDungeonHp(0);
				PLAYER->setPlayerState(PLAYER_DIE);
			}
			_vArrow.erase(_vArrow.begin() + i);
		}
	}
}

animation * gasMan::getAttackAnimation()
{
	return _attack;
}

int gasMan::getDownDirectionY()
{
	return 3;
}
