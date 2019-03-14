#include <iostream>
#include "ray.h"
#include "game.h"
#include "gameobject.h"

extern Game game;

void Ray::init()
{
	radius = 1;
};

bool Ray::CollidingWithTank()
{
	for (int i = 0; i < game.tanks.size(); i++) {
		sf::Vector2f pointreletivetome =
			game.tanks[i].position - position;
		float desiredrot =
			atanf(pointreletivetome.y / pointreletivetome.x) * 180 /
				PI -
			90;
		if (pointreletivetome.x < 0) {
			desiredrot += 180;
		}
		float magnitude =
			sqrtf(pointreletivetome.x * pointreletivetome.x +
			      pointreletivetome.y * pointreletivetome.y);
		if (magnitude < radius + game.tanks[i].radius) {
			game.tanks[i].Push(velocity);
			return true;
		}
	}
	return false;
};

int Ray::Move()
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

	return 0;
}

int Ray::Bounce()
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