#pragma once

typedef struct
{
	image *img;				//오브젝트 이미지 
	POINT pos;				//오브젝트 위치 
	float movingSpeed;		//오브젝트 이동속도 
	bool isArrived;			//목표지점까지 도달상태(true/false)

}objectUnit;

class movingObject
{
private:
	objectUnit _movingObj;

public:
	HRESULT init();
	HRESULT init(image *img, int posX, int posY, float speed);
	void release();
	void update();
	void render(HDC hdc);

	//get함수 
	objectUnit getObject() { return _movingObj; }
	image *getImg() { return _movingObj.img; }
	POINT getPos() { return _movingObj.pos; }
	float getSpeed() { return _movingObj.movingSpeed; }
	bool getIsArrived() { return _movingObj.isArrived; }

	//set함수 
	void setImg(image* img) { _movingObj.img = img; }
	void setPos(POINT pos) { _movingObj.pos = pos; }
	void setPosX(int posX) { _movingObj.pos.x = posX; }
	void setPosY(int posY) { _movingObj.pos.y = posY; }
	void setSpeed(float speed) { _movingObj.movingSpeed = speed; }
	void setIsArrived(bool value) { _movingObj.isArrived = value; }

	void initPos(int posX, int posY);		//매개변수 값을 받아 위치를 초기화하는 함수 
	void moveLeft(const int destPos);		//목적지로 왼쪽 이동
	void moveRight(const int destPos);		//목적지로 오른쪽 이동
	void moveUp(const int destPos);			//목적지로 위쪽 이동
	void moveDown(const int destPos);		//목적지로 아래쪽 이동 
};

