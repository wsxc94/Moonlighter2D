#include "stdafx.h"
#include "effectManager.h"
#include "animation.h"

HRESULT effectManager::addEffect(string key, float z, int x, int y, int frameY, int playSpeed, bool isAlpha, BYTE alpha)
{
	tagEffect effect;
	effect.ani = new animation;
	effect.ani->init(IMAGEMANAGER->findImage(key), frameY, playSpeed);
	effect.z = z;
	effect.x = x - IMAGEMANAGER->findImage(key)->getFrameWidth() / 2;
	effect.y = y - IMAGEMANAGER->findImage(key)->getFrameHeight() / 2;
	effect._isAlpha = isAlpha;
	effect.alpha = alpha;
	_vEffect.push_back(effect);
	return S_OK;
}

void effectManager::update()
{
	for (int i = 0; i < _vEffect.size();)
	{
		_vEffect[i].ani->update();
		if (_vEffect[i].ani->getAniState() == ANIMATION_END)
			this->deleteAnimation(i);
		else i++;
	}
	for (int i = 0; i < _vParticle.size();)
	{
		_vParticle[i].x += cosf(_vParticle[i].angle) * _vParticle[i].speed;
		_vParticle[i].y -= sinf(_vParticle[i].angle) * _vParticle[i].speed;
		_vParticle[i].count--;
		if (_vParticle[i].count <= 0)
			this->delParticle(i);
		else
			i++;
	}
}

void effectManager::render()
{
	for (int i = 0; i < _vEffect.size(); i++)
	{
		if (_vEffect[i]._isAlpha == false)
			_vEffect[i].ani->ZoderRender(_vEffect[i].z, _vEffect[i].x, _vEffect[i].y);
		else
			_vEffect[i].ani->ZoderAlphaRender(_vEffect[i].z, _vEffect[i].x, _vEffect[i].y, _vEffect[i].alpha);
	}
	for (int i = 0; i < _vParticle.size(); i++)
	{
		if (_vParticle[i].isAlpha)
			CAMERAMANAGER->ZorderAlphaFrameRender(IMAGEMANAGER->findImage(_vParticle[i].imgKey), _vParticle[i].z,
				_vParticle[i].x, _vParticle[i].y, _vParticle[i].frameX, _vParticle[i].frameY, _vParticle[i].alpha);
		else
			CAMERAMANAGER->ZorderFrameRender(IMAGEMANAGER->findImage(_vParticle[i].imgKey), _vParticle[i].z,
				_vParticle[i].x, _vParticle[i].y, _vParticle[i].frameX, _vParticle[i].frameY);
	}
}

void effectManager::deleteAnimation(int idx)
{
	_vEffect.erase(_vEffect.begin() + idx);
}

HRESULT effectManager::addParticle(string key, float z, int x, int y, bool isAlpha, BYTE alpha)
{
	tagParticle ptcle;
	ptcle.imgKey = key;
	ptcle.z = z;
	ptcle.x = x - IMAGEMANAGER->findImage(key)->getFrameWidth() / 2;
	ptcle.y = y - IMAGEMANAGER->findImage(key)->getFrameHeight() / 2;
	ptcle.angle = RANDOM->range(PI2);
	ptcle.frameX = RANDOM->range(IMAGEMANAGER->findImage(key)->getMaxFrameX() + 1);
	ptcle.frameY = RANDOM->range(IMAGEMANAGER->findImage(key)->getMaxFrameY() + 1);
	ptcle.isAlpha = isAlpha;
	ptcle.alpha = alpha;
	ptcle.count = RANDOM->range(15, 25);
	ptcle.speed = RANDOM->range(1.f);
	_vParticle.push_back(ptcle);
	return S_OK	;
}

void effectManager::delParticle(int idx)
{
	_vParticle.erase(_vParticle.begin() + idx);
}
