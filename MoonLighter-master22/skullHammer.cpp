#include "stdafx.h"
#include "enemy.h"

HRESULT skullHammer::init(int x, int y)
{
	_move = new animation;
	_attack = new animation;
	_moveHit = new animation;
	_attackHit = new animation;
	_die = new animation;

	_move->init(IMAGEMANAGER->findImage("해골전사"), 0, 6, true);
	_moveHit->init(IMAGEMANAGER->findImage("해골전사피격"), 0, 6, true);
	_attack->init(IMAGEMANAGER->findImage("해골공격"), 0, 6);
	_attackHit->init(IMAGEMANAGER->findImage("해골공격피격"), 0, 6);
	_die->init(IMAGEMANAGER->findImage("enemyDie"), 0, 6);

	_x = x;
	_y = y;

	_emRC = RectMakeCenter(x, y, 40, 70);
	_aStarRC = RectMakeCenter(x, y + 20, 35, 35);

	_emHp = 100;
	_emAtk = 15;
	_hitCount = 0;

	_attackRange = RectMake(0, 0, 0, 0);
	_isActivate = true;
	_isAttackRange = false;
	_isHit = false;
	_isDieSoundPlay = false;
	_isAttackSoundPlay = false;
	_isItemDrop = false;

	_emState = EM_MOVE;
	_emDirection = EM_LEFT;

	_hpBar = new progressBar;
	_hpBar->init("enemyProgressBarFront", "enemyProgressBarBack", _emHp);
	_hpBarAlpha = 255;
	_isHpBarRender = true;

	_itemIndexSize = 2;
	_itemDropSize = RANDOM->range(3) + 1;
	_itemIndex = new int[_itemIndexSize];
	_itemNum = new int[_itemDropSize];

	//나올수 있는 아이템인덱스 초기화
	_itemIndex[0] = BROKENSWORD_IDX;
	_itemIndex[1] = FABRIC_IDX;

	// 드랍될 아이템의 인덱스를 랜덤으로 지정
	for (int i = 0; i < _itemDropSize; i++)
	{
		_itemNum[i] = _itemIndex[RANDOM->range(_itemIndexSize)];
	}

	_emPlayerColi = false;
	count = 0;

	return S_OK;
}

void skullHammer::release()
{
	
}

void skullHammer::update()
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
	// 에너미 자리를 에이스타 시작으로 설정
	this->setStartNode();
	//상태별 업데이트
	switch (_emState)
	{
	case EM_MOVE:
		_move->update();
		_moveHit->update();
		//에이스타
		this->aStar();
		//에이스타로 길찾은걸 기반으로 움직이기
		if (!isAstarFail())
		this->skullMove();
		//공격범위안이면 공격
		if (_isAttackRange)
		{
			_emState = EM_ATTACK;
		}
		// 에너미가 보고있는 방향 업데이트
		this->directionUpdate();
		break;
	case EM_ATTACK:
		_attack->update();
		_attackHit->update();
		//어택박스 업뎃
		this->attackBox();
		//공격범위 밖이면 움직여
		if (_attack->getAniState() == ANIMATION_END)
		{
			_isAttackSoundPlay = false;
			if (!_isAttackRange)
			{
				_emState = EM_MOVE;
			}
			else
			{
				_attack->aniRestart();
				_attackHit->aniRestart();
			}
		}
		// 히트일때 색깔바꿔주기
		if (_isHit)
		{
			_hitCount++;
			if (_hitCount > 10)
			{
				_isHit = false;
				_hitCount = 0;
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
			_isActivate = false;
		}
		break;
	case EM_HIT:
		break;
	}

	skullHammerCollision();
}

void skullHammer::render()
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
		_die->ZoderRender(_y, _x - 60, _y - 70);
		break;
	}
	if (_isHpBarRender) _hpBar->cameraAlphaRender(_x, _y - 70, _hpBarAlpha);
	aStarRender();
}

void skullHammer::directionUpdate()
{
	float ex = (_aStarRC.right + _aStarRC.left) / 2;
	float ey = (_aStarRC.bottom + _aStarRC.top) / 2;
	float angle = getAngle(ex, ey, PLAYER->getX(), PLAYER->getY());

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

	switch (_emDirection)
	{
	case EM_LEFT:
		_attackRange = RectMake(_emRC.left - 60, _emRC.top, 60, 70);
		break;
	case EM_RIGHT:
		_attackRange = RectMake(_emRC.right, _emRC.top, 60, 70);
		break;
	case EM_TOP:
		_attackRange = RectMake(_emRC.left, _emRC.top - 30, 40, 30);
		break;
	case EM_BOTTOM:
		_attackRange = RectMake(_emRC.left, _emRC.bottom, 40, 30);
		break;
	default:
		break;
	}

}

