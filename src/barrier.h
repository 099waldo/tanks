#pragma once
#include "SFML/Graphics.hpp"
#include "gameobject.h"

class Barrier : public GameObject {
    public:
	void init(float x, float y);
};