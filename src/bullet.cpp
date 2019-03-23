#include "SFML/Graphics.hpp"
#include "vector"
#include "tank.h"
#include "barrier.h"
#include "bullet.h"
#include "game.h"

extern Game game;
extern Assets assets;

void Bullet::init(sf::Vector2f pos, sf::Vector2f vel, float rot)
{
	position = pos;
	velocity = vel;
	rotation = rot;

	radius = 10;

	m_sprite.setTexture(assets.bullettex);
	SetOriginToSprite();

	bouncesLeft = 2;
}

int Bullet::update()
{
	if (bouncesLeft == -1) {
		return 1;
	}
	return MoveBullet();
}

void Bullet::DestroyMe()
{
	game.tanks[Mtank].activeBuls -= 1;
}

int Bullet::MoveBullet()
{
	sf::Vector2f newvel = velocity;
	newvel.y = newvel.y * -1;

	position.x += newvel.x * speed;

	if (CheckWallCollisions()) {
		position.x -= newvel.x * speed;
		velocity.x = velocity.x * -1;
		rotation += 2 * (90 - rotation) + 180;
		if (Bounce() == 1) {
			return 1;
		}
		// std::cout << "Bullet hit wall" << std::endl;
	}

	position.y += newvel.y * speed;

	if (CheckWallCollisions()) {
		position.y -= newvel.y * speed;
		velocity.y = velocity.y * -1;
		rotation += 2 * (90 - rotation);
		if (Bounce() == 1) {
			return 1;
		}
		// std::cout << "Bullet hit wall" << std::endl;
	}

	// Check bullet bullet collisions.
	if (CollidingWithBullet()) {
		return 1;
	}

	FixRotation();

	m_sprite.setPosition(position);
	m_sprite.setRotation(rotation);
	return 0;
}

void Bullet::FixRotation()
{
	if (rotation > 360) {
		while (rotation > 360) {
			rotation -= 360;
		}
	} else if (rotation < 0) {
		while (rotation < 0) {
			rotation += 360;
		}
	}
}

int Bullet::Bounce()
{
	bouncesLeft -= 1;
	if (bouncesLeft < 1) {
		for (int j = 0; j < game.bullets.size(); j++) {
			game.bullets[j].vectorAdr = j;
		}
		return 1;
	}
	return 0;
}

void Bullet::SetOriginToSprite()
{
	sf::Vector2f neworigin = sf::Vector2f(
		m_sprite.getOrigin().x + assets.bullettex.getSize().x / 2,
		m_sprite.getOrigin().y + assets.bullettex.getSize().y / 2);
	m_sprite.setOrigin(neworigin);
}

bool Bullet::CollidingWithBullet()
{
	for (int i = 0; i < game.bullets.size(); i++) {
		if (i != vectorAdr) {
			sf::Vector2f pointreletivetome =
				game.bullets[i].position - position;
			float desiredrot = atanf(pointreletivetome.y /
						 pointreletivetome.x) *
						   180 / PI -
					   90;
			if (pointreletivetome.x < 0) {
				desiredrot += 180;
			}
			float magnitude = sqrtf(
				pointreletivetome.x * pointreletivetome.x +
				pointreletivetome.y * pointreletivetome.y);
			if (magnitude < 2 * radius) {
				game.bullets[i].bouncesLeft = -1;
				return true;
			}
		}
	}
	return false;
}