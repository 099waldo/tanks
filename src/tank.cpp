#include "vector"
#include "SFML/Graphics.hpp"
#include <math.h> /* atan */
#include "iostream"
#include "barrier.h"
#include "bullet.h"
#include "game.h"
#include "tank.h"
#include "gameobject.h"
#include "tanksettings.h"
#include "time.h"
#include "ray.h"

#include <cstdlib>

#define PI 3.14159265

extern Game game;
extern Assets assets;

void Tank::Push(sf::Vector2f pushDir)
{
	PushVel = pushDir;
}

void Tank::init(int tankType)
{
	type = tankType;
	activeBuls = 0;
	LoadSettings();
	if (type == 0)
		player = true;

	// Set tank textures

	bodytex = assets.tankBodys[type];
	turrettex = assets.tankTurrets[type];
	body.setTexture(assets.tankBodys[type]);
	turret.setTexture(assets.tankTurrets[type]);

	position = sf::Vector2f(200, 200);
	radius = 31;
	body.setPosition(position);
	turret.setPosition(position);
	SetOriginToSprite();
	lastDecision = time(NULL);
}

int Tank::update()
{
	FixRotation();
	MoveTank();
	if (CollidingWithBullet()) {
		//std::cout << "Got hit by bullet!!" << std::endl;
	}

	PushVel = sf::Vector2f(0.f, 0.f);
	turret.setPosition(position);
	turret.setRotation(turretrotation);

	if (canMove())
		MakeDecision();

	return 0;
}

int Tank::FaceVelocityDir()
{
	if (velocity.x == 0 && velocity.y == 0)
		return 0;
	sf::Vector2f newvel = velocity;
	newvel.y = newvel.y * -1;
	float desiredrot = atanf(newvel.y / newvel.x) * 180 / PI + 90;
	if (newvel.x < 0) {
		desiredrot += 180;
	}
	rotation = desiredrot;
	return 0;
}

void Tank::PointTurretAt(sf::Vector2f point)
{
	sf::Vector2f pointreletivetome = point - position;
	float desiredrot =
		atanf(pointreletivetome.y / pointreletivetome.x) * 180 / PI -
		90;
	if (pointreletivetome.x < 0) {
		desiredrot += 180;
	}
	turretrotation = desiredrot;
}

void Tank::Shoot()
{
	if (activeBuls < 0) {
		activeBuls = 0;
	}
	if (activeBuls < maxBuls && canHitPlayer()) {
		Bullet bul;
		sf::Vector2f bulv;
		bulv.x = 1 * cosf((turretrotation + 90) * (PI / 180));
		bulv.y = -1 * sinf((turretrotation + 90) * (PI / 180));
		bul.init(BulletSpawnPos(), bulv, turretrotation + 180);
		bul.Mtank = vectorAdr;
		game.bullets.push_back(bul);
		activeBuls += 1;
	}
}

sf::Vector2f Tank::BulletSpawnPos()
{
	sf::Vector2f bulpos = position;
	bulpos.x += 28 * cosf(turretrotation * PI / 180);
	bulpos.y += 28 * sinf(turretrotation * PI / 180);

	bulpos.x += (radius + 28) * cosf(turretrotation * PI / 180 + 90);
	bulpos.y += (radius + 28) * sinf(turretrotation * PI / 180 + 90);

	return bulpos;
}

// Computer Player AI Functions

bool Tank::canMove()
{
	return !player && speed > 0;
}

bool Tank::canHitPlayer()
{
	// If we are the player this function doesn't need to be run.
	if (player) {
		return true;
	}
	Ray ray;
	sf::Vector2f bulv;
	bulv.x = 1 * cosf((turretrotation + 90) * (PI / 180));
	bulv.y = -1 * sinf((turretrotation + 90) * (PI / 180));
	ray.init(BulletSpawnPos(), bulv, turretrotation + 180);

	bool done = false;
	while (done != true) {
		int value = ray.update();
		if (value == 1) {
			// std::cout << "Hit wall" << std::endl;
			done = true;
			return false;
		}
		if (value == 2) {
			// std::cout << "Hit tank" << std::endl;
			done = true;
			return true;
		}
	}
	return false;
}

// TODO: Change this to compare all of the player positions to see
// which one is the closest to the tank.

sf::Vector2f Tank::playerPos()
{
	std::vector<sf::Vector2f> players;
	for (int i = 0; i < game.tanks.size(); i++) {
		if (game.tanks[i].player) {
			players.push_back(game.tanks[i].position);
		}
	}
	if (players.size() > 0) {
		return players[0];
	}
	return sf::Vector2f(0, 0);
}

