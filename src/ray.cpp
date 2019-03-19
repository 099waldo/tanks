#include <iostream>
#include "ray.h"
#include "game.h"
#include "gameobject.h"

extern Game game;

void Ray::init(sf::Vector2f pos, sf::Vector2f vel, float rot)
{
	position = pos;
	velocity = vel;
	rotation = rot;

	radius = 10;
	bouncesLeft = 10;
};

int Ray::update()
{
	if (bouncesLeft == -1) { // Is this used with rays?
		return 1;
	}
	if (hit) {
		return 1;
	}
	return Move(); // Returns 1 if it hit a wall
}

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
			if (game.tanks[i].player == true) {
				game.tanks[i].Push(velocity);
				return true;
			} else {
				hit = true;
			}
		}
	}
	return false;
};

int Ray::Move()
{
	sf::Vector2f newvel = velocity;
	newvel.y = newvel.y * -1;

	position.x += newvel.x * speed;

	// TODO: Make it so it starts another ray when it hits a wall.

	if (CheckWallCollisions())
		return 1;
	if (CollidingWithTank())
		return 2;

	position.y += newvel.y * speed;

	if (CheckWallCollisions())
		return 1;
	if (CollidingWithTank())
		return 2;

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