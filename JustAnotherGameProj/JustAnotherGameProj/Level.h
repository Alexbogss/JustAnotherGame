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
	Структура объекта.
		Объект читается из XML файла уровня, где прописаны параметры объекта. Параметры записываются в properies.
		Из properties можно получить:
			name - имя объекта
			type - его тип
			rect - прямоугольник, описывающий объект
			sprite - изображение (часть тайлсета объекта)
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
	Структура слоя.
		opacity - прозрачность слоя
		tiles - список из тайлов
*/
struct Layer
{
	int opacity;
	vector<sf::Sprite> tiles;
};


/*
	Класс уровня.
		Методы класса:
			LoadFromFile - загружает карту из файла fname
			GetObject - возвращает первый объект с указанным именем
			GetObjects - возвращает список объектов с указанным именем
			Draw - рисует все тайлы(!) по экземпляру RenderWindow
			GetTileSize - возвращает вектор значения размера тайла уровня
		Поля класса:
			wigth, height - размер уровня в тайлах
			tileWidth, tileHeight - размер тайла в пикселях
			firstTileID - ID первого тайла
			tilesetImage - сам тайлсет (текстура)
			objects - список всех объектов уровня
			layers - список всех слоев уровня
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