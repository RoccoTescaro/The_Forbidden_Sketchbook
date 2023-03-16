#pragma once
#include <SFML/Graphics.hpp>
#include "Config.h"
#include <iostream>
#include <cmath>
#include<sstream>

#define PI 3.14159265358979323846f

namespace Utils 
{

    template<typename T>
    static std::string to_string(const T& value) {
        std::ostringstream ss;
        ss << value;
        return ss.str();
    }

    template <typename T>
    static void print(std::string& text, std::size_t dim, std::size_t pos, T t)
    {
        std::size_t i = dim - pos;
        while (text.find("{" + std::to_string(i) + "}") != std::string::npos)
            text.replace(text.find("{" + std::to_string(i) + "}"), 3, to_string(t));
    }

    template <typename T, typename... Args>
    static void print(std::string& text, std::size_t dim, std::size_t pos, T t, Args... args)
    {
        std::size_t i = dim - pos;
        while (text.find("{" + std::to_string(i) + "}") != std::string::npos)
            text.replace(text.find("{" + std::to_string(i) + "}"), 3, to_string(t));
        print(text, dim, pos - 1, args...);
    }

    static void print(std::string& text, std::size_t dim, std::size_t pos)
    {
        return;
    }

    template<typename... Args>
    static void log(const std::string& text, Args... args)
    {
        std::string message{ text.c_str() };
        print(message, sizeof...(args), sizeof...(args) - 1, args...);
        std::cout << "[LOG] " << message << std::endl;
    }

    template<typename... Args>
    static void error(const char* message, Args... args)
    {
        std::string string{ message };
        print(string, sizeof...(args), sizeof...(args) - 1, args...);
        std::cout << "[ERROR] " << string << std::endl;
        exit(1);
    }

    /*
    template<typename T>
    static void ass(const T& condition, const char* expression)
    {
        if (condition)
            Utils::error(expression);
    }*/

    template<typename T, typename... Args>
    static void warning(const T& condition, const char* message, Args... args)
    {
        if (condition)
        {
            std::string string{ message };
            print(string, sizeof...(args), sizeof...(args) - 1, args...);
            std::cout << "[WARNING] " << string << std::endl;
        }
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
            return std::abs(pos2 - pos1);
        }

        template<typename Type>
        static Type manhattanDistance(const sf::Vector2<Type>& pos1, const sf::Vector2<Type>& pos2)
        {
            return distance(pos1.x, pos2.x) + distance(pos1.y, pos2.y);
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
            return std::min(std::max(value, min), max);
        }

        template<typename Type>
        static float mod(const sf::Vector2<Type>& vec)
        {
            return (float) Utils::Math::distance(vec, {0,0});
        }

        static sf::Vector2<float> normalize(const sf::Vector2<float>& vec)
        {
            float mod = Utils::Math::mod(vec);
            return vec/mod;
        }

    }

} // namespace utils

#define LOG(...) Utils::log(__VA_ARGS__)
#define ERROR(...) Utils::error(__VA_ARGS__)
//#define ASSERT(condition) Utils::assert(condition, #condition)
#define WARNING(...) Utils::warning(__VA_ARGS__)

