#include "stdafx.h"
#include "enemy.h"

HRESULT redGolem::init(int x, int y)
{
	_move = new animation;
	_attack = new animation;
	_moveHit = new animation;
	_attackHit = new animation;
	_die = new animation;

	_move->init(IMAGEMANAGER->findImage("red골렘전사"), 0, 6, true);
	_attack->init(IMAGEMANAGER->findImage("red골렘공격"), 0, 6);
	_moveHit->init(IMAGEMANAGER->findImage("골렘전사피격"), 0, 6, true);
	_attackHit->init(IMAGEMANAGER->findImage("골렘공격피격"), 0, 6);
	_die->init(IMAGEMANAGER->findImage("enemyDie"), 0, 6);

	_emKind = EM_GOLEM;
	_emState = EM_MOVE;
	_emDirection = EM_LEFT;

	_isActivate = true;
	_isAttackRange = false;
	_isHit = false;
	_isAttackSoundPlay = false;
	_isDieSoundPlay = false;
	_isItemDrop = false;
	_emRC = RectMakeCenter(x, y - 20, 50, 80);

	_knockBackSpeed = 3.f;

	_emHp = 75;
	_emAtk = 15;

	_aStarRC = RectMakeCenter(x, y - 20, 35, 35);
	_x = x;
	_y = y;


	_hpBar = new progressBar;
	_hpBar->init("enemyProgressBarFront", "enemyProgressBarBack", _emHp);
	_hpBarAlpha = 255;
	_isHpBarRender = true;

	_itemIndexSize = 3;
	_itemDropSize = RANDOM->range(3) + 1;
	_itemIndex = new int[_itemIndexSize];
	_itemNum = new int[_itemDropSize];

	//나올수 있는 아이템인덱스 초기화
	_itemIndex[0] = 3;
	_itemIndex[1] = 8;
	_itemIndex[2] = 11;

	// 드랍될 아이템의 인덱스를 랜덤으로 지정
	for (int i = 0; i < _itemDropSize; i++)
	{
		_itemNum[i] = _itemIndex[RANDOM->range(_itemIndexSize)];
	}

	_emPlayerColi = false;
	count = 0;

	return S_OK;
}

void redGolem::release()
{
	SAFE_DELETE(_move);
	SAFE_DELETE(_attack);
	SAFE_DELETE(_moveHit);
	SAFE_DELETE(_attackHit);
	SAFE_DELETE(_die);
}

void redGolem::update()
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
		this->aStar();

		this->golemMove();

		if (_isAttackRange)
		{
			_emState = EM_ATTACK;
		}

		_attackBox = RectMake(0, 0, 0, 0);
		break;
	case EM_ATTACK:
		_attack->update();
		_attackHit->update();
		this->attackBox();
		if (!_isAttackRange)
		{
			if (this->attackAniStop())
			{
				this->attackAniStop();
				_emState = EM_MOVE;
			}
		}
		this->attackAniPlay();

		if (_isHit)
		{
			// 스피드를 카운트처럼 사용한것
			_knockBackSpeed -= 0.5f;
			if (_knockBackSpeed < 0)
			{
				_isHit = false;
				_knockBackSpeed = 3.f;
			}
		}

		break;
	case EM_DIE:
		_die->update();
		if (_isDieSoundPlay == false)
		{
			SOUNDMANAGER->play("golemDie");
			_isDieSoundPlay = true;
		}
		if (_die->getAniState() == ANIMATION_END)
		{
			_isActivate = false;
		}
		break;
	}

	// 골렘의 위치를 항상 에이스타 시작지점으로 한다
	this->setStartNode();

	directionUpdate();
	redGolemCollision();

	_emRC = RectMakeCenter(_x, _y - 20, 50, 80);
	_aStarRC = RectMakeCenter(_x, _y - 20, 35, 35);
}

