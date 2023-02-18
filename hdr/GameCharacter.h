#pragma once

#include <queue>
#include <memory>
#include "Entity.h"
#include "PathFinding.h"
#include "Utils.h"
#include "GcBrain.h"

class Weapon;

class GameCharacter : public Entity
{
public:
    GameCharacter(uint8_t maxHealth, uint8_t health, uint8_t maxEnergy, uint8_t energy, uint8_t priority, GcBrain::MovementType movementType ) 
        : maxHealth(maxHealth), health(health), priority(priority), gcBrain(maxEnergy, energy, movementType) {};
    GameCharacter() :  priority(0), gcBrain(20, 20, GcBrain::ASTAR) {};


    //ENTITY
    void update(Map &map, const float &dt) override;
    void execute(GameCharacter &gameCharacter, Map &map) override;
    inline void render() override 
    {   
        Entity::render();
        //weapon->render();    
    };
    
    //GAMECHARACTER GET&SET
    inline bool isSolid() const override { return true; };
    inline uint8_t getMaxHealth() const { return maxHealth; }; //need to return as a reference and not by copy to allow hud auto update
    inline uint8_t getMaxEnergy() const { return gcBrain.getMaxEnergy();; }; //need to return as a reference and not by copy to allow hud auto update
    inline const uint8_t& getHealth() const { return health; };
    inline const uint8_t& getEnergy() const { return gcBrain.getEnergy(); };
    inline uint8_t getPriority() const { return priority; };
    inline float getRange() const { return 1; /*return weapon->getRange()*/};


    //GAMECHARACTER FUNCTIONS
    void updateStepQueue( Map &map, const sf::Vector2<float> target);
    inline void turnReset(){    gcBrain.turnReset();          };
    inline bool isStepQueueEmpty(){     return gcBrain.isStepQueueEmpty();   };

    inline void serialize(Archive& fs) 
    {
        Entity::serialize(fs);
        fs.serialize(health);
        //fs.serialize(stepQueue);
    }
protected:
    //STATS
    const uint8_t maxHealth = 0;  
    const uint8_t priority = 0;
    uint8_t health = 0;
    //Weapon weapon; //std::unique_ptr<Weapon> weapon = nullptr;

    //MOVEMENT
    GcBrain gcBrain;
};


class Player : public GameCharacter
{
public:
    Player(uint8_t health, uint8_t energy, uint8_t filterColorR, uint8_t filterColorG, uint8_t filterColorB);
    Player() : Player(100,100,255,255,255) {}; //should initialize by the list initialization the const memeber that we dont wont/cant serialize

    inline const sf::Color& getFilterColor() const { return filterColor; };
  
    void serialize(Archive& fs) override 
    {
        GameCharacter::serialize(fs);
        //fs.serialize(filterColor);
    }

private:
    static Serializable* create() { return new Player; };
    static Register registration;
    sf::Color filterColor; 
};


class Melee : public GameCharacter
{
public:
    Melee(uint8_t health, uint8_t energy);
    Melee() {}; //should initialize by the list initialization the const memeber that we dont wont/cant serialize

    void serialize(Archive& fs) override
    {
        GameCharacter::serialize(fs);
        //fs.serialize(filterColor);
    }
};


class Bat : public GameCharacter
{
public:
    Bat(uint8_t health, uint8_t energy);

    inline bool isSolid() const override { return false; };

    void serialize(Archive& fs) override 
    {
        GameCharacter::serialize(fs);
    }
};


class Ranged : public GameCharacter
{
public:
    Ranged(uint8_t health, uint8_t energy);
    Ranged() {}; //should initialize by the list initialization the const memeber that we dont wont/cant serialize

    void update(Map &map, const float &dt) override;

    void serialize(Archive& fs) override 
    {
        GameCharacter::serialize(fs);
        fs.serialize(animationTime);
    }
private:
    const float animationDuration = 0.f;
    float animationTime = 0.f;
};
