#include "stdafx.h"
#include "enemy.h"

HRESULT bossSkeleton::init(int x, int y)
{
	_move = new animation;
	_attackSword = new animation;
	_attackHammer = new animation;
	_hammerWave1 = new animation;
	_hammerWave2 = new animation;

	_move->init(IMAGEMANAGER->findImage("skeletonMove"), 0, 8,true);
	_attackSword->init(IMAGEMANAGER->findImage("skeletonAttackSword"), 0, 7);
	_attackHammer->init(IMAGEMANAGER->findImage("skeletonAttackHammer"), 0, 7);
	_hammerWave1->init(IMAGEMANAGER->findImage("hammerWave1"), 0, 7);
	_hammerWave2->init(IMAGEMANAGER->findImage("hammerWave2"), 0, 7);

	_attackSword->aniStop();
	_attackHammer->aniStop();
	_hammerWave1->aniStop();
	_hammerWave2->aniStop();

	_attackState = ST_MOVE;
	_emState = EM_MOVE;
	_x = x;
	_y = y;

	_waveCount = 0;
	_waveTime = 0;

	// 사운드 관련 함수 초기화
	_isAttackPlay = false;

	return S_OK;
}

void bossSkeleton::release()
{
}

void bossSkeleton::update()
{
	if (INPUT->GetKeyDown(VK_LEFT))
	{
		_emState = EM_ATTACK;
		_attackState = ST_SKILL_HAMMER;
		_attackHammer->aniRestart();
	}
	if (INPUT->GetKeyDown(VK_RIGHT))
	{
		_attackHammer->aniRestart();
	}
	this->soundUpdate();				//사운드 업뎃

	switch (_emState)
	{
	case EM_MOVE:
		_move->update();				// 애니메이션 업뎃

		this->setStartNode();			// 에이스타 시작설정
		this->aStar();					// 에이스타
		this->enemyMove();				// 움직임
		this->atkRangeUpdate();			// 공격범위 업뎃
		this->moveUpdate();				// 플레이어와 충돌검사해서 공격	


		break;
	case EM_ATTACK:
		_attackHammer->update();		// 애니메이션 업뎃
		_attackSword->update();			// 애니메이션 업뎃

		this->atkBoxUpdate();			// 공격박스 업뎃
		this->attackUpdate();			// 공격이 끝나면 움직이게 설정
		
		break;
	case EM_DIE:
		break;
	case EM_HIT:
		break;
	default:
		break;
	}

	_hammerWave1->update();
	_hammerWave2->update();
}

void bossSkeleton::render()
{
	this->animationRender();

	//FrameRect(getMemDC(), RectMakeCenter(_x, _y - 80, 130, 220), RGB(0, 0, 0));
	//FrameRect(getMemDC(), RectMakeCenter(_x, _y, 35, 35), RGB(255, 255, 255));
	FrameRect(getMemDC(), _hammerRange, RGB(0, 0, 255));
	FrameRect(getMemDC(), _swordRange, RGB(0, 255, 0));
	FrameRect(getMemDC(), _hammerAtkBox.box, RGB(0, 0, 0));
	FrameRect(getMemDC(), _swordAtkBox.box, RGB(0, 0, 0));
	FrameRect(getMemDC(), _emRC, RGB(255, 0, 0));
}

void bossSkeleton::setStartNode()
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

