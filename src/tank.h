#pragma once
#include "vector"
#include "SFML/Graphics.hpp"
#include <math.h> /* atan */
#include "iostream"
#include "barrier.h"
#include "bullet.h"
#include "gameobject.h"
#include "tanksettings.h"
#include "time.h"

#define PI 3.14159265

class Tank : public GameObject {
    public:
	int type;
	float turretrotation = 0.f;
	float speed = 3.f;
	float speedFactor = 1;
	bool player = false;
	int shootingProb;
	int vectorAdr;

	TankSettings settings;

	int activeBuls;
	int maxBuls;

	sf::Vector2f PushVel;

	void Push(sf::Vector2f pushDir);

	void init(int tankType);
	int update();
	int FaceVelocityDir();
	void PointTurretAt(sf::Vector2f point);
	void Shoot();
	sf::Vector2f BulletSpawnPos();

	// Computer Player AI Functions

	time_t lastDecision;

	bool canMove();
	bool canHitPlayer();
	bool turnIfTouchingTank;

	sf::Vector2f playerPos();
	int randomint();

    private:
	sf::Texture bodytex;
	sf::Texture turrettex;
	sf::Sprite body;
	sf::Sprite turret;

	void LoadSettings();
	void SetOriginToSprite();
	int MoveTank();
	void FixRotation();
	bool CollidingWithBullet();
	bool CollidingWithTank();

	// Computer Player AI Functions

	void MakeDecision();
	void ChangeDirection();
	void ChangeSpeed();

	void draw(sf::RenderTarget &target,
		  sf::RenderStates states) const override
	{
		target.draw(this->body);
		target.draw(turret);
	}
};
