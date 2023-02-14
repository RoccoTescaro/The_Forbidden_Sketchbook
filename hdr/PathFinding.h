#pragma once

#include "Utils.h"
#include <SFML/Graphics.hpp>
#include <queue>
#include <unordered_map>
#include <set>
#include <random>

class Map;

class PathAlgorithm
{
public:

    virtual std::deque<sf::Vector2<float>> findPath( Map &map, sf::Vector2<float> fStart,sf::Vector2<float> fTarget, bool flying)=0;


protected:
struct hash
    {
        std::size_t operator()(const sf::Vector2<int> &pos) const
        {
            auto h1 = std::hash<int>{}(pos.x);
            auto h2 = std::hash<int>{}(pos.y);
            return h1 ^ h2;
        }
    };

    bool isValid(Map &map, const sf::Vector2<int> pos, bool flying, bool ignoreGC=false);

};

class AStar : public PathAlgorithm
{
public:

    std::deque<sf::Vector2<float>> findPath( Map &map, sf::Vector2<float> fStart,sf::Vector2<float> fTarget, bool flying) override;

private:

    typedef std::pair<int, sf::Vector2<int>> Pair;
    struct fCompare
    {
        bool operator()(const Pair &p1,
                        const Pair &p2) const
        {
            return (p1.first <= p2.first);
        }
    };
    
    struct node
    {

        sf::Vector2i parentPos;
        int g;
        int h;
        int f;
    };

    std::deque<sf::Vector2<float>> tracePath(Map &map, std::unordered_map<sf::Vector2i, node, hash> nodeInfo, sf::Vector2<int> target);

    inline bool isDestination(const sf::Vector2<int> &pos, const sf::Vector2<int> &target){    return (pos==target);   };

};

class DigletMovement : public PathAlgorithm
{
public:

    std::deque<sf::Vector2<float>> findPath( Map &map, sf::Vector2<float> fStart,sf::Vector2<float> fTarget, bool flying) override;

};
