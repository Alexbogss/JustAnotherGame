#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "main.h"

using namespace sf;

int main()
{
	srand(time(NULL));

	Vector2i screenSize(800, 600);

	RenderWindow window;
	window.create(sf::VideoMode(screenSize.x, screenSize.y, 32), "Test");

	Texture menu_texture1, menu_texture3, about_texture;
	menu_texture1.loadFromFile("111.png");
	menu_texture3.loadFromFile("333.png");
	Sprite menu1(menu_texture1), menu3(menu_texture3), about(about_texture);
	bool Menu = 1;
	int MenuNum = 0;
	menu1.setPosition(300, 200);
	menu3.setPosition(300, 260);

	static int frameLimit = 0;

	while (Menu)
	{
		menu1.setColor(Color::White);
		menu3.setColor(Color::White);
		MenuNum = 0;
		
		if (IntRect(300, 200, 300, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Yellow); MenuNum = 1; }
		if (IntRect(300, 260, 300, 50).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Yellow); MenuNum = 3; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (MenuNum == 1) Menu = false;
			if (MenuNum == 3) { window.close(); Menu = false; }
		}

		frameLimit++;
		if (frameLimit == 500)
		{
			window.clear(Color(0, 0, 0));

			frameLimit = 0;
			window.draw(menu1);
			window.draw(menu3);

			window.display();
		}
	}
	frameLimit = 0;

	Level level;
	level.LoadFromFile("JAGfirst.tmx");

	b2Vec2 gravity(0.0f, 0.1f);
	b2World world(gravity);

	Vector2i tileSize = level.GetTileSize();
	Vector2i tileCount = level.GetTileCount();
	
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

	crate = level.GetObjects("crate");
	for (int i = 0; i < crate.size(); i++)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(crate[i].rect.left + tileSize.x / 2 * (crate[i].rect.width / tileSize.x),
			crate[i].rect.top + tileSize.y / 2 * (crate[i].rect.height / tileSize.y));
		bodyDef.fixedRotation = true;
		b2Body* body = world.CreateBody(&bodyDef);
		b2PolygonShape shape;
		shape.SetAsBox(crate[i].rect.width / 2, crate[i].rect.height / 2);
		body->CreateFixture(&shape, 1.0f);
		crateBody.push_back(body);
	}

	enemy = level.GetObjects("enemy");
	for (int i = 0; i < enemy.size(); i++)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(enemy[i].rect.left +
			tileSize.x / 2 * (enemy[i].rect.width / tileSize.x),
			enemy[i].rect.top + tileSize.y / 2 * (enemy[i].rect.height / tileSize.y ));
		bodyDef.fixedRotation = true;
		b2Body* body = world.CreateBody(&bodyDef);
		b2PolygonShape shape;
		shape.SetAsBox(enemy[i].rect.width / 2, enemy[i].rect.height / 2);
		body->CreateFixture(&shape, 1.0f);
		enemyBody.push_back(body);
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
	fixtureDef.friction = 0.0f;
	playerBody->CreateFixture(&fixtureDef);

	View view;
	view.reset(FloatRect(0.0f, 0.0f, screenSize.x, screenSize.y));
	view.setViewport(FloatRect(0.0f, 0.0f, 4.0f, 4.0f));

	SoundBuffer buffer;
	buffer.loadFromFile("Jump.ogg");
	Sound sound(buffer);

	static int enemyLimit = 0;

	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		world.Step(1 / 400.0f * (enemy.size() / 2.0f + 1), 1, 1);

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

		if (abs(playerVel.x) > 0.0f)
		{
			if (playerVel.x > 0.0f && (!Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::A)))
			{
				playerBody->SetLinearVelocity(b2Vec2(-0.5f, playerVel.y));
			}
			if (playerVel.x < -0.0f && (!Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::D)))
			{
				playerBody->SetLinearVelocity(b2Vec2(0.5f, playerVel.y));
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::D) && (playerVel.x < 2.5f)  && (onGround))
			playerBody->ApplyLinearImpulse(b2Vec2(0.3f, 0.0f), playerBody->GetWorldCenter(), true);
		else if (Keyboard::isKeyPressed(Keyboard::D) && (playerVel.x < 1.0f) && (!onGround))
			playerBody->ApplyLinearImpulse(b2Vec2(0.12f, 0.0f), playerBody->GetWorldCenter(), true);
			
		if (Keyboard::isKeyPressed(Keyboard::A) && (playerVel.x > -2.5f) && (onGround))
			playerBody->ApplyLinearImpulse(b2Vec2(-0.3f, 0.0f), playerBody->GetWorldCenter(), true);
		else if (Keyboard::isKeyPressed(Keyboard::A) && (playerVel.x > -1.0f) && (!onGround))
			playerBody->ApplyLinearImpulse(b2Vec2(-0.12f, 0.0f), playerBody->GetWorldCenter(), true);
			
		if (Keyboard::isKeyPressed(Keyboard::W) && onGround && (playerVel.y > -3.0f))
		{
			playerBody->ApplyLinearImpulse(b2Vec2(0.0f, -0.8f), playerBody->GetWorldCenter(), true);
			if (!sound.getStatus())
				sound.play();
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			return 0;

		if (playerBody->GetPosition().y > 200)
			return 0;

		enemyLimit++;

		if (enemyLimit == 75 && ((coin.size() > 0) || (enemy.size() > 0)))
		{
			bool check = false;

			for (b2ContactEdge* ce = playerBody->GetContactList(); ce; ce = ce->next)
			{
				b2Contact* contact = ce->contact;

				if (coin.size() > 0)
				{
					for (int i = 0; i < coinBody.size(); i++)
					{
						if (contact->GetFixtureA() == coinBody[i]->GetFixtureList())
						{
							sound.play();
							coinBody[i]->DestroyFixture(coinBody[i]->GetFixtureList());
							coin.erase(coin.begin() + i);
							coinBody.erase(coinBody.begin() + i);
							check = true;
							break;
						}
					}
				}

				if (enemy.size() > 0)
				{
					for (int i = 0; i < enemyBody.size(); i++)
					{
						if (contact->GetFixtureA() == enemyBody[i]->GetFixtureList())
						{
							if (playerBody->GetPosition().y < enemyBody[i]->GetPosition().y)
							{
								playerBody->SetLinearVelocity(b2Vec2(0.0f, -3.0f));
								sound.play();
								enemyBody[i]->DestroyFixture(enemyBody[i]->GetFixtureList());
								enemy.erase(enemy.begin() + i);
								enemyBody.erase(enemyBody.begin() + i);
							}
							else
							{
								int tmp = (playerBody->GetPosition().x < enemyBody[i]->GetPosition().x)
									? -1 : 1;
								playerBody->SetLinearVelocity(b2Vec2(0, -3.0f));
								playerBody->SetLinearVelocity(b2Vec2(5.0f * tmp, playerBody->GetLinearVelocity().y));
								sound.play();
							}
							check = true;
							break;
						}
					}
				}
				if (check) break;
			}
		}

		if (enemyLimit == 800 && (enemy.size() > 0))
		{
			enemyLimit = 0;
			for (int i = 0; i < enemyBody.size(); i++)
			{
				if (enemyBody[i]->GetLinearVelocity() == b2Vec2_zero)
				{
					int tmp = (rand() % 2 == 1) ? 1 : -1;
					enemyBody[i]->SetLinearVelocity(b2Vec2(1.0f * tmp, 0.0f));
				}
			}
		}

		frameLimit++;

		if (frameLimit == 30)
		{
			b2Vec2 pos = playerBody->GetPosition();
			static b2Vec2 temp;
			static bool check = true;
			if (pos.x + screenSize.x / 2.6 < screenSize.x / 2)
				view.setCenter(screenSize.x / 2, pos.y + screenSize.y / 2.85);
			else if (pos.x - screenSize.x / 2.9 > tileSize.x * tileCount.x - screenSize.x / 2)
			{
				if (check)	temp.x = pos.x;
				check = false;
				view.setCenter(temp.x + screenSize.x / 2.6, pos.y + screenSize.y / 2.85);
			}
			else
			{
				view.setCenter(pos.x + screenSize.x / 2.6, pos.y + screenSize.y / 2.85);
				check = true;
			}


			window.setView(view);
			player.sprite.setPosition(pos.x, pos.y);

			for (int i = 0; i < coin.size(); i++)
				coin[i].sprite.setPosition(coinBody[i]->GetPosition().x, coinBody[i]->GetPosition().y);

			for (int i = 0; i < crate.size(); i++)
				crate[i].sprite.setPosition(crateBody[i]->GetPosition().x, crateBody[i]->GetPosition().y);

			for (int i = 0; i < enemy.size(); i++)
				enemy[i].sprite.setPosition(enemyBody[i]->GetPosition().x, enemyBody[i]->GetPosition().y);

			window.clear(Color::Color(0xAB, 0xF9, 0xFF, 0xFF));
			level.Draw(window);
			window.draw(player.sprite);

			for (int i = 0; i < coin.size(); i++)
				window.draw(coin[i].sprite);

			for (int i = 0; i < crate.size(); i++)
				window.draw(crate[i].sprite);

			for (int i = 0; i < enemy.size(); i++)
				window.draw(enemy[i].sprite);

			window.display();

			frameLimit = 0;
		}
	}

	return 0;
}