void redGolem::render()
{
	switch (_emState)
	{
	case EM_MOVE:
		this->moveRender();
		break;
	case EM_ATTACK:
		this->attackRender();
		break;
	case EM_DIE:
		_die->ZoderRender(_y, _x - 60, _y - 95);
		break;
	}
	if (_isHpBarRender) _hpBar->cameraAlphaRender(_x, _y - 70, _hpBarAlpha);
}

void redGolem::directionUpdate()
{
	switch (_emDirection)
	{
	case EM_LEFT:
		_attackRange = RectMake(_emRC.left - 40, _emRC.top, 40, 80);
		break;
	case EM_RIGHT:
		_attackRange = RectMake(_emRC.right, _emRC.top, 40, 80);
		break;
	case EM_TOP:
		_attackRange = RectMake(_emRC.left, _emRC.top - 20, 50, 20);
		break;
	case EM_BOTTOM:
		_attackRange = RectMake(_emRC.left, _emRC.bottom, 50, 40);
		break;
	}
}

void redGolem::moveRender()
{
	switch (_emDirection)
	{
	case EM_LEFT:
		if (_isHit == true)
		{
			_moveHit->setFrameY(0);
			_moveHit->ZoderRender(_y, _emRC.left - 70, _emRC.top - 55);
		}
		else
		{
			_move->setFrameY(0);
			_move->ZoderRender(_y, _emRC.left - 70, _emRC.top - 55);
		}
		break;
	case EM_RIGHT:
		if (_isHit == true)
		{
			_moveHit->setFrameY(1);
			_moveHit->ZoderRender(_y, _emRC.left - 70, _emRC.top - 55);
		}
		else
		{
			_move->setFrameY(1);
			_move->ZoderRender(_y, _emRC.left - 70, _emRC.top - 55);
		}
		break;
	case EM_TOP:
		if (_isHit == true)
		{
			_moveHit->setFrameY(2);
			_moveHit->ZoderRender(_y, _emRC.left - 70, _emRC.top - 55);
		}
		else
		{
			_move->setFrameY(2);
			_move->ZoderRender(_y, _emRC.left - 70, _emRC.top - 55);
		}
		break;
	case EM_BOTTOM:
		if (_isHit == true)
		{
			_moveHit->setFrameY(3);
			_moveHit->ZoderRender(_y, _emRC.left - 70, _emRC.top - 55);
		}
		else
		{
			_move->setFrameY(3);
			_move->ZoderRender(_y, _emRC.left - 70, _emRC.top - 55);
		}
		break;
	}
}

void redGolem::attackRender()
{
	switch (_emDirection)
	{
	case EM_LEFT:
		if (_isHit)
		{
			_attackHit->setFrameY(0);
			_attackHit->ZoderRender(_y, _emRC.left - 70, _emRC.top - 35);
		}
		else
		{
			_attack->setFrameY(0);
			_attack->ZoderRender(_y, _emRC.left - 70, _emRC.top - 35);
		}
		break;
	case EM_RIGHT:
		if (_isHit)
		{
			_attackHit->setFrameY(1);
			_attackHit->ZoderRender(_y, _emRC.left - 70, _emRC.top - 35);
		}
		else
		{
			_attack->setFrameY(1);
			_attack->ZoderRender(_y, _emRC.left - 70, _emRC.top - 35);
		}
		break;
	case EM_TOP:
		if (_isHit)
		{
			_attackHit->setFrameY(2);
			_attackHit->ZoderRender(_y, _emRC.left - 70, _emRC.top - 35);
		}
		else
		{
			_attack->setFrameY(2);
			_attack->ZoderRender(_y, _emRC.left - 70, _emRC.top - 35);
		}
		break;
	case EM_BOTTOM:
		if (_isHit)
		{
			_attackHit->setFrameY(3);
			_attackHit->ZoderRender(_y, _emRC.left - 70, _emRC.top - 35);
		}
		else
		{
			_attack->setFrameY(3);
			_attack->ZoderRender(_y, _emRC.left - 70, _emRC.top - 35);
		}
		break;
	}
}

