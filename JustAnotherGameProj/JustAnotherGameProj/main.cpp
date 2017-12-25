#include <SFML/Graphics.hpp>
#include "main.h"

using namespace sf;

int main()
{
	srand(time(NULL));

	Level level;
	level.LoadFromFile("JAGfirst.tmx");

	b2Vec2 gravity(0.0f, 0.1f);
	b2World world(gravity);

	Vector2i tileSize = level.GetTileSize();
	
	std::vector<Object> block = level.GetObjects("block");

	for (int i = 0; i < block.size(); i++)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(block[i].rect.left + tileSize.x / 2 * (block[i].rect.width / tileSize.x),
			block[i].rect.top + tileSize.y / 2 * (block[i].rect.height / tileSize.y));
		b2Body* body = world.CreateBody(&bodyDef);
		b2PolygonShape shape;
		shape.SetAsBox(block[i].rect.width / 2, block[i].rect.height / 2);
		body->CreateFixture(&shape, 1.0f);
	}
	
	coin = level.GetObjects("coin");
	for (int i = 0; i < coin.size(); i++)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(coin[i].rect.left + tileSize.x / 2 * (coin[i].rect.width / tileSize.x),
			coin[i].rect.top + tileSize.y / 2 * (coin[i].rect.height / tileSize.y));
		bodyDef.fixedRotation = true;
		b2Body* body = world.CreateBody(&bodyDef);
		b2PolygonShape shape;
		shape.SetAsBox(coin[i].rect.width / 2, coin[i].rect.height / 2);
		body->CreateFixture(&shape, 1.0f);
		coinBody.push_back(body);
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
	fixtureDef.density = 0.5f; 
	fixtureDef.friction = 0.1f;
	playerBody->CreateFixture(&fixtureDef);

	Vector2i screenSize(800, 600);

	RenderWindow window;
	window.create(sf::VideoMode(screenSize.x, screenSize.y, 32), "Test");

	View view;
	view.reset(FloatRect(0.0f, 0.0f, screenSize.x, screenSize.y));
	view.setViewport(FloatRect(0.0f, 0.0f, 4.0f, 4.0f));

	static int frameLimit = 0;

	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		world.Step(1 / 400.0f, 1, 1);
		
		bool onGround = false;
		b2Vec2 posTest = playerBody->GetPosition();
		posTest.y += tileSize.y;
		
		for (b2Body *it = world.GetBodyList(); it != 0; it = it->GetNext())
		{
			for (b2Fixture *f = it->GetFixtureList(); f != 0; f = f->GetNext())
			{
				if (f->TestPoint(posTest))
					onGround = true; 
				posTest.x += player.rect.width / 2;
				if (f->TestPoint(posTest))
					onGround = true;
				posTest.x -= player.rect.width;
				if (f->TestPoint(posTest))
					onGround = true;
				posTest.x += player.rect.width / 2;
			}
		}

		b2Vec2 playerVel = playerBody->GetLinearVelocity();

		if (abs(playerVel.x) > 0.2f)
		{
			if (playerVel.x > 0.2f)
			{
				playerBody->ApplyLinearImpulse(b2Vec2(-0.05f, 0.0f), playerBody->GetWorldCenter(), true);
			}
			else if (playerVel.x < -0.2f)
			{
				playerBody->ApplyLinearImpulse(b2Vec2(0.05f, 0.0f), playerBody->GetWorldCenter(), true);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::D) && (playerVel.x < 2.0f)  && (onGround))
			playerBody->ApplyLinearImpulse(b2Vec2(0.5f, 0.0f), playerBody->GetWorldCenter(), true);
		else if (Keyboard::isKeyPressed(Keyboard::D) && (!onGround))
			playerBody->ApplyLinearImpulse(b2Vec2(0.05f, 0.0f), playerBody->GetWorldCenter(), true);
			
		if (Keyboard::isKeyPressed(Keyboard::A) && (playerVel.x > -2.0f) && (onGround))
			playerBody->ApplyLinearImpulse(b2Vec2(-0.5f, 0.0f), playerBody->GetWorldCenter(), true);
		
		else if (Keyboard::isKeyPressed(Keyboard::A) && (!onGround))
			playerBody->ApplyLinearImpulse(b2Vec2(-0.05f, 0.0f), playerBody->GetWorldCenter(), true);
			

		if (Keyboard::isKeyPressed(Keyboard::W) && onGround && (playerVel.y > -3.0f))
			playerBody->ApplyLinearImpulse(b2Vec2(0.0f, -0.3f), playerBody->GetWorldCenter(), true);
		
		

		/*
		for (b2ContactEdge* ce = playerBody->GetContactList(); ce; ce = ce->next)
		{
			b2Contact* contact = ce->contact;
		}
		*/

		frameLimit++;
		if (frameLimit == 60)
		{
			b2Vec2 pos = playerBody->GetPosition();
			view.setCenter(pos.x + screenSize.x / 2.6, pos.y + screenSize.y / 2.85);
			window.setView(view);

			player.sprite.setPosition(pos.x, pos.y);

			for (int i = 0; i < coin.size(); i++)
				coin[i].sprite.setPosition(coinBody[i]->GetPosition().x, coinBody[i]->GetPosition().y);

			window.clear(Color::Color(0xAB, 0xF9, 0xFF, 0xFF));
			level.Draw(window);
			window.draw(player.sprite);
			for (int i = 0; i < coin.size(); i++)
				window.draw(coin[i].sprite);


			window.display();

			frameLimit = 0;
		}
	}

	return 0;
}