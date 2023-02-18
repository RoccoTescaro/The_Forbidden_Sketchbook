#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <queue>
#include "PathFinding.h"


class Map;

class GcBrain{

public:

    enum MovementType
    {
        ASTAR,
        DIGLETMOVEMENT,
        ENUM_SIZE //must be mainteined as last element of the enum, indicates the number of elements in the list
    };

    GcBrain(uint8_t maxEnergy, uint8_t energy, MovementType type);

//getter and setter

    inline bool isStepQueueEmpty(){     return stepQueue.empty();   };
    
    inline const uint8_t& getEnergy() const { return energy; };
    inline const uint8_t& getMaxEnergy() const { return maxEnergy; };


    inline void setEnergy(const uint8_t newEnergy) { energy=newEnergy; };
   
//turnChecks
    inline bool ready()             {    return (!stepQueue.empty() && energy>0);    };
    inline bool canExecute()        {    return (stepQueue.size()>1);    };
    bool canAttack(Map &map, float range);
   
//actions
    sf::Vector2<float> move(Map &map, sf::Sprite &sprite, sf::Vector2<float> pos, const float &dt);
    //attack

//miscellaneus
    void updateStepQueue( Map &map, const sf::Vector2<float> start, const sf::Vector2<float> target, bool solid);
    inline void queueClearCheck(){  if(stepQueue.size()==1)
                                        {   stepQueue.clear();  }   };

    inline void turnReset(){    setEnergy(maxEnergy);
                                stepQueue.clear();           };



private:

    std::unique_ptr<PathAlgorithm> movementStrategy;

    const uint8_t maxEnergy;
    uint8_t energy;

    const float animationSpeed = 300.f;

    std::deque<sf::Vector2<float>> stepQueue;

};