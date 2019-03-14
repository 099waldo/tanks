#pragma once
#include <iostream>
#include "gameobject.h"

class Ray : public GameObject {
    public:
	bool CollidingWithTank();
	int Move();
	int Bounce();
	int bouncesLeft;
	float speed = 3.f;

    private:
	void init();
};