void skullHammer::moveRender()
{
	switch (_emDirection)
	{
	case EM_LEFT:
		if (_isHit)
		{
			_moveHit->setFrameY(2);
			_moveHit->ZoderRender(_y, _emRC.left - IMAGEMANAGER->findImage("해골전사")->getFrameWidth() / 2 + 20, _emRC.top - 50);
		}
		else
		{
			_move->setFrameY(2);
			_move->ZoderRender(_y, _emRC.left - IMAGEMANAGER->findImage("해골전사")->getFrameWidth() / 2 + 20, _emRC.top - 50);
		}
		break;
	case EM_RIGHT:
		if (_isHit)
		{
			_moveHit->setFrameY(3);
			_moveHit->ZoderRender(_y, _emRC.left - IMAGEMANAGER->findImage("해골전사")->getFrameWidth() / 2 + 20, _emRC.top - 50);
		}
		else
		{
			_move->setFrameY(3);
			_move->ZoderRender(_y, _emRC.left - IMAGEMANAGER->findImage("해골전사")->getFrameWidth() / 2 + 20, _emRC.top - 50);
		}
		break;
	case EM_TOP:
		if (_isHit)
		{
			_moveHit->setFrameY(0);
			_moveHit->ZoderRender(_y, _emRC.left - IMAGEMANAGER->findImage("해골전사")->getFrameWidth() / 2 + 20, _emRC.top - 50);
		}
		else
		{
			_move->setFrameY(0);
			_move->ZoderRender(_y, _emRC.left - IMAGEMANAGER->findImage("해골전사")->getFrameWidth() / 2 + 20, _emRC.top - 50);
		}
		break;
	case EM_BOTTOM:
		if (_isHit)
		{
			_moveHit->setFrameY(1);
			_moveHit->ZoderRender(_y, _emRC.left - IMAGEMANAGER->findImage("해골전사")->getFrameWidth() / 2 + 20, _emRC.top - 50);
		}
		else
		{
			_move->setFrameY(1);
			_move->ZoderRender(_y, _emRC.left - IMAGEMANAGER->findImage("해골전사")->getFrameWidth() / 2 + 20, _emRC.top - 50);
		}
		break;
	}
}

void skullHammer::attackRender()
{
	switch (_emDirection)
	{
	case EM_LEFT:
		if (_isHit)
		{
			_attackHit->setFrameY(3);
			_attackHit->ZoderRender(_y, _emRC.left - IMAGEMANAGER->findImage("해골공격")->getFrameWidth() / 2 + 20, _emRC.top - 50);
		}
		else
		{
			_attack->setFrameY(3);
			_attack->ZoderRender(_y, _emRC.left - IMAGEMANAGER->findImage("해골공격")->getFrameWidth() / 2 + 20, _emRC.top - 50);
		}
		break;
	case EM_RIGHT:
		if (_isHit)
		{
			_attackHit->setFrameY(0);
			_attackHit->ZoderRender(_y, _emRC.left - IMAGEMANAGER->findImage("해골공격")->getFrameWidth() / 2 + 20, _emRC.top - 50);
		}
		else
		{
			_attack->setFrameY(0);
			_attack->ZoderRender(_y, _emRC.left - IMAGEMANAGER->findImage("해골공격")->getFrameWidth() / 2 + 20, _emRC.top - 50);
		}
		break;
	case EM_TOP:
		if (_isHit)
		{
			_attackHit->setFrameY(1);
			_attackHit->ZoderRender(_y, _emRC.left - IMAGEMANAGER->findImage("해골공격")->getFrameWidth() / 2 + 20, _emRC.top - 50);
		}
		else
		{
			_attack->setFrameY(1);
			_attack->ZoderRender(_y, _emRC.left - IMAGEMANAGER->findImage("해골공격")->getFrameWidth() / 2 + 20, _emRC.top - 50);
		}
		break;
	case EM_BOTTOM:
		if (_isHit)
		{
			_attackHit->setFrameY(2);
			_attackHit->ZoderRender(_y, _emRC.left - IMAGEMANAGER->findImage("해골공격")->getFrameWidth() / 2 + 20, _emRC.top - 50);
		}
		else
		{
			_attack->setFrameY(2);
			_attack->ZoderRender(_y, _emRC.left - IMAGEMANAGER->findImage("해골공격")->getFrameWidth() / 2 + 20, _emRC.top - 50);
		}
		break;
	}
}