void redGolem::attackAniPlay()
{
	if (_attack->getAniState() == ANIMATION_END)
	{
		_attack->aniRestart();
		_attackHit->aniRestart();
		_isAttackSoundPlay = false;
	}
}

bool redGolem::attackAniStop()
{
	if (_attack->getAniState() == ANIMATION_END)
	{
		_attack->aniStop();
		_attackHit->aniStop();
		return true;
	}
	return false;
}

void redGolem::attackBox()
{
	switch (_emDirection)
	{
	case EM_LEFT:
		if (_attack->getCurIndex() > 6) _attackBox = RectMake(_emRC.left - 60, _emRC.top, 60, 80);
		if (_attack->getCurIndex() < 6) _attackBox = RectMake(0, 0, 0, 0);
		break;
	case EM_RIGHT:
		if (_attack->getCurIndex() > 6) _attackBox = RectMake(_emRC.right, _emRC.top, 60, 80);
		if (_attack->getCurIndex() < 6) _attackBox = RectMake(0, 0, 0, 0);
		break;
	case EM_TOP:
		if (_attack->getCurIndex() > 6) _attackBox = RectMake(_emRC.left, _emRC.top - 20, 50, 20);
		if (_attack->getCurIndex() < 6) _attackBox = RectMake(0, 0, 0, 0);
		break;
	case EM_BOTTOM:
		if (_attack->getCurIndex() > 6) _attackBox = RectMake(_emRC.left, _emRC.bottom, 50, 50);
		if (_attack->getCurIndex() < 6) _attackBox = RectMake(0, 0, 0, 0);
		break;
	}
	if (_attack->getCurIndex() > 6 && _isAttackSoundPlay == false)
	{
		SOUNDMANAGER->play("golemAttack");
		_isAttackSoundPlay = true;
	}
}

void redGolem::golemMove()
{
	if (_finalList.size() > 0)
	{
		if (!_isHit)
		{
			_x += cosf(getAngle(_x, _y, (_finalList.front()->rc.left + _finalList.front()->rc.right) / 2, (_finalList.front()->rc.top + _finalList.front()->rc.bottom) / 2)) * 1.5;
			_y -= sinf(getAngle(_x, _y, (_finalList.front()->rc.left + _finalList.front()->rc.right) / 2, (_finalList.front()->rc.top + _finalList.front()->rc.bottom) / 2)) * 1.5;
		}

		float angle = getAngle(_x, _y, (_endNode->rc.left + _endNode->rc.right) / 2, (_endNode->rc.top + _endNode->rc.bottom) / 2);
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

	if (_isHit)
	{
		_knockBackSpeed -= 0.3f;
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
		if (_knockBackSpeed < 0)
		{
			_isHit = false;
			_knockBackSpeed = 3.f;
		}
	}
}

void redGolem::setStartNode()
{
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
}

void redGolem::soundUpdate()
{

}

void redGolem::hitSoundPlay()
{
	SOUNDMANAGER->play("golemHit", 0.5f);
}

void redGolem::dropItem()
{

}

void redGolem::redGolemCollision()
{
	RECT temp;
	if (!_emPlayerColi && PLAYER->getPlayerState() != PLAYER_ROLL)
	{
		if (IntersectRect(&temp, &PLAYER->getRect(), &_attackBox))
		{
			_emPlayerColi = true;
			PLAYERDATA->setInDungeonHp(PLAYERDATA->getInDungeonHp() - _emAtk);
			PLAYER->setPlayerState(HIT_IDLE);
			PLAYER->setHit(true);
			if (PLAYERDATA->getInDungeonHp() <= 0)
			{
				PLAYER->setPlayerState(PLAYER_DIE);
			}
		}
	}
	if (_emPlayerColi)
	{
		if (_attack->getAniState() == ANIMATION_END)
		{
			_emPlayerColi = false;
		}
	}

}
