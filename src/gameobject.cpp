#include "gameobject.h"
#include "game.h"

extern Game game;
extern Assets assets;

void GameObject::init()
{
}

void GameObject::update()
{
}

void GameObject::SetOriginToSprite()
{
	sf::Vector2f neworigin =
		sf::Vector2f(m_sprite.getOrigin().x + tex.getSize().x / 2,
			     m_sprite.getOrigin().y + tex.getSize().y / 2);
	m_sprite.setOrigin(neworigin);
}

bool GameObject::CheckWallCollisions()
{
	for (int i = 0; i < game.walls.size(); i++) {
		sf::FloatRect therect =
			(sf::FloatRect)game.walls[i].m_sprite.getTextureRect();
		therect.left += game.walls[i].position.x;
		therect.top += game.walls[i].position.y;
		if (CollidingWithRect(therect)) {
			// std::cout << "Colliding!!" << std::endl;
			return true;
		}
		// else
		// {
		//     std::cout << "not Collidiong" << std::endl;
		// }
	}
	return false;
}

bool GameObject::CollidingWithRect(sf::FloatRect rect)
{
	for (int i = 0; i < 17; i++) {
		if (rect.contains(PointOnCircle(i * 20))) {
			return true;
		}
	}
	return false;
}

sf::Vector2f GameObject::PointOnCircle(float angle)
{
	sf::Vector2f point;
	point.x = position.x + (radius * cosf(angle));
	point.y = position.y + (radius * sinf(angle));
	return point;
}