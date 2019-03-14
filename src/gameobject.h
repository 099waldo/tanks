#pragma once
#include "SFML/Graphics.hpp"

class GameObject : public sf::Drawable {
    public:
	sf::Sprite m_sprite;
	sf::Texture tex;
	sf::Vector2f position;
	sf::Vector2f velocity;
	float rotation = 0.f;
	int radius;
	void init();
	void update();
	void SetOriginToSprite();
	bool CheckWallCollisions();
	bool CollidingWithRect(sf::FloatRect rect);
	sf::Vector2f PointOnCircle(float angle);

    private:
	void draw(sf::RenderTarget &target,
		  sf::RenderStates states) const override
	{
		target.draw(m_sprite);
	}
};