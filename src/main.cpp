#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>
#include "tank.h"
#include "barrier.h"
#include "bullet.h"
#include "game.h"

Game game;
Assets assets;

void LoadSprites()
{
	for (int i = 0; i < 7; i++) {
		assets.tankBodys.push_back(sf::Texture());
		assets.tankTurrets.push_back(sf::Texture());
	}
	assets.tankBodys[0].loadFromFile("img/tankBlue_outline.png");
	assets.tankBodys[1].loadFromFile("img/tankBeige_outline.png");
	assets.tankBodys[2].loadFromFile("img/tankBlack_outline.png");
	assets.tankBodys[3].loadFromFile("img/tankGreen_outline.png");
	assets.tankBodys[4].loadFromFile("img/tankLightGreen_outline.png");
	assets.tankBodys[5].loadFromFile("img/tankOrange_outline.png");
	assets.tankBodys[6].loadFromFile("img/tankRed_outline.png");

	assets.tankTurrets[0].loadFromFile("img/barrelBlue_outline.png");
	assets.tankTurrets[1].loadFromFile("img/barrelBeige_outline.png");
	assets.tankTurrets[2].loadFromFile("img/barrelBlack_outline.png");
	assets.tankTurrets[3].loadFromFile("img/barrelGreen_outline.png");
	assets.tankTurrets[4].loadFromFile("img/barrelLightGreen_outline.png");
	assets.tankTurrets[5].loadFromFile("img/barrelOrange_outline.png");
	assets.tankTurrets[6].loadFromFile("img/barrelRed_outline.png");

	assets.walltex.loadFromFile("img/wall.png");
	assets.bullettex.loadFromFile("img/bullet_silver_short.png");
}

void AddWall(int i, int j, int num)
{
	Barrier barrier;
	game.walls.push_back(barrier);
	game.walls[num].init(i * 64, j * 64);
	game.walls[num].m_sprite.setTexture(assets.walltex);
}

void AddTank(int i, int j, int num, int type)
{
	Tank tank;
	tank.vectorAdr = num;
	game.tanks.push_back(tank);
}

void SetupArena()
{
	// Setup arena

	int wallNum = 0;
	int tankNum = 0;
	Tank tank;
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			if (game.level[i][j] == 0) {
				AddWall(i, j, wallNum);
				wallNum += 1;
			}
			if (game.level[i][j] > 0) {
				tank.vectorAdr = tankNum;
				game.tanks.push_back(tank);
				//AddTank(i, j, tankNum, game.level[i][j] - 1);
				game.tanks[tankNum].init(game.level[i][j] - 1);
				game.tanks[tankNum].position = sf::Vector2f(
					(i * 64) + 32, (j * 64) + 32);
				tankNum += 1;
			}
		}
	}
}

int main()
{
	assets.tankBodys.reserve(1024);
	LoadSprites();

	// Setup player
	/*
	Tank tank;
	tank.init(0);;
	tank.vectorAd;
	tank.position;700);
	game.tanks.pu;

	Tank tank1;
	tank1.init(1);
	tank1.vectorAdr = 1;
	game.tanks.push_back(tank1);*/

	game.LoadConfig("test.ini");
	game.LoadLevel();
	SetupArena();

	game.gameloop();

	return 0;
}