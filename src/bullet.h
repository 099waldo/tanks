#pragma once
#include "SFML/Graphics.hpp"
#include "gameobject.h"

class Bullet : public GameObject {
    public:
	int vectorAdr;
	int Mtank;
	int bouncesLeft;

	float speed = 3.f;
	void init(sf::Vector2f pos, sf::Vector2f vel, float rot);
	int update();

    private:
	int MoveBullet();
	void FixRotation();
	int Bounce();
	void SetOriginToSprite();
	bool CollidingWithBullet();
	void DestroyMe();
};