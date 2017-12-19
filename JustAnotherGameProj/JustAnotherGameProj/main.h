#include <Box2D/Box2D.h>
#include "Level.h"

#include <iostream>
#include <random>

Object player;
b2Body *playerBody;

std::vector<Object> coin;
std::vector<b2Body*> coinBody;

std::vector<Object> enemy;
std::vector<b2Body*> enemyBody;