#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Level.h"

using namespace sf;

int main()
{
	Level level;
	level.LoadFromFile("newtestmap.tmx");

	sf::RenderWindow window;
	window.create(sf::VideoMode(800, 600), "Test");

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();
		level.Draw(window);
		window.display();
	}

	return 0;
}