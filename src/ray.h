#pragma once
#include <iostream>
#include "gameobject.h"

class Ray : public GameObject {
    public:
	bool CollidingWithTank();
	void init(sf::Vector2f pos, sf::Vector2f vel, float rot);
	int update();
	int Move();
	int Bounce();
	int bouncesLeft;
	float speed = 5.f;
	bool hit = false;

    private:
};
