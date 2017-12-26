#include "level.h"

#include <iostream>
#include "tinyxml.h"

int Object::GetPropInt(string name)
{
	return atoi(properties[name].c_str());
}

float Object::GetPropFloat(string name)
{
	return strtod(properties[name].c_str(), NULL);
}

string Object::GetPropString(string name)
{
	return properties[name];
}

bool Level::LoadFromFile(std::string fname)
{
	TiXmlDocument lvlFile(fname.c_str());

		// Загрузка XML файла карты
	if (!lvlFile.LoadFile())
	{
		std::cout << "Loading level " << fname << " error\n";
		return false;
	}

		// Работа с контейнером map XML файла
	TiXmlElement *map;
	map = lvlFile.FirstChildElement("map");

		// Парсинг XML файла
	this->width = atoi(map->Attribute("width"));
	this->height = atoi(map->Attribute("height"));
	this->tileWidth = atoi(map->Attribute("tilewidth"));
	this->tileHeight = atoi(map->Attribute("tileheight"));

		// Работа с контейнером tileset
	TiXmlElement *tilesetElmt;
	tilesetElmt = map->FirstChildElement("tileset");
	this->firstTileID = atoi(tilesetElmt->Attribute("firstgid"));

	
		// Парсинг пути к тайлсету
	TiXmlElement *imgElmt;
	imgElmt = tilesetElmt->FirstChildElement("image");
	std::string imgPath = imgElmt->Attribute("source");
	

	sf::Image img;
		// Загрузка тайлсета
	if (!img.loadFromFile(imgPath))
	{
		std::cout << "Failed to load tileset\n";
		return false;
	}

	
		// "Очищение" тайлсета от севта (109, 159, 185)
	//img.createMaskFromColor(sf::Color(109, 159, 185));
	
		// Загрузка текстуры из тайлсета
	this->tilesetImage.loadFromImage(img);
		// Отключение расплывчивости
	this->tilesetImage.setSmooth(false);

		// Вычисление количества столбцов тайлсета
	int cols = this->tilesetImage.getSize().x / tileWidth;
		// Вычисление кол-ва строк тайлсета
	int rows = this->tilesetImage.getSize().y / tileHeight;

		// Создание и заполнения вектора из прямоугольников изображений тайлсета. По сути - разбивка тайлсета на отдельные спрайты
	std::vector<sf::Rect<int>> subRects;

	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			sf::Rect<int> t_rect;

			t_rect.top = y * tileHeight;
			t_rect.height = tileHeight;
			t_rect.left = x * tileWidth;
			t_rect.width = tileWidth;

			subRects.push_back(t_rect);
		}
	}
	
		//	Работа со слоями
	TiXmlElement *layerElement;
	layerElement = map->FirstChildElement("layer");
	
		//	Обработка всех слоев
    while(layerElement)
    {
        Layer layer;
		
		// Если присутствует opacity, то задаем прозрачность слоя, иначе он полностью непрозрачен
        if (layerElement->Attribute("opacity") != NULL)
        {
            float opacity = strtod(layerElement->Attribute("opacity"), NULL);
            layer.opacity = 255 * opacity;
        }
        else
        {
            layer.opacity = 255;
        }

		// Контейнер <data>
        TiXmlElement *layerDataElement;
        layerDataElement = layerElement->FirstChildElement("data");

        if(layerDataElement == NULL)
        {
            std::cout << "Bad map. No layer information found." << std::endl;
        }

		// Контейнер <tile> - описание тайлов каждого слоя
        TiXmlElement *tileElement;
        tileElement = layerDataElement->FirstChildElement("tile");

        if(tileElement == NULL)
        {
            std::cout << "Bad map. No tile information found." << std::endl;
            return false;
        }

        int x = 0;
        int y = 0;

        while(tileElement)
        {
			int tileGID = 0;

			if (tileElement->Attribute("gid") != NULL)
			{
				tileGID = atoi(tileElement->Attribute("gid"));
			}

            int subRectToUse = tileGID - firstTileID;

			// Устанавливаем TextureRect каждого тайла
            if (subRectToUse >= 0)
            {
                sf::Sprite sprite;
                sprite.setTexture(tilesetImage);
				sprite.setTextureRect(subRects[subRectToUse]);
                sprite.setPosition(x * tileWidth, y * tileHeight);
                sprite.setColor(sf::Color(255, 255, 255, layer.opacity));

                layer.tiles.push_back(sprite);
            }

            tileElement = tileElement->NextSiblingElement("tile");

            x++;
            if (x >= width)
            {
                x = 0;
                y++;
                if(y >= height)
                    y = 0;
            }
        }

        layers.push_back(layer);

        layerElement = layerElement->NextSiblingElement("layer");
    }

		//	Работа с объектами
	TiXmlElement *objectGroupElement;
		
		//	Если есть слои объектов
	if (map->FirstChildElement("objectgroup") != NULL)
	{
		objectGroupElement = map->FirstChildElement("objectgroup");
			
			// Заполнение массива объектов аналогично слоям
		while (objectGroupElement)
		{
				// Контейнер <object>
			TiXmlElement *objectElmt;
			objectElmt = objectGroupElement->FirstChildElement("object");
				
				//	Добавление всех object группы objectgroup в поле класса objects
			while (objectElmt)
			{
					
					//	Определение типа объекта
				std::string objectType;
				if (objectElmt->Attribute("type") != NULL)
				{
					objectType = objectElmt->Attribute("type");
				}
				else
				{
					objectType = "defaultType";
				}

					// Определение имени объекта
				std::string objectName;
				if (objectElmt->Attribute("name") != NULL)
				{
					objectName = objectElmt->Attribute("name");
				}
				else
				{
					objectName = "unnamedObj";
				}

				int x = atoi(objectElmt->Attribute("x"));
				int y = atoi(objectElmt->Attribute("y"));

				int obj_width, obj_height;

					//	Определение и присваивание спрайта объекта
				sf::Sprite obj_sprite;
				obj_sprite.setTexture(tilesetImage);
				obj_sprite.setTextureRect(sf::Rect<int>(0, 0, 0, 0));
				obj_sprite.setPosition(x, y);

					//	Если не заданы размеры объекта - размерами объекта будут размеры спрайта
				if (objectElmt->Attribute("width") != NULL && (objectName == "block"))
				{
					obj_width = atoi(objectElmt->Attribute("width"));
					obj_height = atoi(objectElmt->Attribute("height"));
				}
				else
				{
					obj_width = subRects[atoi(objectElmt->Attribute("gid")) - firstTileID].width;
					obj_height = subRects[atoi(objectElmt->Attribute("gid")) - firstTileID].height;
					obj_sprite.setTextureRect(subRects[atoi(objectElmt->Attribute("gid")) - firstTileID]);
				}

					//	Заполнение структуры объекта полученными полями
				Object obj;
				obj.name = objectName;
				obj.type = objectType;
				obj.sprite = obj_sprite;

				sf::Rect<int> objRect;
				objRect.top = y;
				objRect.left = x;
				objRect.height = obj_height;
				objRect.width = obj_width;

				obj.rect = objRect;

					//	Свойства(переменные) объекта
				TiXmlElement *obj_props;
				obj_props = objectElmt->FirstChildElement("properties");
				if (obj_props != NULL)
				{
					TiXmlElement *prop;
					prop = obj_props->FirstChildElement("property");
					if (prop != NULL)
					{
						while (prop)
						{
							std::string propName = prop->Attribute("name");
							std::string propVal = prop->Attribute("value");

							obj.properties[propName] = propVal;

							prop = prop->NextSiblingElement("property");
						}
					}
				}
					//	Заполнение поля класса объектов
				this->objects.push_back(obj);

				objectElmt = objectElmt->NextSiblingElement("object");
			}

			objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
		}
	}
	else
	{
		std::cout << "No object found.\n";
	}
	
	return true;
}

Object Level::GetObject(std::string name)
{
	for (int i = 0; i < this->objects.size(); ++i)
	{
		if (this->objects[i].name == name)
			return this->objects[i];
	}
	return Object();
}

std::vector<Object> Level::GetObjects(std::string name)
{
	std::vector<Object> ret;
	for (int i = 0; i < this->objects.size(); ++i)
	{
		if (this->objects[i].name == name)
			ret.push_back(this->objects[i]);
	}
	return ret;
}

sf::Vector2i Level::GetTileSize()
{
	return sf::Vector2i(this->tileWidth, this->tileHeight);
}

sf::Vector2i Level::GetTileCount()
{
	return sf::Vector2i(this->width, this->height);
}

void Level::Draw(sf::RenderWindow &window)
{
	for (int lr = 0; lr < this->layers.size(); ++lr)
	{
		for (int tile = 0; tile < this->layers[lr].tiles.size(); ++tile)
			window.draw(this->layers[lr].tiles[tile]);
	}
}