void skullHammer::attackAniPlay()
{
}

bool skullHammer::attackAniStop()
{
	return false;
}

void skullHammer::attackBox()
{
	if (_attack->getCurIndex() == 3 && _isAttackSoundPlay == false)
	{
		SOUNDMANAGER->play("skullAttackSword", 0.5f);
		_isAttackSoundPlay = true;
	}
	switch (_emDirection)
	{
	case EM_LEFT:
		if (_attack->getCurIndex() > 6 && _attack->getCurIndex() < 11)
		{
			_attackBox = RectMake(_emRC.left - 60, _emRC.top, 60, 100);
		}
		else
		{
			_attackBox = RectMake(0, 0, 0, 0);
		}
		break;
	case EM_RIGHT:
		if (_attack->getCurIndex() > 6 && _attack->getCurIndex() < 11)
		{
			_attackBox = RectMake(_emRC.right, _emRC.top, 60, 100);
		}
		else
		{
			_attackBox = RectMake(0, 0, 0, 0);
		}
		break;
	case EM_TOP:
		if (_attack->getCurIndex() > 6 && _attack->getCurIndex() < 11)
		{
			_attackBox = RectMake(_emRC.left - 20, _emRC.top - 30, 80, 30);
		}
		else
		{
			_attackBox = RectMake(0, 0, 0, 0);
		}
		break;
	case EM_BOTTOM:
		if (_attack->getCurIndex() > 6 && _attack->getCurIndex() < 11)
		{
			_attackBox = RectMake(_emRC.left, _emRC.bottom, 60, 30);
		}
		else
		{
			_attackBox = RectMake(0, 0, 0, 0);
		}
		break;
	default:
		break;
	}
}

void skullHammer::skullMove()
{
	if (_finalList.size() > 0)
	{
		float x = _finalList.front()->centerX;
		float y = _finalList.front()->centerY;
		float ex = (_aStarRC.right + _aStarRC.left) / 2;
		float ey = (_aStarRC.top + _aStarRC.bottom) / 2;
		float angle = getAngle(ex, ey, x, y);
		if (!_isHit)
		{
			_x += cosf(angle) * 1.5f;
			_y -= sinf(angle) * 1.5f;
		}
	}
	if (!_isHit)
	{
		_knockBackSpeed = 3.f;
	}
	if (_isHit)
	{
		_knockBackSpeed -= 0.3f;
		if (_knockBackSpeed < 0) _isHit = false;
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
		default:
			break;
		}
	}

	_emRC = RectMakeCenter(_x, _y, 40, 70);
	_aStarRC = RectMakeCenter(_x, _y + 20, 35, 35);
}

void skullHammer::setStartNode()
{
	POINT pt;
	pt.x = (_aStarRC.right + _aStarRC.left) / 2;
	pt.y = (_aStarRC.top + _aStarRC.bottom) / 2;

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

void skullHammer::hitSoundPlay()
{
	SOUNDMANAGER->play("skullHit", 0.5f);
}

void skullHammer::skullHammerCollision()
{
	RECT temp;

	if (!_emPlayerColi  && PLAYER->getPlayerState() != PLAYER_ROLL)
	{
		if (IntersectRect(&temp, &PLAYER->getRect(), &_attackBox))
		{
			_emPlayerColi = true;
		
			allEnemyColi(_emAtk, checkDirection());

			if (PLAYERDATA->getInDungeonHp() <= 0)
			{
				RESULTENEMY* em = new RESULTENEMY;
				em->attack = new animation;
				em->attack->init(_attack->getImage(), 0, 7, true);
				em->frameY = 2;
				em->scale = 1.f;
				PLAYERDATA->setKillEnemy(em);
				PLAYERDATA->setInDungeonHp(0);
				PLAYER->setPlayerState(PLAYER_DIE);
			}
		}
	}

	if (_emPlayerColi)
	{
		if (_attack->getCurIndex() == 21)
		{
			_emPlayerColi = false;
		}
	}
}

animation * skullHammer::getAttackAnimation()
{
	return _attack;
}

int skullHammer::getDownDirectionY()
{
	return 2;
}
