#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

class Level {
    public:
	int grid[16][16];

	void LoadFromFile()
	{
		std::ifstream file("levels/testlevel.csv");
		if (file.is_open()) {
			std::stringstream ss;
			ss << file.rdbuf();

			std::vector<int> vect;

			int i;

			while (ss >> i) {
				vect.push_back(i);

				if (ss.peek() == ',' || ss.peek() == ' ')
					ss.ignore();
			}

			int num = 0;
			for (i = 0; i < 16; i++) {
				for (int j = 0; j < 16; j++) {
					grid[j][i] = vect[num];
					num += 1;
				}
			}

			/*
			for (i = 0; i < 16; i++) {
				for (int j = 0; j < 16; j++) {
					std::cout << grid[i][j];
				}
				std::cout << std::endl;
			}
			*/

			file.close();
		}
	}
};