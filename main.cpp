#include "hdr/Utils.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "hdr/Application.h"

int main() 
{
	LOG("%", Utils::Math::manhattanDistance<int>(sf::Vector2<int>{3, 3}, sf::Vector2<int>{3, 12}));
	return 0;
}