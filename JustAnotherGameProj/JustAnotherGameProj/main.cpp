#include <SFML/Graphics.hpp>
#include "main.h"

using namespace sf;

int main()
{
	srand(time(NULL));

	Level level;
	level.LoadFromFile("platformer.tmx");

	b2Vec2 gravity(0.0f, 1.0f);
	b2World world(gravity);

	sf::Vector2i tileSize = level.GetTileSize();
	
	std::vector<Object> block = level.GetObjects("block");
	for (int i = 0; i < block.size(); i++)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(block[i].rect.left + tileSize.x / 2 * (block[i].rect.width / tileSize.x - 1),
			block[i].rect.top + tileSize.y / 2 * (block[i].rect.height / tileSize.y - 1));
		b2Body* body = world.CreateBody(&bodyDef);
		b2PolygonShape shape;
		shape.SetAsBox(block[i].rect.width / 2, block[i].rect.height / 2);
		body->CreateFixture(&shape, 1.0f);
	}

	player = level.GetObject("player");
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(player.rect.left, player.rect.top);
	bodyDef.fixedRotation = true;
	playerBody = world.CreateBody(&bodyDef);
	b2PolygonShape shape; 
	shape.SetAsBox(player.rect.width / 2, player.rect.height / 2);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f; 
	fixtureDef.friction = 0.3f;
	playerBody->CreateFixture(&fixtureDef);

	sf::Vector2i screenSize(800, 600);

	sf::RenderWindow window;
	window.create(sf::VideoMode(screenSize.x, screenSize.y), "Test");

	sf::View view;
	view.reset(sf::FloatRect(0.0f, 0.0f, screenSize.x, screenSize.y));
	view.setViewport(sf::FloatRect(0.0f, 0.0f, 2.0f, 2.0f));

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::W)
					playerBody->SetLinearVelocity(b2Vec2(0.0f, -10.0f));

				if (event.key.code == sf::Keyboard::D)
					playerBody->SetLinearVelocity(b2Vec2(5.0f, 0.0f));

				if (event.key.code == sf::Keyboard::A)
					playerBody->SetLinearVelocity(b2Vec2(-5.0f, 0.0f));
				break;
			}
		}

		world.Step(1.0f / 60.0f, 3, 3);

		/*
		for (b2ContactEdge* ce = playerBody->GetContactList(); ce; ce = ce->next)
		{
			b2Contact* contact = ce->contact;
		}
		*/

		b2Vec2 pos = playerBody->GetPosition();
		view.setCenter(pos.x + screenSize.x / 4, pos.y + screenSize.y / 4);
		window.setView(view);

		player.sprite.setPosition(pos.x, pos.y);

		for (int i = 0; i < coin.size(); i++)
			coin[i].sprite.setPosition(coinBody[i]->GetPosition().x, coinBody[i]->GetPosition().y);

		for (int i = 0; i < coin.size(); i++)
			window.draw(coin[i].sprite);

		window.clear();
		level.Draw(window);
		window.draw(player.sprite);
		window.display();
	}

	return 0;
}