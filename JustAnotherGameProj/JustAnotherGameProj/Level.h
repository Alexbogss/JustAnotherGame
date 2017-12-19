#ifndef LEVEL_H
#define LEVEL_H

#pragma comment(lib,"Box2D.lib")
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-system.lib")

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

using namespace std;

/*
	��������� �������.
		������ �������� �� XML ����� ������, ��� ��������� ��������� �������. ��������� ������������ � properies.
		�� properties ����� ��������:
			name - ��� �������
			type - ��� ���
			rect - �������������, ����������� ������
			sprite - ����������� (����� �������� �������)
*/
struct Object								
{
	int GetPropInt(string name);
	float GetPropFloat(string name);
	string GetPropString(string name);

	string name;
	string type;
	sf::Rect<int> rect;
	map <string, string> properties;

	sf::Sprite sprite;
};


/*
	��������� ����.
		opacity - ������������ ����
		tiles - ������ �� ������
*/
struct Layer
{
	int opacity;
	vector<sf::Sprite> tiles;
};


/*
	����� ������.
		������ ������:
			LoadFromFile - ��������� ����� �� ����� fname
			GetObject - ���������� ������ ������ � ��������� ������
			GetObjects - ���������� ������ �������� � ��������� ������
			Draw - ������ ��� �����(!) �� ���������� RenderWindow
			GetTileSize - ���������� ������ �������� ������� ����� ������
		���� ������:
			wigth, height - ������ ������ � ������
			tileWidth, tileHeight - ������ ����� � ��������
			firstTileID - ID ������� �����
			tilesetImage - ��� ������� (��������)
			objects - ������ ���� �������� ������
			layers - ������ ���� ����� ������
*/
class Level
{
private:
	int width, height;
	int tileWidth, tileHeight;
	int firstTileID;
	sf::Rect<float> drawingBounds;
	sf::Texture tilesetImage;
	vector<Object> objects;
	vector<Layer> layers;

public:
	bool LoadFromFile(string fname);
	Object GetObject(string name);
	vector<Object> GetObjects(string name);
	void Draw(sf::RenderWindow &window);
	sf::Vector2i GetTileSize();
};

#endif