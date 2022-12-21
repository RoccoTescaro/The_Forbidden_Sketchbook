#pragma once

#include <queue>
#include <memory>
#include "Entity.h"

class Weapon{};
class PathAlgorithm{};
class AStar : public PathAlgorithm{};
class DiglettMovement : public PathAlgorithm{};

class GameCharacter : public Entity {
public:

    GameCharacter(  uint8_t maxHealth, uint8_t health, uint8_t maxEnergy, uint8_t energy, uint8_t priority):
                    maxHealth(maxHealth), health(health), maxEnergy(maxEnergy), energy(energy), priority(priority), animationSpeed(300), weapon(nullptr){};
    virtual ~GameCharacter()=default;

    void update  (Map &map, const float &dt) override;
    void execute (GameCharacter &gameCharacter, Map &map) override;
    inline void render  () override {   Entity::render();
                                        if(weapon) weapon->render();    };



//stats getter
    inline const bool isSolid() const override  {  return true;        }
    inline const uint8_t getMaxHealth() const   {  return maxHealth;   }
    inline const uint8_t getMaxEnergy() const   {  return maxEnergy;   }
    inline const uint8_t getHealth()    const   {  return health;      }
    inline const uint8_t getEnergy()    const   {  return energy;      }
    inline const uint8_t getPriority()  const   {  return priority;    }
    inline const uint8_t getRange()     const   {  if(weapon)
		                                            return weapon->getRange();
	                                            return 1;           }

//utility functions
    inline const bool isStepQueueEmpty()  const{  return stepQueue.empty();    }
    const bool isInRange(Map &map);

    void updateStepQueue(Map &map,const sf::Vector2<float> target) ;
    inline void roundReset(){   energy=maxEnergy;
                                stepQueue.clear();  }




protected:
//Game Stats
    const uint8_t priority;
    const uint8_t maxHealth;
    const uint8_t maxEnergy;
    uint8_t health;
    uint8_t energy;

    std::unique_ptr<Weapon> weapon;

//Movement Var
    std::unique_ptr<PathAlgorithm> movementStrategy;
    const float animationSpeed;

    std::deque<sf::Vector2<float>> stepQueue;
};

    //GAMECHARACTERS TYPES


struct BaseStats
{
    uint8_t maxHealth;
    uint8_t maxEnergy;
    uint8_t priority;
};


class Player : public GameCharacter {
public:
    Player(uint8_t health, uint8_t energy, bool filterColorR, bool filterColorG, bool filterColorB);

    inline const sf::Color& getFilterColor() const { return filterColor; };

private:
    static BaseStats baseStats;
    sf::Color filterColor;
};


class Melee : public GameCharacter {
public:
    Melee(uint8_t health, uint8_t energy);
private:
    static BaseStats baseStats;
};


class Bat : public GameCharacter {
public:
    Bat(uint8_t health, uint8_t energy);

    inline const bool isSolid() const  override  { return false; };
private:
    static BaseStats baseStats;
};


class Ranged : public GameCharacter {
public:
    Ranged(uint8_t health, uint8_t energy);

    void update(Map &map, const float &dt) override;
private:
    static BaseStats baseStats;
//Movement var
    const float animationDuration;
    float animationTime;
};
