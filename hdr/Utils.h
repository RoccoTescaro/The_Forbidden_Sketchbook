#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"
#include <iostream>
#include <cmath>


#define ERROR(description) if(Config::debugMode) Utils::error(description); 
#define LOG(string, ...) if(Config::debugMode) Utils::log(string, __VA_ARGS__); 
#define ASSERT(condition, errorMessage) if(Config::debugMode) Utils::assert(condition, errorMessage); 
#define ASSERT(condition) if(Config::debugMode) Utils::assert(condition, #condition);  //display stringified condition

namespace Utils
{

	static void error(const char* description)
	{
		std::cout << "[ERROR] " << description << std::endl;
        exit(1);
	}

    template <typename T>
    static void print(std::string& text, std::size_t dim, std::size_t pos, T t)
    {
        std::size_t i = dim - pos;
        while (text.find("{" + std::to_string(i) + "}") != std::string::npos)
            text.replace(text.find("{" + std::to_string(i) + "}"), 3, std::to_string(t));
    }

    template <typename T, typename... Args>
    static void print(std::string& text, std::size_t dim, std::size_t pos, T t, Args... args)
    {
        std::size_t i = dim - pos;
        while (text.find("{" + std::to_string(i) + "}") != std::string::npos)
            text.replace(text.find("{" + std::to_string(i) + "}"), 3, std::to_string(t));
        print(text, dim, pos - 1, args...);
    }

    template<typename... Args>
    static void log(const char* text, Args... args)
    {
        std::string string{ text };
        print(string, sizeof...(args), sizeof...(args)-1, args...);
        std::cout << "[LOG] " << string << std::endl;
    }

	static void assert(bool condition, const char * errorMessage)
	{
        if (!condition) 
            ERROR(errorMessage);
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
        
        template<typename Type>
        static Type clamp(Type min, Type max, Type value)
        {
            return std::min(std::max(value,min),max);
        }
    }

}
