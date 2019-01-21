#include "Game.h"
#include <Windows.h>
#include <ctime>

using namespace std;

int main()
{
	sf::RenderWindow window(sf::VideoMode(WIN_SIZE, WIN_SIZE), "Hunger Games");
	Game g;

	window.setPosition(sf::Vector2i(10, 10));

	g.initMap();

	for (int i = 0; i < 10; i++)
		g.createRandPlayer();

	for (int i = 0; i < 50; i++)
		g.createRandFood();

	while (window.isOpen())
	{
		int startTime = clock();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		g.tick();
		if (rand() % 5 == 0)
			g.createRandFood();

		if (rand() % 20 == 0)
			g.createRandWeapon();

		if (rand() % 100 == 0)
			g.createRandPlayer();
		window.clear();
		g.draw(window);
		window.display();
		Sleep(max(20 - (clock()-startTime),0L));
	}

	return 0;
}