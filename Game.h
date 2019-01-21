#pragma once
#include <SFML/Graphics.hpp>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int WIN_SIZE = 800;
const int FIELD_SIZE = 100;
const int LEN = WIN_SIZE / FIELD_SIZE;

bool correct(int x, int y);

namespace Cell
{
	enum CellPointer
	{
		Empty,
		Food,
		Weapon,
		Player
	};
}

class Food;
class Weapon;
class Game;

class Player
{
public:
	int x, y, hp,dmg;
	int id;

	sf::Color playerColor;

	double cPlayer, cFood, cWeapon;
	Game *game;

	Player() {}

	Player(int x, int y, int hp, int dmg, int id,double cp = 1, double cf = 1, double cw = 1) :x(x), y(y), hp(hp), dmg(dmg), id(id), cPlayer(cp), cFood(cf), cWeapon(cw)
	{
		playerColor = sf::Color(rand() % 128 + 128, rand() % 128 + 128, rand() % 128 + 128);
		if (rand() % 2)
			playerColor.r -= 128;
		if (rand() % 2)
			playerColor.g -= 128;
		if (rand() % 2)
			playerColor.b -= 128;
	}

	void setGame(Game &game1);

	double getFoodPotential(int x, int y, Food f);
	double getWeaponPotential(int x, int y, Weapon w);
	double getPlayerPotential(int x, int y, Player f);
	double getPotential(int x, int y);

	void move();
};

class Food
{
public:
	int x, y, m;
	
	Food() {}

	Food(int x, int y, int m) :x(x), y(y), m(m){};
};

class Weapon
{
public:
	int x, y, m;

	Weapon() {}

	Weapon(int x, int y, int m) :x(x), y(y), m(m){};
};

class Game
{
public:
	vector<Player> players;
	vector<Food> food;
	vector<Weapon> weapon;

	vector < vector<pair<Cell::CellPointer, int>>> field;//Поле игры с указателями на содержащиеся объекты в каждой позиции

	int timer;
	int idGen;

	void initMap();

	bool empty(int x, int y);

	void createPlayer(Player p);
	void createFood(Food f);
	void createWeapon(Weapon w);

	void createRandFood();
	void createRandPlayer();
	void createRandWeapon();

	void foodCollision(int pInd, int fInd);
	void weaponCollision(int pInd, int fInd);
	void playerCollision(int p1Ind, int p2Ind);

	void updateFood();
	void updateWeapon();
	void updatePlayers();

	void move(int ind);
	void tick();

	void draw(sf::RenderWindow &win);
};