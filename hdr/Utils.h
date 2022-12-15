#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#define DEBUGMODE 0 //for release set to 0, for debug set to 1

#if DEBUGMODE

#define ERROR(description) { Utils::error(description); }
#define LOG(string, ...) { Utils::debugLog(string, __VA_ARGS__); }
#define ASSERT(string, condition) { Utils::assert(string, condition); }

#else

#define ERROR(description)
#define LOG(string, ...)
#define ASSERT(string, boolean);

#endif


namespace Utils
{

	static void error(const char* description)
	{
		std::cout << "[ERROR] " << description << std::endl;
	}

    static void log(const char* string)
    {
        std::cout << string << std::endl;
    }

    template<typename T, typename ...Args>
    static void log(const char* string, T value, Args... args) 
    {

        for (; *string != '\0'; string++)
        {
            //substitute first "%" with first parameter converted to string
            if (*string == '%') 
            {
                std::cout << std::to_string(value);
                log(string + 1, args...);
                return;
            }
            std::cout << *string;
        }

    }

    template<typename ...Args>
    static void debugLog(const char* string, Args... args)
    {
        std::cout << "[LOG] ";
        log(string, args...);
    }

	static void assert(const char* string, bool condition) 
	{
		if (!condition)
			ERROR(string);
	}

    namespace Math 
    {
        template<typename Type>
        static Type distance(const sf::Vector2<Type>& pos1, const sf::Vector2<Type>& pos2)  //Euclidean distance
        {
            return std::sqrt(std::pow(pos1.x - pos2.x, 2) + std::pow(pos1.y - pos2.y, 2));
        }

        template<typename Type>
        static Type distance(Type pos1, Type pos2) 
        {
            return std::abs(pos2-pos1);
        }

        template<typename Type>
        static Type manhattanDistance(const sf::Vector2<Type>& pos1, const sf::Vector2<Type>& pos2)
        {
            return distance(pos1.x,pos2.x) + distance(pos1.y,pos2.y);
        }

        template<typename Type>
        static sf::Vector2<Type> lerp(const sf::Vector2<Type>& pos1, const sf::Vector2<Type>& pos2, Type dt) //Linear interpolation
        {
            return sf::Vector2<Type>{lerp(pos1.x, pos2.x, dt), lerp(pos1.y, pos2.y, dt)};
        }

        template<typename Type>
        static Type lerp(Type pos1, Type pos2, Type dt) 
        {
            return pos1 + dt * (pos2 - pos1);
        }
        
    }

}
