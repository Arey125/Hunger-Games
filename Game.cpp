#include "Game.h"

//---------------Методы игрока------------------------

bool correct(int x, int y)
{
	return (x >= 0 && x < FIELD_SIZE && y >= 0 && y < FIELD_SIZE);
}

void Player::setGame(Game &game1)
{
	game = &game1;
}

double getDist(int x, int y, Food f)
{
	return (abs(f.x - x) + abs(f.y - y) + 0.1);
}
double getDist2(int x, int y, Food f)
{
	return getDist(x, y, f) * getDist(x, y, f);
}
double getDist(int x, int y, Weapon w)
{
	return (abs(w.x - x) + abs(w.y - y) + 0.1);
}
double getDist2(int x, int y, Weapon w)
{
	return getDist(x, y, w) * getDist(x, y, w);
}
double getDist(int x, int y, Player p)
{
	return (abs(p.x - x) + abs(p.y - y) + 0.1);
}
double getDist2(int x, int y, Player p)
{
	return getDist(x, y, p) * getDist(x, y, p);
}

double Player::getFoodPotential(int x, int y,Food f)
{
	return f.m*dmg * (1.0 / getDist2(x,y,f));
}

double Player::getWeaponPotential(int x, int y, Weapon w)//?
{
	return w.m*hp * (1.0 / getDist2(x, y, w));
}

double Player::getPlayerPotential(int x1, int y1, Player p)//?
{
	if (x == p.x && y == p.y)//Если позиции игроков совпадают
		return 0;			 //Выводим 0

	return 1.0 / ((p.hp*p.dmg < hp*dmg ? p.hp*p.dmg : -hp*dmg)*getDist2(x, y, p));
}

double Player::getPotential(int x1, int y1)//?
{
	double foodPot = 0, weaponPot = 0, playerPot = 0;

	for (auto &i : game->food)
		foodPot += getFoodPotential(x1, y1, i);

	for (auto &i : game->weapon)
		foodPot += getWeaponPotential(x1, y1, i);

	for (auto &i : game->players)
		playerPot += getPlayerPotential(x1, y1, i);

	return cPlayer*playerPot + cFood*foodPot + cWeapon*weaponPot;
}

void Player::move()
{
	playerColor = sf::Color(min(255, dmg * 4 + 105), min(255, hp + 105), max(0,105-(hp+dmg)/2));

	int dx = 0, dy = -1;

	int xMax = x, yMax = y;
	double maxPot = getPotential(x,y)-0.00001;

	for (int i = 0; i < 4; i++)
	{
		swap(dx, dy);
		dx = -dx;

		int x1 = x + dx, y1 = y + dy;
		if (!correct(x1, y1))
			continue;

		double pot = getPotential(x1,y1);

		if (pot >= maxPot)
		{
			xMax = x1;
			yMax = y1;
			maxPot = pot;
		}
	}

	x = xMax;
	y = yMax;
}

//---------------Методы игры------------------------

void Game::initMap()
{
	timer = 0;
	field.assign(FIELD_SIZE, vector<pair<Cell::CellPointer, int>>(FIELD_SIZE, pair<Cell::CellPointer, int>(Cell::Empty, -1)));
}

bool Game::empty(int x, int y)
{
	return field[x][y].first == Cell::Empty;
}

void Game::createFood(Food f)//Создание еды в предположении, что это не вызовет конфликтов
{
	food.push_back(f);
	field[f.x][f.y] = make_pair(Cell::Food, food.size() - 1);
}

void Game::createWeapon(Weapon w)
{
	weapon.push_back(w);
	field[w.x][w.y] = make_pair(Cell::Weapon, weapon.size() - 1);
}

void Game::createRandFood()//Создание еды в предположении, что это не вызовет конфликтов
{
	while (1)
	{
		Food f(rand() % FIELD_SIZE, rand() % FIELD_SIZE, rand() % 5);
		if (empty(f.x, f.y))
		{
			createFood(f);
			return;
		}
	}
}

void Game::createRandWeapon()
{
	while (1)
	{
		Weapon w(rand() % FIELD_SIZE, rand() % FIELD_SIZE, rand() % 5);
		if (empty(w.x, w.y))
		{
			createWeapon(w);
			return;
		}
	}
}