void bossSkeleton::enemyMove()
{
	if (_finalList.size() > 0)
	{
		float angle = getAngle(_x, _y, _finalList.front()->centerX, _finalList.front()->centerY);
		//_x += cosf(angle) * 1.5f;
		//_y -= sinf(angle) * 1.5f;

		angle = getAngle(_x, _y, (_endNode->rc.left + _endNode->rc.right) / 2, (_endNode->rc.top + _endNode->rc.bottom) / 2);
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

	_emRC = RectMakeCenter(_x, _y - 80, 130, 220);
	_aStarRC = RectMakeCenter(_x, _y, 35, 35);
}

void bossSkeleton::animationRender()
{

	switch (_emDirection)
	{
	case EM_LEFT:
		_move->setFrameY(1);
		_attackHammer->setFrameY(2);
		_attackSword->setFrameY(3);

		if (_hammerWave1->getAniState() != ANIMATION_END) _hammerWave1->ZoderRender(0, _x - IMAGEMANAGER->findImage("hammerWave1")->getFrameWidth() / 2 - 166, _y - IMAGEMANAGER->findImage("hammerWave1")->getFrameHeight() / 2  + 30);
		if (_hammerWave2->getAniState() != ANIMATION_END) _hammerWave2->ZoderRender(0, _x - IMAGEMANAGER->findImage("hammerWave2")->getFrameWidth() / 2 - 166, _y - IMAGEMANAGER->findImage("hammerWave2")->getFrameHeight() / 2  + 30);
		break;
	case EM_RIGHT:
		_move->setFrameY(2);
		_attackHammer->setFrameY(1);
		_attackSword->setFrameY(0);

		if (_hammerWave1->getAniState() != ANIMATION_END) _hammerWave1->ZoderRender(0, _x - IMAGEMANAGER->findImage("hammerWave1")->getFrameWidth() / 2 + 166, _y - IMAGEMANAGER->findImage("hammerWave1")->getFrameHeight() / 2 + 30);
		if (_hammerWave2->getAniState() != ANIMATION_END) _hammerWave2->ZoderRender(0, _x - IMAGEMANAGER->findImage("hammerWave2")->getFrameWidth() / 2 + 166, _y - IMAGEMANAGER->findImage("hammerWave2")->getFrameHeight() / 2 + 30);
		break;
	case EM_TOP:
		_move->setFrameY(3);
		_attackHammer->setFrameY(0);
		_attackSword->setFrameY(1);

		if (_hammerWave1->getAniState() != ANIMATION_END) _hammerWave1->ZoderRender(0, _x - IMAGEMANAGER->findImage("hammerWave1")->getFrameWidth() / 2 - 8, _y - IMAGEMANAGER->findImage("hammerWave1")->getFrameHeight() / 2 - 149);
		if (_hammerWave2->getAniState() != ANIMATION_END) _hammerWave2->ZoderRender(0, _x - IMAGEMANAGER->findImage("hammerWave2")->getFrameWidth() / 2 - 8, _y - IMAGEMANAGER->findImage("hammerWave2")->getFrameHeight() / 2 - 149);
		break;
	case EM_BOTTOM:
		_move->setFrameY(0);
		_attackHammer->setFrameY(3);
		_attackSword->setFrameY(2);

		if (_hammerWave1->getAniState() != ANIMATION_END) _hammerWave1->ZoderRender(0, _x - IMAGEMANAGER->findImage("hammerWave1")->getFrameWidth() / 2 + 13, _y - IMAGEMANAGER->findImage("hammerWave1")->getFrameHeight() / 2 + 140);
		if (_hammerWave2->getAniState() != ANIMATION_END) _hammerWave2->ZoderRender(0, _x - IMAGEMANAGER->findImage("hammerWave2")->getFrameWidth() / 2 + 13, _y - IMAGEMANAGER->findImage("hammerWave2")->getFrameHeight() / 2 + 140);
		break;

	}

	switch (_emState)
	{
	case EM_MOVE:
		_move->ZoderRender(_y, _x - IMAGEMANAGER->findImage("skeletonMove")->getFrameWidth() / 2, _y - IMAGEMANAGER->findImage("skeletonMove")->getFrameHeight() / 2);
		break;
	case EM_ATTACK:
		if (_attackHammer->getAniState() != ANIMATION_END)
		{
			if(_attackHammer->getCurIndex() < 6)
			_attackHammer->ZoderRender(_y, _x - IMAGEMANAGER->findImage("skeletonMove")->getFrameWidth() / 2, _y - IMAGEMANAGER->findImage("skeletonMove")->getFrameHeight() / 2);
			else if(_attackHammer->getCurIndex() >= 6)
			_attackHammer->ZoderRender(PLAYER->getY() + 1, _x - IMAGEMANAGER->findImage("skeletonMove")->getFrameWidth() / 2, _y - IMAGEMANAGER->findImage("skeletonMove")->getFrameHeight() / 2);
			else if(_attackHammer->getCurIndex() >= 13)
			_attackHammer->ZoderRender(_y, _x - IMAGEMANAGER->findImage("skeletonMove")->getFrameWidth() / 2, _y - IMAGEMANAGER->findImage("skeletonMove")->getFrameHeight() / 2);
		}
		if (_attackSword->getAniState() != ANIMATION_END) _attackSword->ZoderRender(_y, _x - IMAGEMANAGER->findImage("skeletonMove")->getFrameWidth() / 2, _y - IMAGEMANAGER->findImage("skeletonMove")->getFrameHeight() / 2);
		
		break;
	case EM_DIE:
		break;
	case EM_HIT:
		break;
	default:
		break;
	}
}

void bossSkeleton::atkRangeUpdate()
{
	switch (_emDirection)
	{
	case EM_LEFT:
		_hammerRange = RectMake(_emRC.left - 140, _y, _x - (_emRC.left - 140), 60);
		_swordRange = RectMake(_emRC.left - 140, _emRC.top + 100, 140, 220);
	break;
	case EM_RIGHT:
		_hammerRange = RectMake(_x, _y, (_emRC.right + 140) - _x, 60);
		_swordRange = RectMake(_emRC.right, _emRC.top + 100, 140, 220);
	break;
	case EM_TOP:
		_hammerRange = RectMake(_emRC.left + 10,_emRC.top + 20,90,_y - _emRC.top + 20);
		_swordRange = RectMake(_emRC.left - 20, _emRC.top, 150, _y - _emRC.top);
	break;
	case EM_BOTTOM:
		_hammerRange = RectMake(_x - 35, _y, 100, 200);
		_swordRange = RectMake(_emRC.left, _y, 150, 150);
	break;
	}
}

void bossSkeleton::atkBoxUpdate()
{
	switch (_emDirection)
	{
	case EM_LEFT:
		// 해머 공격용 렉트 
		if (_attackHammer->getCurIndex() >= 6 && _attackHammer->getCurIndex() <= 13)
		{
			_hammerAtkBox.box = RectMake(_hammerRange.left + 10, _hammerRange.top + 10, _x- _hammerRange.left + 10, 50);
		}
		else _hammerAtkBox = { 0,0,0,0 };
		// 칼 공격용 렉트
		if (_attackSword->getCurIndex() >= 7 && _attackSword->getCurIndex() <= 13)
		{
			_swordAtkBox.box = RectMake(_swordRange.left, _swordRange.top, 100, 100);
		}
		else _swordAtkBox.box = { 0,0,0,0 };
		break;
	case EM_RIGHT:
		if (_attackHammer->getCurIndex() >= 6 && _attackHammer->getCurIndex() <= 13)
		{
			_hammerAtkBox.box = RectMake(_x , _hammerRange.top + 10, _hammerRange.right - _x - 10, 50);
		}
		else _hammerAtkBox = { 0,0,0,0 };
		// 칼 공격용 렉트
		if (_attackSword->getCurIndex() >= 7 && _attackSword->getCurIndex() <= 13)
		{
			_swordAtkBox.box = RectMake(_swordRange.left, _swordRange.top, 100, 100);
		}
		else _swordAtkBox.box = { 0,0,0,0 };
		break;
	case EM_TOP:
		if (_attackHammer->getCurIndex() >= 6 && _attackHammer->getCurIndex() <= 13)
		{
			_hammerAtkBox.box = RectMake(_hammerRange.left + 10, _hammerRange.top + 5, 80, _y - _hammerRange.top + 5);
		}
		else _hammerAtkBox = { 0,0,0,0 };
		// 칼 공격용 렉트
		if (_attackSword->getCurIndex() >= 7 && _attackSword->getCurIndex() <= 13)
		{
			_swordAtkBox.box = RectMake(_swordRange.left, _swordRange.top, 100, 100);
		}
		else _swordAtkBox.box = { 0,0,0,0 };
		break;
	case EM_BOTTOM:
		if (_attackHammer->getCurIndex() >= 6 && _attackHammer->getCurIndex() <= 13)
		{
			_hammerAtkBox.box = RectMake(_x - 35,_y,95,180);
		}
		else _hammerAtkBox = { 0,0,0,0 };
		// 칼 공격용 렉트
		if (_attackSword->getCurIndex() >= 7 && _attackSword->getCurIndex() <= 13)
		{
			_swordAtkBox.box = RectMake(_swordRange.left, _swordRange.top, 100, 100);
		}
		else _swordAtkBox.box = { 0,0,0,0 };
		break;
	}
}

void bossSkeleton::moveUpdate()
{

	RECT temp;
	
	if (IntersectRect(&temp, &PLAYER->getRect(), &_hammerRange) && IntersectRect(&temp, &PLAYER->getRect(), &_swordRange))
	{
		if (RANDOM->range(2) == 0)
		{

			if (_attackSword->getAniState() == ANIMATION_END)
			{
				_attackSword->aniRestart();
				_attackState = ST_ATTACK_SWORD;
				_emState = EM_ATTACK;
			}
		}
		else
		{
			if (_attackHammer->getAniState() == ANIMATION_END)
			{
				_attackHammer->aniRestart();
				_attackState = ST_ATTACK_HAMMER;
				_emState = EM_ATTACK;
			}
		}
	}
	else if (IntersectRect(&temp, &PLAYER->getRect(), &_hammerRange))
	{
		if (_attackHammer->getAniState() == ANIMATION_END)
		{
			_attackHammer->aniRestart();
			_attackState = ST_ATTACK_HAMMER;
			_emState = EM_ATTACK;
		}
	}
	else if (IntersectRect(&temp, &PLAYER->getRect(), &_swordRange))
	{
		if (_attackSword->getAniState() == ANIMATION_END)
		{
			_attackSword->aniRestart();
			_attackState = ST_ATTACK_SWORD;
			_emState = EM_ATTACK;
		}
	}

}

void bossSkeleton::attackUpdate()
{
	switch (_attackState)
	{
	case bossSkeleton::ST_ATTACK_SWORD:
		if (_attackSword->getAniState() == ANIMATION_END)
		{
			_isAttackPlay = false;
			_attackSword->aniStop();
			_emState = EM_MOVE;
			_attackState = ST_MOVE;
		}
		break;
	case bossSkeleton::ST_ATTACK_HAMMER:
		// 망치가 땅을 찍었을때 충격파 애니메이션 시작
		if (_attackHammer->getCurIndex() == 6 && _hammerWave1->getAniState() == ANIMATION_END) _hammerWave1->aniRestart();
		if (_attackHammer->getAniState() == ANIMATION_END)
		{
			_isAttackPlay = false;				//사운드 재생 초기화
			_attackHammer->aniStop();
			_emState = EM_MOVE;
			_attackState = ST_MOVE;
		}
		break;
	case bossSkeleton::ST_SKILL_SWORD:
		break;
	case bossSkeleton::ST_SKILL_HAMMER:
		if (_attackHammer->getCurIndex() == 12)
		{
			_attackHammer->aniPause();
			_hammerWave1->aniRestart();
			_attackState = ST_WAVE;
		}
		break;
	case bossSkeleton::ST_WAVE:
		//첫번째 충격파
		if (_hammerWave1->getAniState() == ANIMATION_END && _waveCount == 0)
		{
			_waveTime++;
			if (_waveTime >= 20)
			{
				_waveTime = 0;
				_waveCount++;
				_hammerWave2->aniRestart();
			}
		}
		//두번째 충격파
		else if (_hammerWave2->getAniState() == ANIMATION_END && _waveCount < 5 && _waveCount > 0)
		{
			_waveTime++;
			if (_waveTime >= 20 && _waveCount < 4)
			{
				_waveTime = 0;
				_waveCount++;
				_hammerWave2->aniRestart();
			}
			else if (_waveTime >= 20 && _waveCount == 4)
			{
				_waveTime = 0;
				_waveCount++;
				_attackHammer->aniPlay();
			}
		}
		else if (_attackHammer->getAniState() == ANIMATION_END && _waveCount == 5)
		{
			_emState = EM_MOVE;
			_attackState = ST_MOVE;
			_waveCount = 0;
			_waveTime = 0;
		}


		break;
	case bossSkeleton::ST_MOVE:
		break;
	default:
		break;
	}
}

void bossSkeleton::soundUpdate()
{
	switch (_attackState)
	{
	case bossSkeleton::ST_ATTACK_SWORD:
		break;
	case bossSkeleton::ST_ATTACK_HAMMER:
		if (_attackHammer->getCurIndex() == 3 && _isAttackPlay == false)
		{
			_isAttackPlay = true;
			SOUNDMANAGER->play("skullAttackHammer",0.2f);
		}
		break;
	case bossSkeleton::ST_SKILL_SWORD:
		break;
	case bossSkeleton::ST_SKILL_HAMMER:
		break;
	case bossSkeleton::ST_MOVE:
		break;
	default:
		break;
	}
}
