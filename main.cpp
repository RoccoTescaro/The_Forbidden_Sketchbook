#include "hdr/Config.h"
#include "hdr/Utils.h"
#include "hdr/Map.h"
#include "hdr/Tile.h"


int main() 
{

	{
		Map map;

		for (int i = 0; i < 10; i++) 
		{
			Wall* newWall = new Wall{0};
			map.add(map.posFloatToInt(sf::Vector2<float>{std::rand()*0.25f,std::rand()*0.25f}),newWall);
		}


		Archive arc("test.tfs", Archive::Save);
		arc << map;

	}

	std::cout << std::endl;
	
	{
		Map map;

		Archive arc("test.tfs", Archive::Load);
		arc >> map;

	}

	Application::run();
	return 0;

}