// Will return a random number between 0 and 1000

int Tank::randomint()
{
	return (rand() % 1000) + 1;
}

void Tank::MakeDecision()
{
	// if(lastDecision == nulls)
	double seconds = difftime(time(NULL), lastDecision);
	if (seconds > 1) {
		if (randomint() < 10) {
			ChangeDirection();
			lastDecision = time(NULL);
		}
	}
}

void Tank::ChangeDirection()
{
	int j = randomint();
	if (j > 720)
		j -= 720;
	if (j > 360)
		j -= 360;

	sf::Vector2f newvel;
	newvel.x = cosf(j * (PI / 180));
	newvel.y = sinf(j * (PI / 180));

	velocity = newvel;
}

void Tank::LoadSettings()
{
	speed = game.tankSettings[type].speed;
	player = game.tankSettings[type].player;
	maxBuls = game.tankSettings[type].maxBuls;
	shootingProb = game.tankSettings[type].shootingProb;
	turnIfTouchingTank = game.tankSettings[type].turnIfTouchingTank;
}

void Tank::SetOriginToSprite()
{
	sf::Vector2f neworigin =
		sf::Vector2f(body.getOrigin().x + bodytex.getSize().x / 2,
			     body.getOrigin().y + bodytex.getSize().y / 2);
	body.setOrigin(neworigin);

	sf::Vector2f newturretorigin =
		sf::Vector2f(turret.getOrigin().x + turrettex.getSize().x / 2,
			     turret.getOrigin().y + turrettex.getSize().x / 2);
	turret.setOrigin(newturretorigin);
}

int Tank::MoveTank()
{
	sf::Vector2f newvel = velocity;

	// The magnitude of the velocity vector.
	float magV = 1.f;
	newvel.y = newvel.y * -1;

	float rot = atanf(newvel.y / newvel.x);
	if (newvel.x < 0) {
		rot += PI;
	}

	// Change this so we check if it is < 0.2 or whatever joystick
	// deadzone value you want. This won't effect keyboard input but
	// will effect how sensitive joysticks are.
	if (newvel.x != 0 && newvel.y != 0) {
		newvel.x = magV * cosf(rot);
		newvel.y = magV * sinf(rot);
	}

	// Move tank to the new position but if we are touching anything,
	// move it back to it's original position.
	position.x += newvel.x * speed;

	// Check for the rare occasion when the tank is touching another
	// tank and a wall at the same time.
	if (CheckWallCollisions() && CollidingWithTank())
		position.x -= newvel.x * speed;

	if (CheckWallCollisions()) {
		if (canMove())
			ChangeDirection();
		position.x -= newvel.x * speed;
	}
	if (CollidingWithTank()) {
		if (turnIfTouchingTank) {
			ChangeDirection();
		}
		position.x -= newvel.x * speed;
	}

	position.y += newvel.y * speed;

	if (CheckWallCollisions() || CollidingWithTank()) {
		if (canMove())
			ChangeDirection();
		position.y -= newvel.y * speed;
	}

	position.x += PushVel.x;

	if (CheckWallCollisions() || CollidingWithTank()) {
		position.x -= PushVel.x;
	}

	position.y += -1 * PushVel.y;

	if (CheckWallCollisions() || CollidingWithTank()) {
		position.y -= -1 * PushVel.y;
	}

	body.setPosition(position);
	body.setRotation(rotation);
	return 0;
}

void Tank::FixRotation()
{
	// Fix body rotation
	if (rotation > 360)
		rotation -= 360;
	if (rotation < 0)
		rotation += 360;

	// Fix turret rotation
	if (turretrotation > 360)
		turretrotation -= 360;
	if (turretrotation < 0)
		turretrotation += 360;
}

bool Tank::CollidingWithBullet()
{
	for (int i = 0; i < game.bullets.size(); i++) {
		sf::Vector2f pointreletivetome =
			game.bullets[i].position - position;
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
		if (magnitude < radius + game.bullets[i].radius) {
			game.bullets[i].bouncesLeft = -1;
			return true;
		}
	}
	return false;
}

bool Tank::CollidingWithTank()
{
	for (int i = 0; i < game.tanks.size(); i++) {
		if (i != vectorAdr) {
			sf::Vector2f pointreletivetome =
				game.tanks[i].position - position;
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
			if (magnitude < radius + game.tanks[i].radius) {
				game.tanks[i].Push(velocity);
				return true;
			}
		}
	}
	return false;
}
