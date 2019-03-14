#include "SFML/Graphics.hpp"
#include "barrier.h"

void Barrier::init(float x, float y)
{
	position = sf::Vector2f(x, y);
	m_sprite.setPosition(position);
}