#include "hdr/Config.h"
#include "hdr/Utils.h"
#include "hdr/GameCharacter.h"

int main() 
{
	{
		std::vector<std::unique_ptr<Entity>> vec;
		for (int i = 0; i < 10; i++)
		{
			std::unique_ptr<Entity> ptr = static_cast<std::unique_ptr<Entity>>(new Player{1,1,255,255,255});
			vec.push_back(std::move(ptr));
		}


		Archive arc("test.tfs", Archive::Save);
		arc << vec;

		for (auto& elm : vec)
			std::cout << elm->getTypeId() << std::endl;
	}
	std::cout << std::endl;
	{
		std::vector<std::unique_ptr<Entity>> vec;

		Archive arc("test.tfs", Archive::Load);
		arc >> vec;

		for (auto& elm : vec)
			std::cout << elm->getTypeId() << std::endl;
	}

	//Application::run();
	return 0;
}