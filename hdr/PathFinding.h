#pragma once
#include "Utils.h"
#include <SFML/Graphics.hpp>
#include <queue>
#include <unordered_map>
#include <set>
#include <random>

//todo using for long types

class Map;

class PathAlgorithm
{
public:

    virtual std::deque<sf::Vector2<float>> findPath( Map &map, sf::Vector2<float> fStart,sf::Vector2<float> fTarget, bool flying) = 0;

    inline uint8_t getMovementCost() const { return movementCost; };

protected:

    struct Hash
    {
        std::size_t operator()(const sf::Vector2<int> &pos) const
        {
            auto h1 = std::hash<int>{}(pos.x);
            auto h2 = std::hash<int>{}(pos.y);
            return h1 ^ h2;
        }
    };

    bool isValid(Map &map, const sf::Vector2<int> pos, bool flying, bool ignoreGC=false);

    const uint8_t movementCost = 1; 
};

class AStar : public PathAlgorithm
{
public:

    std::deque<sf::Vector2<float>> findPath( Map &map, sf::Vector2<float> fStart,sf::Vector2<float> fTarget, bool flying) override;

private:

    using Pair = std::pair<int, sf::Vector2<int>>;
    struct Compare
    {
        bool operator()(const Pair &p1, const Pair &p2) const
        {
            return (p1.first <= p2.first);
        }
    };
    
    struct Node
    {
        sf::Vector2<int> parentPos;
        int g;
        int h;
        int f;
    };

    std::deque<sf::Vector2<float>> tracePath(Map &map, std::unordered_map<sf::Vector2i, Node, Hash> nodeInfo, sf::Vector2<int> target);
};

class DigletMovement : public PathAlgorithm
{
public:

    std::deque<sf::Vector2<float>> findPath( Map &map, sf::Vector2<float> fStart,sf::Vector2<float> fTarget, bool flying) override;

};
