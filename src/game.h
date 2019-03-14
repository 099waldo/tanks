#pragma once
#include "vector"
#include "SFML/Graphics.hpp"
#include "INIReader.h"

#include "tank.h"
#include "barrier.h"
#include "bullet.h"

#include "level.h"
#include "tanksettings.h"

class Game {
    public:
	std::vector<Tank> tanks;
	std::vector<Barrier> walls;
	std::vector<Bullet> bullets;

	std::vector<TankSettings> tankSettings;

	int level[16][16];

	bool mousePressed = false;

	int LoadLevel()
	{
		Level lev;
		lev.LoadFromFile();
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				level[i][j] = lev.grid[i][j];
			}
		}
	}

	TankSettings LoadTank(std::string name, INIReader reader)
	{
		TankSettings tank0;
		tank0.speed = reader.GetReal(name, "speed", 0);
		tank0.maxBuls = reader.GetInteger(name, "maxBullets", 5);
		tank0.player = reader.GetBoolean(name, "player", false);
		tank0.shootingProb = reader.GetInteger(name, "shootingProb", 2);
		tank0.turnIfTouchingTank =
			reader.GetBoolean(name, "turnIfTouchingTank", false);
		return tank0;
	}

	void LoadConfig(std::string file)
	{
		INIReader reader(file);

		if (reader.ParseError() != 0) {
			std::cout << "Can't load config file" << std::endl;
		}

		tankSettings.push_back(LoadTank("tank0", reader));
		tankSettings.push_back(LoadTank("tank1", reader));
		tankSettings.push_back(LoadTank("tank2", reader));
		tankSettings.push_back(LoadTank("tank3", reader));
		tankSettings.push_back(LoadTank("tank4", reader));
		tankSettings.push_back(LoadTank("tank5", reader));
		tankSettings.push_back(LoadTank("tank6", reader));
	}

	void gameloop()
	{
		sf::RenderWindow window(sf::VideoMode(1024, 1024),
					"Waldo's Tanks");
		window.setFramerateLimit(60);
		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
				}
				if (event.type == sf::Event::Resized) {
					// update the view to the new size of the window
					sf::FloatRect visibleArea(
						0, 0, event.size.width,
						event.size.height);
					window.setView(sf::View(visibleArea));
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				window.close();
			}
			int pT = 0;
			for (int i = 0; i < tanks.size(); i++) {
				if (tanks[i].player) {
					pT = i;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				tanks[pT].velocity.x = 1;
			} else if (sf::Keyboard::isKeyPressed(
					   sf::Keyboard::A)) {
				tanks[pT].velocity.x = -1;
			} else {
				tanks[pT].velocity.x = 0;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				tanks[pT].velocity.y = 1;
			} else if (sf::Keyboard::isKeyPressed(
					   sf::Keyboard::S)) {
				tanks[pT].velocity.y = -1;
			} else {
				tanks[pT].velocity.y = 0;
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
			    mousePressed == false) {
				// left mouse button is pressed: shoot
				tanks[pT].Shoot();
				mousePressed = true;
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) ==
			    false) {
				mousePressed = false;
			}

			// Draw Walls
			window.clear(sf::Color::White);
			for (int i = 0; i < walls.size(); i++) {
				window.draw(walls[i]);
			}

			// Update Tanks
			for (int i = 0; i < tanks.size(); i++) {
				tanks[i].update();
				if (tanks[i].player) {
					sf::Vector2f mousepos = (sf::Vector2f)
						sf::Mouse::getPosition(window);
					tanks[i].PointTurretAt(mousepos);
					tanks[i].FaceVelocityDir();
				} else {
					tanks[i].PointTurretAt(
						tanks[i].playerPos());
					tanks[i].FaceVelocityDir();
					if (tanks[i].randomint() <
						    tanks[i].shootingProb &&
					    tanks[i].activeBuls <
						    tanks[i].maxBuls) {
						tanks[i].Shoot();
					}
				}
				window.draw(tanks[i]);
			}

			// Update bullets
			std::vector<int> bulletsToDelete;
			for (int i = 0; i < bullets.size(); i++) {
				bullets[i].vectorAdr = i;
				if (bullets[i].update() == 1) {
					bulletsToDelete.push_back(i);
				}

				if (bullets.size() > 0) {
					window.draw(bullets[i]);
				}
			}

			// Delete bullets that got destroyed this frame
			for (int i = 0; i < bulletsToDelete.size(); i++) {
				bullets.erase(bullets.begin() +
					      bulletsToDelete[i]);
			}
			window.display();
		}
	}
};

struct Assets {
	sf::Texture walltex;
	sf::Texture bullettex;

	std::vector<sf::Texture> tankBodys;
	std::vector<sf::Texture> tankTurrets;
};