void Game::createPlayer(Player p)//Создание игрока в предположении, что это не вызовет конфликтов
{
	p.setGame(*this);
	players.push_back(p);
	field[p.x][p.y] = make_pair(Cell::Player, players.size() - 1);
}

void Game::createRandPlayer()//Создание еды в предположении, что это не вызовет конфликтов
{
	while (1)
	{
		Player p(rand() % FIELD_SIZE, rand() % FIELD_SIZE, 10, 1, idGen++,1 + (rand() % 30) / 10.0, 1 + (rand() % 30) / 10.0, 1 + (rand() % 30) / 10.0);
		if (empty(p.x, p.y))
		{
			createPlayer(p);
			return;
		}
	}
}

void Game::updateFood()
{
	for (int i = 0; i < food.size(); i++)
		field[food[i].x][food[i].y] = make_pair(Cell::Food, i);
}

void Game::updatePlayers()
{
	for (int i = 0; i < players.size(); i++)
		field[players[i].x][players[i].y] = make_pair(Cell::Player, i);
}

void Game::updateWeapon()
{
	for (int i = 0; i < weapon.size(); i++)
		field[weapon[i].x][weapon[i].y] = make_pair(Cell::Weapon, i);
}

void Game::foodCollision(int pInd, int fInd)//Столкноаение игрока с едой
{
	players[pInd].hp += food[fInd].m;
	food.erase(food.begin() + fInd);
	field[players[pInd].x][players[pInd].y] = make_pair(Cell::Player, pInd);
	updateFood();
}

void Game::weaponCollision(int pInd, int wInd)
{
	players[pInd].dmg += weapon[wInd].m;
	weapon.erase(weapon.begin() + wInd);
	field[players[pInd].x][players[pInd].y] = make_pair(Cell::Player, pInd);
	updateWeapon();
}

void Game::playerCollision(int p1Ind, int p2Ind)//Столкноаение игрока с игроком, второй был неподвижным
{
	Player *p1 = &players[p1Ind];
	Player *p2 = &players[p2Ind];

	if (p1->dmg*p1->hp < p2->dmg*p2->hp)
	{
		swap(p1, p2);
		swap(p1Ind, p2Ind);
	}

	p1->hp -= p2->hp*p2->dmg / p1->dmg;
	players.erase(players.begin() + p2Ind);
	updatePlayers();

	if (p1->hp > 0)
		return;

	players.erase(players.begin() + field[p1->x][p1->y].second);
	updatePlayers();
}

void Game::move(int ind)
{
	Player &p = players[ind];

	field[p.x][p.y].first = Cell::Empty;
	p.move();

	if (field[p.x][p.y].first == Cell::Empty)
	{
		field[p.x][p.y] = make_pair(Cell::Player,ind);
		return;
	}

	if (field[p.x][p.y].first == Cell::Food)
		foodCollision(ind, field[p.x][p.y].second);
	else if (field[p.x][p.y].first == Cell::Weapon)
		weaponCollision(ind, field[p.x][p.y].second);
	else
		playerCollision(ind, field[p.x][p.y].second);
}

void Game::tick()
{
	int size = players.size();
	for (int i = 0; i < players.size(); i++)
	{
		int id = players[i].id;
		move(i);
		if (size != players.size())
		{
			size--;
			if (i == size || id != players[i].id)
				i--;
		}
	}
	timer++;
}

void Game::draw(sf::RenderWindow &win)
{
	sf::RectangleShape rect(sf::Vector2f(LEN, LEN));
	for (int i = 0; i < food.size(); i++)
	{
		rect.setPosition(sf::Vector2f(LEN*food[i].x, LEN*food[i].y));
		rect.setFillColor(sf::Color(0, 255, 0));

		win.draw(rect);
	}

	for (int i = 0; i < weapon.size(); i++)
	{
		rect.setPosition(sf::Vector2f(LEN* weapon[i].x, LEN* weapon[i].y));
		rect.setFillColor(sf::Color(255, 0, 0));

		win.draw(rect);
	}

	for (int i = 0; i < players.size(); i++)
	{
		rect.setPosition(sf::Vector2f(LEN*players[i].x, LEN*players[i].y));
		rect.setFillColor(players[i].playerColor);

		win.draw(rect);
	}
}